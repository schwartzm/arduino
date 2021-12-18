#include <string.h>

#include "audio.h"
#include "util.h"

#define CARDCS 4     // Card chip select pin (SD reader on Adafruit Music Maker shield)
const unsigned long showPause = 120000; // ms
void setup()
{
  delay(1000);
  Serial.begin(19200);
  //randomSeed(micros());

  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(1,1);  

  setupTopics();
}

//mainTopic lastShow = story; // starting default: story
mainTopic currShow = story;

void loop()
{
  delay(showPause);
  Serial.print("Free SRAM: ");
  Serial.println(freeMemory());
  
  switch(currShow)
  {
    case story:
      currShow = song;
      break;
    case song:
      currShow = joke;
      break;
    case joke:
      currShow = fact;
      break;
    case fact:
      currShow = riddle;
      break;
    case riddle:
      currShow = knock;
      break;
    case knock:
      currShow = story;
      break;      
    default:
      currShow = song;
      break;
  }
  
  playMedia(currShow); // mainTopic enum

}
