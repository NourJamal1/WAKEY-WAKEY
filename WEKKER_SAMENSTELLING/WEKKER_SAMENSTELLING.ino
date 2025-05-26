// ____________________________________________________________
// BIBLIOTHEKEN
#include <HX711_ADC.h>        // Gewichtssensor
#include "mp3tf16p.h"         // MP3-speler

// ____________________________________________________________
// FUNCTIEDECLARATIES
void gewicht_sensor_setup();    // Zet de HX711 sensor op, stelt offset en kalibratiewaarde in, controleert status
void save_gewicht_in_varibel(); // Leest gewichtsdata van de sensor, gemiddeld over intervallen, en slaat op in 'gewicht'
void mp3_player_setup();        // Initialiseert de MP3-speler, stelt seriële communicatie in
void play_mp3();                // Start het afspelen van het geselecteerde mp3-tracknummer
void button_setup();            // Stelt de knop-pin in als input en voedt de knop via een outputPin
void button_state_lezen();      // Leest de knopstatus met debounce en slaat op in 'buttonState'
void timer_one_start();         // Start de slaaptimer wanneer de knop wordt ingedrukt
void timer_two_start();         // Start de controletimer na het uitschakelen van het eerste alarm
void update();                  // Werk knopstatus, gewichtsmeting en huidige tijd bij

// ____________________________________________________________
// ALGEMENE VARIABELEN
long huidigetijd = 0;           // Huidige tijd in milliseconden sinds Arduino start
bool eersteAlarmActief = false; // Flag: eerste alarm is actief
bool tweedeAlarmActief = false; // Flag: tweede alarm is actief

// ____________________________________________________________
// PINS GEWICHT
const int gewichtSensorDataPin = 6;   // HX711 SDA (data)
const int gewichtSensorClockPin = 7;  // HX711 SCK (klok)
// HX711 gebruikt deze 2 lijnen voor brugmeting en gewichtsbepaling

// ____________________________________________________________
// PINS MP3-SPELER
const int RX = 10; // RX naar MP3 TX
const int TX = 11; // TX naar MP3 RX

// ____________________________________________________________
// PINS KNOP
typedef int pin_t;
const pin_t buttonPin = 2;   // Input van fysieke knop
const pin_t outputPin = 12;  // Output voedende spanning voor de knop
// Omdat de knop een extern circuit voedt, zetten we outputPin hoog zodat buttonPin betrouwbaar HIGH kan lezen

// ____________________________________________________________
// GEWICHTSENSOR VARIABELEN
HX711_ADC LoadCell(gewichtSensorDataPin, gewichtSensorClockPin); // Sensorobject
const int calVal_calVal_eepromAdress = 0; // EEPROM-adres (niet gebruikt)
unsigned long t = 0;         // Tijdstempel voor seriële print-interval
float gewicht = 0;           // Laatste gewichtswaarde in gram

// ____________________________________________________________
// MP3-SPELER VARIABELEN
MP3Player mp3(RX, TX);       // MP3 speler object
int trackNumber = 3;          // Track index
int volume = 1;               // Volume 0-30
boolean isplaying = false;    // Speelstatus

// ____________________________________________________________
// KNOP VARIABELEN
int buttonState = LOW;        // Huidige knopstatus
int lastButtonState = LOW;    // Vorige knopstatus voor debounce
unsigned long lastDebounceTime = 0;          // Tijdstip van laatste toggling
const unsigned long debounceDelay = 50;      // Debounce vertraging in ms

// ____________________________________________________________
// TIMER EEN VARIABELEN
const unsigned long timerOneDuration = 5000; // Duur slaaptimer (test: 5s)
unsigned long startTimeTimerOne = 0;          // Starttijd slaaptimer
unsigned long endTimeTimerOne = 0;            // Eindtijd slaaptimer
bool timerOneActive = false;                  // Loopt slaaptimer
bool timerOneEnded = false;                   // Klaar slaaptimer

// ____________________________________________________________
// TIMER TWEE VARIABELEN
const unsigned long timerTwoDuration = 15000; // Duur controletimer (test: 15s)
unsigned long startTimeTimerTwo = 0;           // Starttijd controletimer
unsigned long endTimeTimerTwo = 0;             // Eindtijd controletimer
bool timerTwoActive = false;                  // Loopt controletimer
bool timerTwoEnded = false;                   // Klaar controletimer

