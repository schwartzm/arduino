#include <SD.h>
#include <SPI.h>
#include <string.h>

#define CARDCS 4     // Card chip select pin (SD reader on Adafruit Music Maker shield)

const int MAX_FILEPATH_SIZE = 15; // Max length of both file path + file name + dot + file ext.
const int MAX_FILENAME_SIZE = 13; // Strict 8.3 filename 8chars+1dot+3chars+nullterm
const int MAX_FILES_PER_TOPIC = 250;
const int TOPIC_COUNT = 11; // Total count of topics (stories, jokes, intros, etc.)

// Structure to hold basic Topic info
// A Topic is the type of content, such as
// joke, riddle, intro_joke, intro_riddle
struct Topic {
  const char *name;
  const char *dir;
  int count;
};

Topic topics[TOPIC_COUNT];

const int STORIES   = 0;
const int JOKES     = 1;
const int RIDDLES   = 2;
const int FACTS     = 3;
const int KNOCK     = 4;
const int SONGS     = 5;
const int ISTORIES  = 6;
const int IJOKES    = 7;
const int IRIDDLES  = 8;
const int IFACTS    = 9;
const int ISONGS    = 10;

void setup()
{ 
  delay(1000);
  Serial.begin(9600);
  randomSeed(micros());
  
  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  Topic stories = {"stories", "/stories"};
  Topic jokes = {"jokes", "/jokes"};
  Topic riddles = {"riddles", "/riddles"};
  Topic facts = {"facts", "/facts"};
  Topic knock = {"knock", "/knock"};
  Topic songs = {"songs", "/songs"};
  Topic intStories = {"int_stories", "/i_story"};
  Topic intJokes = {"int_jokes", "/i_jokes"};
  Topic intRiddles = {"int_riddles", "/i_riddle"};
  Topic intFacts = {"int_facts", "/i_facts"};
  Topic intSongs = {"int_songs", "/i_songs"};
  
  topics[STORIES]   = stories;
  topics[JOKES]     = jokes;
  topics[RIDDLES]   = riddles;
  topics[FACTS]     = facts;
  topics[KNOCK]     = knock;
  topics[SONGS]     = songs;
  topics[ISTORIES]  = intStories;
  topics[IJOKES]    = intJokes;
  topics[IRIDDLES]  = intRiddles;
  topics[IFACTS]    = intFacts;
  topics[ISONGS]    = intSongs;

  // Get file count in each topic
  for(int i=0; i<TOPIC_COUNT; i++){
    Serial.print("Topic name: ");
    Serial.println(topics[i].name);
    Serial.print("Topic dir: ");
    Serial.println(topics[i].dir);
    File f = SD.open(topics[i].dir);
    Serial.print("Is dir: ");
    Serial.println(f.isDirectory());
    Serial.print("Bytes available: ");
    Serial.println(f.available());
    topics[i].count = getFileCount(f);
    f.close();
  }

  // Debug info
  for(int i=0; i<TOPIC_COUNT; i++){
    if (sizeof(topics[i].dir) == 0){
      break;      
    }
    Serial.print(topics[i].dir);
    Serial.print(": ");
    Serial.println(topics[i].count);
    Serial.println("---");
  }
}

// Plays the media for the given topic (story, joke, etc.).
void playMedia(const char * topic){

  int mainTopicIndex, introTopicIndex;
  
  if(topic == "story"){
    mainTopicIndex = STORIES;
    introTopicIndex = ISTORIES;
  }

  // Get the "main" audio file (story, joke, knock, etc.)
  Serial.println("Main...");
  randomSeed(micros());
  int mainIdx = random(0, topics[mainTopicIndex].count);
  Serial.print("Random index: ");
  Serial.println(mainIdx);
  File f1 = SD.open(topics[mainTopicIndex].dir);
  char mainFile[MAX_FILENAME_SIZE+1];
  getFileAtIndex(f1, mainIdx, mainFile);
  Serial.print("File: ");
  Serial.println(mainFile);
  f1.close();

  // Get the intro file (intro associated with main topic)
  Serial.println("Intro...");
  randomSeed(micros());
  int introIdx = random(0, topics[introTopicIndex].count);
  Serial.print("Random index: ");
  Serial.println(introIdx);
  File f2 = SD.open(topics[introTopicIndex].dir);
  char introFile[MAX_FILENAME_SIZE+1];
  getFileAtIndex(f2, introIdx, introFile);
  Serial.print("File: ");
  Serial.println(introFile);
  f2.close();
  Serial.println("-----------");
}

void loop()
{
  Serial.print("Free SRAM: ");
  Serial.println(freeMemory());
  delay(20000);
  playMedia("story");
}

// Sets filename to the file found at selectIdx. selectIdx
// should be less than the count of files in dir.
// Iterates dir contents, and when i++ matches selectIdx,
// that's the file which is returned.
void getFileAtIndex(File dir, int selectIdx, char filename[]){
  int idx = 0;
  while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       Serial.println("** No more files **");
       break;
     }
     
     if (entry.isDirectory()) {
       Serial.print(entry.name());
       Serial.println("/");
     } else {
       if (selectIdx == idx){
        strcpy(filename, entry.name());
        entry.close();
        return;
       }
       idx++;
     }
     entry.close();
  } 
}

/* Deprecated
void getFileList(File dir, int maxCount, char dirFiles[][MAX_FILEPATH_SIZE]){
  int idx = 0;
  Serial.print("maxCount: ");
  Serial.println(maxCount);

  Serial.print("Building file list for ");
  Serial.println(dir.name());

  while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       Serial.println("** No more files **");
       break;
     }
     
     if (entry.isDirectory()) {
       Serial.println("/");
     } else {
       char * n = entry.name();
       if (idx < maxCount){
         strcpy(dirFiles[idx], n);
         idx++;
       }
     }
     entry.close();
  }
}
*/

// Returns number of files in the given dir.
int getFileCount(File dir){
  Serial.print("Counting files in ");
  Serial.println(dir.name());
  int count = 0;
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       Serial.println("** No more files **");
       break;
     }

     //Serial.println(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
     } else {
       count++;
     }
     entry.close();
   }

  Serial.print("File count: ");
  Serial.println(count);
  return count;
}

/*
 * freeMemory code to get current free dynamic ram.
 * Just call freeMemory.
 * Source: https://learn.adafruit.com/memories-of-an-arduino/measuring-free-memory
 */
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}
