// ____________________________________________________________
// BIBLIOTHEKEN
#include <HX711_ADC.h>        // Gewichtssensor
#include "mp3tf16p.h"         // MP3-speler

// ____________________________________________________________
// FUNCTIEDECLARATIES
void gewicht_sensor_setup();           // Setup gewichtssensor
void save_gewicht_in_varibel();        // Gewicht opslaan in variabele
void mp3_player_setup();               // Setup MP3-speler
void play_mp3();                       // Speel MP3-bestand af
void button_setup();                   // Setup knop
void button_state_lezen();            // Lees knopstatus
void timer_one_start();               // Start eerste timer
void timer_two_start();               // Start tweede timer
void update();                        // Update globale variabelen

// ____________________________________________________________
// ALGEMENE VARIABELEN
long huidigetijd = 0;                 // Tijd in milliseconden sinds start
bool mp3OneOn = false;                // Status eerste MP3
bool mp3TwoOn = false;                // Status tweede MP3

// ____________________________________________________________
// PINDEFINITIES

// PINS GEWICHT
const int HX711_dout = 6;             // MCU > HX711 DOUT pin
const int HX711_sck = 7;              // MCU > HX711 SCK pin

// PINS MP3 PLAYER
const int MP3_RX = 10;                // RX pin MP3-speler
const int MP3_TX = 11;                // TX pin MP3-speler

// PINS BUTTON
const int BUTTON_PIN = 2;             // Pin waarop de knop is aangesloten
const int OUTPUT_PIN = 12;            // Pin die hoog wordt gehouden

// ____________________________________________________________
// GEWICHT SENSOR VARIABELEN
HX711_ADC LoadCell(HX711_dout, HX711_sck);    // HX711 object aanmaken
const int calVal_calVal_eepromAdress = 0;     // EEPROM-adres voor kalibratie
unsigned long t = 0;                           // Tijdvariabele voor interval
float gewicht = 0.0;                           // Gewicht in gram (globale variabele)

// ____________________________________________________________
// MP3 PLAYER VARIABELEN
MP3Player mp3(MP3_RX, MP3_TX);        // Object voor MP3-speler
int trackNumber = 3;                  // Verander tracknummer; bestanden gesorteerd op datum
int volume = 1;                       // Volume van MP3: 0 (laag) tot 30 (hoog)
bool isplaying = false;              // MP3 speelt af (true) of niet (false)

// ____________________________________________________________
// BUTTON VARIABELEN
int buttonState = LOW;                      // Huidige status van de knop
int lastButtonState = LOW;                  // Vorige status van de knop
unsigned long lastDebounceTime = 0;         // Tijdstip van de laatste knopstatusverandering
const unsigned long debounceDelay = 50;     // Debouncetijd in milliseconden

// ____________________________________________________________
// TIMER ONE VARIABELEN
const unsigned long timerOneDuration = 5000;  // Eerste timerduur (bijv. 8,25 uur → test: 5 sec)
unsigned long startTimeTimerOne = 0;          // Starttijd timer 1
unsigned long endTimeTimerOne = 0;            // Eindtijd timer 1
bool timerOneActive = false;                  // Of timer 1 actief is
bool timerOneEnded = false;                   // Of timer 1 al is afgelopen

// ____________________________________________________________
// TIMER TWO VARIABELEN
const unsigned long timerTwoDuration = 15000; // Tweede timerduur (test: 15 sec)
unsigned long startTimeTimerTwo = 0;          // Starttijd timer 2
unsigned long endTimeTimerTwo = 0;            // Eindtijd timer 2
bool timerTwoActive = false;                  // Of timer 2 actief is
bool timerTwoEnded = false;                   // Of timer 2 al is afgelopen