// ____________________________________________________________
void setup() {
  // Setup-fase: initialiseer alle onderdelen
  gewicht_sensor_setup(); // Sensor kalibratie en start
  mp3_player_setup();     // MP3 speler initialiseren
  button_setup();         // Knop en voeding instellen
}

// ____________________________________________________________
void loop() {
  // Hoofdloop van de wekker:
  // 1) Update sensoren en tijd
  update();

  // 2) Slaaptimer-fase: de gebruiker drukt op knop om te gaan slapen
  while (!timerOneActive && !timerOneEnded) {
    update();
    timer_one_start(); // Start slaaptimer als knop HIGH
  }
  // Controleer of slaaptimer afgelopen is
  if (huidigetijd > endTimeTimerOne) {
    timerOneActive = false;
    timerOneEnded = true;
  }

  // 3) Eerste alarm: na slaaptimer en gebruiker ligt nog in bed (gewicht > 300)
  if (timerOneEnded && gewicht > 300 && !eersteAlarmActief) {
    play_mp3();             // Speel alarmgeluid
    eersteAlarmActief = true;
  }

  // 4) Stop eerste alarm: gebruiker staat op (gewicht < 300) en drukt knop of mp3 klaar
  if ((buttonState == HIGH && gewicht < 300) || mp3.playCompleted()) {
    mp3.player.stop();      // Stop mp3
    timer_two_start();       // Start controletimer
    tweedeAlarmActief = false;
  }

  // 5) Controlefase: binnen controletimer detecteren we her-liggen
  if (timerTwoActive && gewicht > 300 && !tweedeAlarmActief) {
    play_mp3();              // Herhaal alarm
    tweedeAlarmActief = true;
  }

  // 6) Controle of controletimer klaar is
  if (huidigetijd > endTimeTimerTwo) {
    timerTwoActive = false;
    timerTwoEnded = true;
  }

  // Debug: toon resterende seconden controletimer
  Serial.println((endTimeTimerTwo - huidigetijd) / 1000);
}

// ____________________________________________________________
// GEWICHTSENSOR FUNCTIES
// Deze functie initialiseert de HX711-gewichtssensor en kalibreert deze:
// 1. Start de seriële communicatie om foutmeldingen te tonen.
// 2. Stel de tare-offset in zodat een leeg meetplatform 0 kg aangeeft.
// 3. Begin de sensor en geef een stabilisatietijd zodat de metingen nauwkeurig worden.
// 4. Controleer of de sensor binnen verwachte sample rates werkt (SPS) en waarschuw bij fouten.
void gewicht_sensor_setup(){ 
  Serial.begin(9600);                // Activeer seriële monitor voor debug en foutmeldingen
  delay(10);                         // Kort wachten zodat seriële poort klaar is

  float calibrationValue = 23.20;    // Kalibratiefactor: bepaalt weging in gram per raw unit
  const long newTareOffset = 8271272;// Offsetwaarde voor tare: basisnulstandinstelling
  LoadCell.setTareOffset(newTareOffset);

  LoadCell.begin();                  // Initialiseer HX711-communicatie
  unsigned long stabilizingtime = 2000; // Tijd in ms om sensorstabilisatie te voltooien
  boolean _tare = false;             // Skip automatische tarra herberekening
  LoadCell.start(stabilizingtime, _tare); // Begin met meten na stabilisatie

  if (LoadCell.getTareTimeoutFlag()) {  // Controleer of tarra niet binnen tijd startte
    Serial.println("FOUT: CONTROLEER BEKABELING");
  } else {
    LoadCell.setCalFactor(calibrationValue); // Stel kalibratiefactor in voor nauwkeurige gewichtsdata
  }

  while (!LoadCell.update());       // Wacht tot de eerste meting beschikbaar is

  // Controleer of samples per seconde (SPS) binnen verwachte range vallen
  long sps = LoadCell.getSPS();     
  if (sps < 7 || sps > 100) {
    Serial.println("FOUT: CONTROLEER BEKABELING");
  }
}

