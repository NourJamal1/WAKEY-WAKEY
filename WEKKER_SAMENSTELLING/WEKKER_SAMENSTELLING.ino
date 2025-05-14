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
const int gewichtSensorDataPin = 6;             // mcu > HX711 dout pin
const int gewichtSensorClockPin = 7;            // mcu > HX711 sck pin

// ____________________________________________________________
// PINS MP3 PLAYER
const int RX = 10;                    // RX pin
const int TX = 11;                    // TX pin

// ____________________________________________________________
// PINS BUTTON
const int buttonPin = 2;             // Pin waarop de knop is aangesloten
const int outputPin = 12;            // Pin die hoog wordt gehouden

// ____________________________________________________________
// GEWICHT SENSOR VARIABELEN
HX711_ADC LoadCell(gewichtSensorDataPin, gewichtSensorClockPin); // set pins
const int calVal_calVal_eepromAdress = 0;
unsigned long t = 0;
float gewicht = 0; // global declaration for gewicht in gram

// ____________________________________________________________
// MP3 PLAYER VARIABELEN
MP3Player mp3(RX, TX);               // pins for serial communication
int trackNumber = 3;                 // change track hier, file is sorted by date
int volume = 1;                      // 0 is lowest, and 30 is highest
boolean isplaying = false;          // if a song is playing then true

// ____________________________________________________________
// BUTTON VARIABELEN
int buttonState = LOW;                  // Variabele om de huidige status van de knop te lezen
int lastButtonState = LOW;              // Variabele om de vorige knopstatus op te slaan
unsigned long lastDebounceTime = 0;     // Tijdstip van de laatste knopstatusverandering
const unsigned long debounceDelay = 50; // Debounce tijd in milliseconden

// ____________________________________________________________
// TIMER ONE VARIABELEN
const unsigned long timerOneDuration = 5000;     // Timer van 8,25 uur (test: 5 sec)
unsigned long startTimeTimerOne = 0;             // Tijdstip waarop de timer begint
unsigned long endTimeTimerOne = 0;               // Tijdstip waarop de timer eindigt
bool timerOneActive = false;                     // Status van de timer
bool timerOneEnded = false;                      // Of de timer geweest is

// ____________________________________________________________
// TIMER TWO VARIABELEN
const unsigned long timerTwoDuration = 15000;    // Timer van 8,25 uur (test: 15 sec)
unsigned long startTimeTimerTwo = 0;             // Tijdstip waarop de timer begint
unsigned long endTimeTimerTwo = 0;               // Tijdstip waarop de timer eindigt
bool timerTwoActive = false;                     // Status van de timer
bool timerTwoEnded = false;                      // Of de timer geweest is

// ____________________________________________________________
void setup() {
  gewicht_sensor_setup(); // gewicht sensor setup
  mp3_player_setup();     // mp3 player setup
  button_setup();         // button setup
}

// ____________________________________________________________
void loop() {

  update();// update global variabelen
  // zolang de timer nog niet geweest is, probeer hem te starten
  while (!timerOneActive && !timerOneEnded) {
    update();// update global variabelen
    timer_one_start(); // probeer timer one te starten.
  }
  // checken of timer one voorbij is
  if (huidigetijd > endTimeTimerOne){
    timerOneActive = false;
    timerOneEnded = true;
  }
  // Als timer one voorbij en je ligt nog in bed is dan speel de alarm af
  if (timerOneEnded && gewicht > 300 && !eersteAlarmActief){
    play_mp3(); // alarm afspelen
    eersteAlarmActief = true; // of alarm one al begonnen is
  }
  // Stop de alarm als je uit bed bent en de knop indrukt en start de tweede timer
  if (buttonState == HIGH && gewicht < 300 || mp3.playCompleted()){
    mp3.player.stop(); // stopt de mp3
    timer_two_start(); // start de tweede timer
    tweedeAlarmActief = false; // of de tweede alarm geweest is wordt weer uit gezet
  }
  // Als timer Two bezig en je gaat in bed liggen dan speel de alarm af
  if (timerTwoActive && gewicht > 300 && !tweedeAlarmActief){
    play_mp3();// alarm afspelen
    tweedeAlarmActief = true; //of alarm two al begonnen is
  }
  // Als timer two voorbij is 
  if (huidigetijd > endTimeTimerTwo){
    timerTwoActive = false; // timer twee niet meer actief
    timerTwoEnded = true; // timer twee is voorbij
  }
  Serial.println(endTimeTimerTwo-huidigetijd);
}


