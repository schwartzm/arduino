#include <string.h>

#include "audio.h"
#include "util.h"

#define CARDCS 4     // Card chip select pin (SD reader on Adafruit Music Maker shield)

void setup()
{ 
  delay(1000);
  Serial.begin(19200);
  randomSeed(micros());
  
  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  setupTopics();
}

void loop()
{
  delay(10000);
  Serial.print("Free SRAM: ");
  Serial.println(freeMemory());
  playMedia(story);
}
