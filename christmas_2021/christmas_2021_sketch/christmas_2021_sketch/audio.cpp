#include "audio.h"

Topic topics[TOPIC_COUNT] = {};
  
void setupTopics(){
  
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
  for(uint8_t i=0; i<TOPIC_COUNT; i++){
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
}


// Sets filename to the file found at selectIdx. selectIdx
// should be less than the count of files in dir.
// Iterates dir contents, and when i++ matches selectIdx,
// that's the file which is returned.
void getFileAtIndex(File dir, int selectIdx, char filename[]){
  uint16_t idx = 0;
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


// Plays the media for the given topic (story, joke, etc.).
//void playMedia(const char * topic){
void playMedia(mainTopic topic){

  int mainTopicIndex, introTopicIndex;
  
  //if(topic == "story"){
  if(topic == story){
    mainTopicIndex = STORIES;
    introTopicIndex = ISTORIES;
  }

  // Get the "main" audio file (story, joke, knock, etc.)
  Serial.println("Main...");
  randomSeed(micros());
  uint16_t mainIdx = random(0, topics[mainTopicIndex].count);
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
  uint16_t introIdx = random(0, topics[introTopicIndex].count);
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

// Returns number of files in the given dir.
uint16_t getFileCount(File dir){
  Serial.print("Counting files in ");
  Serial.println(dir.name());
  uint16_t count = 0;
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