// Lees en verwerk gewichtsmetingen:
// 1. Vraag de sensor om een nieuwe meting (LoadCell.update()).
// 2. Zodra er nieuwe data beschikbaar is, wacht tot de geplande print-interval verstreken is.
// 3. Bereken en sla het gemiddelde gewicht op in de variabele 'gewicht'.
void save_gewicht_in_varibel(){
  static boolean newDataReady = false;           // Flag dat aangeeft dat er nieuwe data is
  const int serialPrintInterval = 500;           // Minimale tijd tussen seriële prints in ms

  if (LoadCell.update()) newDataReady = true;    // Update data en zet flag

  if (newDataReady) {
    // Controleer of interval verstreken is om print-throttling te voorkomen
    if (millis() > t + serialPrintInterval) {
      gewicht = LoadCell.getData();              // Lees gemeten gewicht (in gram)
      newDataReady = false;                      // Reset flag
      t = millis();                              // Herstart interval timer
    }
  }
}

// ____________________________________________________________
// MP3 FUNCTIES
// Initialiseert de MP3-speler module:
// - Start seriële communicatie op 9600 baud voor commando's
// - Roep mp3.initialize() aan om de DFPlayer klaar te zetten
void mp3_player_setup(){
  Serial.begin(9600);
  mp3.initialize();
}

// Speelt een track af op de MP3-speler:
// - Bepaalt het track-nummer en volume
// - Zet de 'isplaying' status op true zodat andere logica weet dat het geluid loopt
void play_mp3(){
  mp3.playTrackNumber(trackNumber, volume, false);
  isplaying = true;
}

// ____________________________________________________________
// KNOP FUNCTIES
// Stel de hardwareknop in met pull-up via aparte outputPin:
// 1. buttonPin als ingang om druk te detecteren
// 2. outputPin hoog zetten voor voeding naar de knop
void button_setup(){
  pinMode(buttonPin, INPUT);       // Lees knopstatus
  pinMode(outputPin, OUTPUT);      // Voed knop
  digitalWrite(outputPin, HIGH);   // Zet voeding aan zodat buttonPin HIGH wordt bij druk
}

// Debounce en lees knopstatus:
// - Voorkomt onbedoelde flutters door korte onderbrekingen
// - update buttonState na stabiele periode
void button_state_lezen(){
  int sensorReading = digitalRead(buttonPin);  // Lees fysieke knop
  if (sensorReading != lastButtonState) {
    lastDebounceTime = millis();               // Reset debounce timer bij verandering
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Pas status toe als de knop lange genoeg stabiel is
    if (sensorReading != buttonState) {
      buttonState = sensorReading;             // Werk buttonState bij
    }
  }
  lastButtonState = sensorReading;             // Sla vorige status op voor volgende loop
}

// ____________________________________________________________
// TIMER & UPDATE FUNCTIES
// Start slaaptimer (fase 1) wanneer knop wordt ingedrukt:
// - Vastleggen huidige tijd als start
// - Bereken eindtijd op basis van duration constant
void timer_one_start(){ 
  if (buttonState == HIGH) {
    startTimeTimerOne = millis();
    endTimeTimerOne = startTimeTimerOne + timerOneDuration;
    timerOneActive = true;
  }
}

// Start controletimer (fase 2) na stoppen eerste alarm:
// - Vastleggen huidige tijd
// - Eindtijd = start + timerTwoDuration
void timer_two_start(){ 
  startTimeTimerTwo = millis();
  endTimeTimerTwo = startTimeTimerTwo + timerTwoDuration;
  timerTwoActive = true;
}

// Werk alle variabelen bij in elke loop-iteratie:
// 1. Lees knopstatus met debounce
// 2. Lees en verwerk gewicht
// 3. Update huidigetijd voor timerchecks
void update(){
  button_state_lezen();          
  save_gewicht_in_varibel();     
  huidigetijd = millis();        
}
// TIMER & UPDATE FUNCTIES
// Start slaaptimer op knopdruk
void timer_one_start(){ 
  if (buttonState == HIGH) {
    startTimeTimerOne = millis();
    endTimeTimerOne = startTimeTimerOne + timerOneDuration;
    timerOneActive = true;
  }
}

// Start controletimer na stop eerste alarm
void timer_two_start(){ 
  startTimeTimerTwo = millis();
  endTimeTimerTwo = startTimeTimerTwo + timerTwoDuration;
  timerTwoActive = true;
}

// Werk knop, gewicht en tijd variabelen bij voor de volgende loop
void update(){
  button_state_lezen();
  save_gewicht_in_varibel();
  huidigetijd = millis();
}
