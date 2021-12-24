#include <string.h>
#include <Wire.h>

#include "audio.h"
#include "util.h"

#define CARDCS 4     // Card chip select pin (SD reader on Adafruit Music Maker shield)
const unsigned long showPause = 90000; // ms
void setup()
{
  delay(1000);
  Serial.begin(9600);
  Wire.begin();

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
mainTopic lastSpoken = story;

//bool isSpokenLast = true 
//spokenContent currShow = story;


void sendLightStateMssg(byte mssg){
  Wire.beginTransmission(4);
  Wire.write(mssg);
  Wire.endTransmission();
}


void loop()
{
  delay(showPause);
  Serial.print("Free SRAM: ");
  Serial.println(freeMemory());

  Serial.print("pre-switch: ");
  Serial.println(currShow);
  
  if (currShow == song) {
    // play something other than a song
    switch(lastSpoken)
    {
      case story:
        currShow = riddle;
        break;
      case riddle:
        currShow = joke;
        break;
      case joke:
        currShow = fact;
        break;
      case fact:
        currShow = knock;
        break;
      case knock:
        currShow = story;
        break;
      default:
        Serial.println("Default case: joke");
        currShow = joke;  
    }
    lastSpoken = currShow;
  } else {
    currShow = song;
  }
  Serial.print("post-switch: ");
  Serial.println(currShow);
  sendLightStateMssg(1);
  delay(10000);
  playMedia(currShow);
  delay(10000);
  sendLightStateMssg(0);
}
 