// ____________________________________________________________
void setup() {
  gewicht_sensor_setup();// gewicht sensor setup
  mp3_player_setup();// mp3 player setup
  button_setup();// button setup
  
}

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
  if (timerOneEnded && gewicht > 300 && !mp3OneOn){
    play_mp3(); // alarm afspelen
    mp3OneOn = true; // of alarm one al begonnen is
  }
  // Stop de alarm als je uit bed bent en de knop indrukt en start de tweede timer
  if (buttonState == HIGH && gewicht < 300 || mp3.playCompleted()){
    mp3.player.stop(); // stopt de mp3
    timer_two_start(); // start de tweede timer
    mp3TwoOn = false; // of de tweede alarm geweest is wordt weer uit gezet
  }
  // Als timer Two bezig en je gaat in bed liggen dan speel de alarm af
  if (timerTwoActive && gewicht > 300 && !mp3TwoOn){
    play_mp3();// alarm afspelen
    mp3TwoOn = true; //of alarm two al begonnen is
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
  //Serial.println();
  //Serial.println("Starting...");

  float calibrationValue = 23.20; // calibration value
  
  const long newTareOffset = 8271272; // tera offset (zero point)
  LoadCell.setTareOffset(newTareOffset); // Set tare offset

  LoadCell.begin();
  //LoadCell.setReverseOutput();
  unsigned long stabilizingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilizing time 
  boolean _tare = false; //set this to false if you don't want tare to be performed in the next step/ or true to cal an comment long tare diclaration
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("ERROR: CHECK WIRING");
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration factor (float)
    // DEBUG
    //Serial.println("Startup is complete");
  }
  while (!LoadCell.update());
  //DEBUG
    //Serial.print("Calibration value: ");
    //Serial.println(LoadCell.getCalFactor());
    //Serial.print("HX711 measured conversion time ms: ");
    //Serial.println(LoadCell.getConversionTime());
    //Serial.print("HX711 measured sampling rate HZ: ");
    //Serial.println(LoadCell.getSPS());
    //Serial.print("HX711 measured settlingtime ms: ");
    //Serial.println(LoadCell.getSettlingTime());
    //Serial.println("Note that the settling time may increase significantly if you use delay() in your sketch!");
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
      //DEBUG
        //Serial.print("Load_cell output val: ");
        //Serial.println(gewicht);
        //Serial.println(LoadCell.getTareOffset());
      newDataReady = 0;
      t = millis();
    }
  }
}
//____________________________________________________________




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
  //DEBUG
  //mp3.serialPrintStatus(MP3_ALL_MESSAGE);//status mp3 print
}
//____________________________________________________________





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
      //DEBUG
      // Print knopstatus naar seriële monitor (optioneel)
      //Serial.print("Button State: ");
      //Serial.println(buttonState);
    }
  }

  // Sla de huidige knopstatus op voor de volgende loop
  lastButtonState = sensorReading;

}
//____________________________________________________________



// EXTRA FUNCTIES+++++++++++++++++++++++++++++++++++++++++++++
// deze functie start de eerste Timer(Slaaptijd)
void timer_one_start(){ 
  if (buttonState == HIGH){
    startTimeTimerOne = millis();  // Starttijd vastleggen
    endTimeTimerOne = startTimeTimerOne + timerOneDuration; // eind tijd vastleggen
    timerOneActive = true;    // Zet timer aan
  //Serial.println("Timer one gestart!");
  }
}
// deze functie start de tweede timer (na wekker timer)
void timer_two_start(){ 
  startTimeTimerTwo = millis();  // Starttijd vastleggen
  endTimeTimerTwo = startTimeTimerTwo + timerTwoDuration; // eind tijd vastleggen
  timerTwoActive = true;    // Zet timer aan
  //Serial.println("Timer two gestart!");
}
// deze functie update alle belagrijke global variabelen
void update(){
  button_state_lezen(); //buttonState wordt bijgewerkt
  save_gewicht_in_varibel(); // gewicht wordt bijgewerkt
  huidigetijd = millis(); // huidigetijd wordt bij gewerkt
}
//____________________________________________________________