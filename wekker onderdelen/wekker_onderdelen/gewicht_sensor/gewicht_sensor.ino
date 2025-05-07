/*
   -------------------------------------------------------------------------------------
   HX711_ADC
   Arduino library for HX711 24-Bit Analog-to-Digital Converter for Weight Scales
   Olav Kallhovd sept2017
   -------------------------------------------------------------------------------------
*/

/*
   Settling time (number of samples) and data filtering can be adjusted in the config.h file
   For calibration and storing the calibration value in eeprom, see example file "Calibration.ino"

   The update() function checks for new data and starts the next conversion. In order to acheive maximum effective
   sample rate, update() should be called at least as often as the HX711 sample rate; >10Hz@10SPS, >80Hz@80SPS.
   If you have other time consuming code running (i.e. a graphical LCD), consider calling update() from an interrupt routine,
   see example file "Read_1x_load_cell_interrupt_driven.ino".

   This is an example sketch on how to use this library
*/

#include <HX711_ADC.h>

//pins:
const int HX711_dout = 6; //mcu > HX711 dout pin
const int HX711_sck = 7; //mcu > HX711 sck pin

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_calVal_eepromAdress = 0;
unsigned long t = 0;
float gewicht = 0; // global declaration for gewicht




void setup() {
  Serial.begin(57600); delay(10);
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
    //Serial.println("Startup is complete");
  }
  while (!LoadCell.update());
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




void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 500; //increase value to slow down serial print activity
  float gewicht = 0; // global declaration for gewicht
  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      gewicht = LoadCell.getData();
      Serial.print("Load_cell output val: ");
      Serial.println(gewicht);
      //Serial.println(LoadCell.getTareOffset());
      newDataReady = 0;
      t = millis();
    }
  }


}