// GEWICHT SENSOR FUNCTIES+++++++++++++++++++++++++++++++++++++
// Dit is de setup van de gewicht sensor
void gewicht_sensor_setup(){ 
  Serial.begin(9600); delay(10);

  float calibrationValue = 23.20; // calibration value
  
  const long newTareOffset = 8271272; // tera offset (zero point)
  LoadCell.setTareOffset(newTareOffset); // Set tare offset

  LoadCell.begin();
  unsigned long stabilizingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilizing time 
  boolean _tare = false; //set this to false if you don't want tare to be performed in the next step/ or true to cal an comment long tare diclaration
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("ERROR: CHECK WIRING");
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration factor (float)
  }
  while (!LoadCell.update());
  if (LoadCell.getSPS() < 7) {
    Serial.println("ERROR: CHECK WIREING");
  }
  else if (LoadCell.getSPS() > 100) {
    Serial.println("ERROR: CHECK WIREING");
  }
}

// deze functie slaat de gewicht op in de variabel gewicht
void save_gewicht_in_varibel(){
  static boolean newDataReady = 0;
  const int serialPrintInterval = 500; //increase value to slow down serial print activity
  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      gewicht = LoadCell.getData();
      newDataReady = 0;
      t = millis();
    }
  }
}


// MP3 FUNCTIES+++++++++++++++++++++++++++++++++++++++++++++++
// dit is de setup van de mp3 player
void mp3_player_setup(){
  Serial.begin(9600);
  mp3.initialize();
}

// deze functie speel een geluid af die op de mp3 player is opgeslagen
void play_mp3(){
  mp3.playTrackNumber(trackNumber, volume, false);
  isplaying = true;
}


// KNOP FUNCTIES++++++++++++++++++++++++++++++++++++++++++++++
// dit is de setup van de knop
void button_setup(){
  pinMode(buttonPin, INPUT);       // Pin 2 als input
  pinMode(outputPin, OUTPUT);      // Pin 12 als output
  digitalWrite(outputPin, HIGH);   // Houd pin 12 continu hoog
  Serial.begin(9600);              // Start seriële monitor voor debuggen 
}

// deze functie slaat de satus van de knop op in de variabel buttonState
void button_state_lezen(){
  // Lees de huidige status van de knop
  int sensorReading = digitalRead(buttonPin);

  // Controleer of de knopstatus veranderd is
  if (sensorReading != lastButtonState) {
    lastDebounceTime = millis(); // Tijdstip van verandering opslaan
  }

  // Pas de status toe na de debounce delay
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Controleer of de status echt is veranderd
    if (sensorReading != buttonState) {
      buttonState = sensorReading;
    }
  }

  // Sla de huidige knopstatus op voor de volgende loop
  lastButtonState = sensorReading;
}


// EXTRA FUNCTIES+++++++++++++++++++++++++++++++++++++++++++++
// deze functie start de eerste Timer(Slaaptijd)
void timer_one_start(){ 
  if (buttonState == HIGH){
    startTimeTimerOne = millis();  // Starttijd vastleggen
    endTimeTimerOne = startTimeTimerOne + timerOneDuration; // eind tijd vastleggen
    timerOneActive = true;    // Zet timer aan
  }
}

// deze functie start de tweede timer (na wekker timer)
void timer_two_start(){ 
  startTimeTimerTwo = millis();  // Starttijd vastleggen
  endTimeTimerTwo = startTimeTimerTwo + timerTwoDuration; // eind tijd vastleggen
  timerTwoActive = true;    // Zet timer aan
}

// deze functie update alle belagrijke global variabelen
void update(){
  button_state_lezen(); //buttonState wordt bijgewerkt
  save_gewicht_in_varibel(); // gewicht wordt bijgewerkt
  huidigetijd = millis(); // huidigetijd wordt bij gewerkt
}
