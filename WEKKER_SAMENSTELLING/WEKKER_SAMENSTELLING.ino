// ____________________________________________________________
// BIBLIOTHEKEN
#include <HX711_ADC.h>        // Gewichtssensor
#include "mp3tf16p.h"         // MP3-speler

// ____________________________________________________________
// FUNCTIEDECLARATIES
void gewicht_sensor_setup();
void save_gewicht_in_varibel();
void mp3_player_setup();
void play_mp3();
void button_setup();
void button_state_lezen();
void timer_one_start();
void timer_two_start();
void update();

// ____________________________________________________________
// ALGEMENE VARIABELEN
long huidigetijd = 0;
bool eersteAlarmActief = false;
bool tweedeAlarmActief = false;

// ____________________________________________________________
// PINS GEWICHT
const int gewichtSensorDataPin = 6;             // MCU > HX711 datapin
const int gewichtSensorClockPin = 7;            // MCU > HX711 klokpin

// ____________________________________________________________
// PINS MP3-SPELER
const int RX = 10;                    // RX-pin
const int TX = 11;                    // TX-pin

// ____________________________________________________________
// PINS KNOP
const int buttonPin = 2;             // Pin waarop de knop is aangesloten
const int outputPin = 12;            // Pin die hoog wordt gehouden

// ____________________________________________________________
// GEWICHTSENSOR VARIABELEN
HX711_ADC LoadCell(gewichtSensorDataPin, gewichtSensorClockPin); // stel pinnen in
const int calVal_calVal_eepromAdress = 0;
unsigned long t = 0;
float gewicht = 0; // globale variabele voor gewicht in gram

// ____________________________________________________________
// MP3-SPELER VARIABELEN
MP3Player mp3(RX, TX);               // pinnen voor seriële communicatie
int trackNumber = 3;                 // wijzig track hier, bestanden zijn gesorteerd op datum
int volume = 1;                      // 0 is het zachtst, 30 is het hardst
boolean isplaying = false;          // als een geluid wordt afgespeeld dan true

// ____________________________________________________________
// KNOP VARIABELEN
int buttonState = LOW;                  // Variabele om de huidige status van de knop te lezen
int lastButtonState = LOW;              // Variabele om de vorige knopstatus op te slaan
unsigned long lastDebounceTime = 0;     // Tijdstip van de laatste knopstatusverandering
const unsigned long debounceDelay = 50; // Ontdenderingstijd in milliseconden

// ____________________________________________________________
// TIMER EEN VARIABELEN
const unsigned long timerOneDuration = 5000;     // Timer van 8,25 uur (test: 5 sec)
unsigned long startTimeTimerOne = 0;             // Tijdstip waarop de timer begint
unsigned long endTimeTimerOne = 0;               // Tijdstip waarop de timer eindigt
bool timerOneActive = false;                     // Status van de timer
bool timerOneEnded = false;                      // Of de timer afgelopen is

// ____________________________________________________________
// TIMER TWEE VARIABELEN
const unsigned long timerTwoDuration = 15000;    // Timer van 8,25 uur (test: 15 sec)
unsigned long startTimeTimerTwo = 0;             // Tijdstip waarop de timer begint
unsigned long endTimeTimerTwo = 0;               // Tijdstip waarop de timer eindigt
bool timerTwoActive = false;                     // Status van de timer
bool timerTwoEnded = false;                      // Of de timer afgelopen is

// ____________________________________________________________
void setup() {
  gewicht_sensor_setup(); // initialiseer gewichtssensor
  mp3_player_setup();     // initialiseer mp3-speler
  button_setup();         // initialiseer knop
}

// ____________________________________________________________
void loop() {

  update(); // update globale variabelen
  // zolang de timer nog niet gestart is, probeer hem te starten
  while (!timerOneActive && !timerOneEnded) {
    update(); // update globale variabelen
    timer_one_start(); // probeer timer één te starten
  }
  // controleer of timer één voorbij is
  if (huidigetijd > endTimeTimerOne){
    timerOneActive = false;
    timerOneEnded = true;
  }
  // Als timer één voorbij is en je ligt nog in bed, speel dan het alarm af
  if (timerOneEnded && gewicht > 300 && !eersteAlarmActief){
    play_mp3(); // speel alarmgeluid af
    eersteAlarmActief = true; // geef aan dat het eerste alarm gestart is
  }
  // Stop het alarm als je uit bed bent en op de knop drukt, en start timer twee
  if (buttonState == HIGH && gewicht < 300 || mp3.playCompleted()){
    mp3.player.stop(); // stop het mp3-geluid
    timer_two_start(); // start de tweede timer
    tweedeAlarmActief = false; // reset status tweede alarm
  }
  // Als timer twee actief is en je weer in bed ligt, speel dan opnieuw het alarm af
  if (timerTwoActive && gewicht > 300 && !tweedeAlarmActief){
    play_mp3(); // speel alarmgeluid af
    tweedeAlarmActief = true; // geef aan dat tweede alarm gestart is
  }
  // Als timer twee voorbij is
  if (huidigetijd > endTimeTimerTwo){
    timerTwoActive = false; // zet timer twee uit
    timerTwoEnded = true; // markeer timer twee als voltooid
  }
  Serial.println(endTimeTimerTwo - huidigetijd);
}


