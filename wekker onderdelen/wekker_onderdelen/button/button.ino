const int buttonPin = 2;      // Pin waarop de knop is aangesloten
const int outputPin = 12;     // Pin die hoog wordt gehouden

int buttonState = LOW;        // Variabele om de huidige status van de knop te lezen
int lastButtonState = LOW;    // Variabele om de vorige knopstatus op te slaan

unsigned long lastDebounceTime = 0; // Tijdstip van de laatste knopstatusverandering
const unsigned long debounceDelay = 50; // Debounce tijd in milliseconden

void setup() {
  pinMode(buttonPin, INPUT);       // Pin 2 als input
  pinMode(outputPin, OUTPUT);      // Pin 12 als output
  digitalWrite(outputPin, HIGH);   // Houd pin 12 continu hoog
  Serial.begin(9600);              // Start seriële monitor voor debuggen
}

void loop() {
  // Lees de huidige status van de knop
  int reading = digitalRead(buttonPin);

  // Controleer of de knopstatus veranderd is
  if (reading != lastButtonState) {
    lastDebounceTime = millis(); // Tijdstip van verandering opslaan
  }

  // Pas de status toe na de debounce delay
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Controleer of de status echt is veranderd
    if (reading != buttonState) {
      buttonState = reading;

      // Print knopstatus naar seriële monitor (optioneel)
      Serial.print("Button State: ");
      Serial.println(buttonState);
    }
  }

  // Sla de huidige knopstatus op voor de volgende loop
  lastButtonState = reading;
}
