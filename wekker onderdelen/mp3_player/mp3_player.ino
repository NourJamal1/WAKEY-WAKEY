#include "mp3tf16p.h"

MP3Player mp3(10,11);

void setup() {
  Serial.begin(9600);
  mp3.initialize();
  mp3.playTrackNumber(3, 1);
}

void loop() {
  mp3.serialPrintStatus(MP3_ALL_MESSAGE);

}