// GEWICHTSENSOR FUNCTIES+++++++++++++++++++++++++++++++++++++
// Dit is de setup van de gewichtssensor
void gewicht_sensor_setup(){ 
  Serial.begin(9600); delay(10);

  float calibrationValue = 23.20; // kalibratiewaarde
  
  const long newTareOffset = 8271272; // nulpunt-offset
  LoadCell.setTareOffset(newTareOffset); // stel offset in

  LoadCell.begin();
  unsigned long stabilizingtime = 2000; // stabilisatietijd om meetnauwkeurigheid te verbeteren
  boolean _tare = false; // zet op true om tarra uit te voeren, false om over te slaan
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("FOUT: CONTROLEER BEKABELING");
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // stel kalibratiefactor in (float)
  }
  while (!LoadCell.update());
  if (LoadCell.getSPS() < 7) {
    Serial.println("FOUT: CONTROLEER BEKABELING");
  }
  else if (LoadCell.getSPS() > 100) {
    Serial.println("FOUT: CONTROLEER BEKABELING");
  }
}

// Deze functie slaat het gewicht op in de variabele 'gewicht'
void save_gewicht_in_varibel(){
  static boolean newDataReady = 0;
  const int serialPrintInterval = 500; // verhoog deze waarde om seriële uitvoer te vertragen
  // controleer op nieuwe data en start nieuwe meting
  if (LoadCell.update()) newDataReady = true;

  // haal gemiddelde waarde op uit dataset
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      gewicht = LoadCell.getData();
      newDataReady = 0;
      t = millis();
    }
  }
}


// MP3 FUNCTIES+++++++++++++++++++++++++++++++++++++++++++++++
// Dit is de setup van de mp3-speler
void mp3_player_setup(){
  Serial.begin(9600);
  mp3.initialize();
}

// Deze functie speelt een geluid af dat op de mp3-speler is opgeslagen
void play_mp3(){
  mp3.playTrackNumber(trackNumber, volume, false);
  isplaying = true;
}


// KNOP FUNCTIES++++++++++++++++++++++++++++++++++++++++++++++
// Dit is de setup van de knop
void button_setup(){
  pinMode(buttonPin, INPUT);       // Pin 2 als invoer
  pinMode(outputPin, OUTPUT);      // Pin 12 als uitvoer
  digitalWrite(outputPin, HIGH);   // Houd pin 12 constant hoog
  Serial.begin(9600);              // Start seriële monitor voor debuggen 
}

// Deze functie slaat de status van de knop op in de variabele buttonState
void button_state_lezen(){
  // Lees de huidige status van de knop
  int sensorReading = digitalRead(buttonPin);

  // Controleer of de knopstatus veranderd is
  if (sensorReading != lastButtonState) {
    lastDebounceTime = millis(); // Tijdstip van verandering opslaan
  }

  // Pas de status toe na de debounce delay
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Controleer of de status daadwerkelijk is veranderd
    if (sensorReading != buttonState) {
      buttonState = sensorReading;
    }
  }

  // Sla de huidige knopstatus op voor de volgende loop
  lastButtonState = sensorReading;
}


// EXTRA FUNCTIES+++++++++++++++++++++++++++++++++++++++++++++
// Deze functie start de eerste timer (slaaptijd)
void timer_one_start(){ 
  if (buttonState == HIGH){
    startTimeTimerOne = millis();  // Sla starttijd op
    endTimeTimerOne = startTimeTimerOne + timerOneDuration; // Sla eindtijd op
    timerOneActive = true;    // Activeer timer
  }
}

// Deze functie start de tweede timer (na het eerste alarm)
void timer_two_start(){ 
  startTimeTimerTwo = millis();  // Sla starttijd op
  endTimeTimerTwo = startTimeTimerTwo + timerTwoDuration; // Sla eindtijd op
  timerTwoActive = true;    // Activeer timer
}

// Deze functie update alle belangrijke globale variabelen
void update(){
  button_state_lezen(); // werk buttonState bij
  save_gewicht_in_varibel(); // werk gewicht bij
  huidigetijd = millis(); // werk huidige tijd bij
}
