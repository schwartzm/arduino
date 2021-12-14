#ifndef H_AUD
#define H_AUD

#include <SD.h>
#include <SPI.h>
#include <string.h>
#include <Arduino.h> //needed for Serial.println

#include "util.h"


const uint8_t MAX_FILEPATH_SIZE = 15; // Max length of both file path + file name + dot + file ext.
const uint8_t MAX_FILENAME_SIZE = 13; // Strict 8.3 filename 8chars+1dot+3chars+nullterm
const uint8_t MAX_FILES_PER_TOPIC = 250;
const uint8_t TOPIC_COUNT = 11; // Total count of topics (stories, jokes, intros, etc.)

struct Show {
  const char *type;   // e.g., story, song, fact, joke, knock
  const char *intro;  // intro audio
  const char *main;   // main audio
};

// Structure to hold basic Topic info
// A Topic is the type of content, such as
// joke, riddle, intro_joke, intro_riddle
struct Topic {
  const char *name;
  const char *dir;
  uint16_t count;
};

enum mainTopic {
  story,
  joke,
  riddle,
  fact,
  knock 
};

extern Topic topics[TOPIC_COUNT];

const uint8_t STORIES   = 0;
const uint8_t JOKES     = 1;
const uint8_t RIDDLES   = 2;
const uint8_t FACTS     = 3;
const uint8_t KNOCK     = 4;
const uint8_t SONGS     = 5;
const uint8_t ISTORIES  = 6;
const uint8_t IJOKES    = 7;
const uint8_t IRIDDLES  = 8;
const uint8_t IFACTS    = 9;
const uint8_t ISONGS    = 10;

void setupTopics();
void getFileAtIndex(File dir, int selectIdx, char filename[]);
void playMedia(mainTopic topic);
uint16_t getFileCount(File dir);

#endif
