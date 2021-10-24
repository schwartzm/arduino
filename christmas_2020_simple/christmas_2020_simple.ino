
// Overall length of the blinking and bursting
unsigned long showDuration = 3000;

// Duration of Intermission between shows
unsigned long intermissionDuration = 10000;

// Elegoo 8 channel Relay module (relay to uno digital pins 12 to 5)
const int D1 = 12;
const int D2 = 11;
const int D3 = 10;
const int D4 = 9;
const int D5 = 8;
const int D6 = 7;
const int D7 = 6;
const int D8 = 5;

const int ALL_PINS_COUNT = 8;
int all_pins[ALL_PINS_COUNT] = {D1, D2, D3, D4, D5, D6, D7, D8};

const int GENTLE_SIZE = 3;
int gentle[GENTLE_SIZE] = {D1, D2, D3};

const int BURST_SIZE = 5;
int burst[BURST_SIZE] = {D4, D5, D6, D7, D8};

const int UNSET = -1;

struct Event
{
  unsigned long onDurationMs;
  unsigned long offDurationMs;
  unsigned long prevTime;
  int currState;
  int pins[];
} events[2];

struct Show
{
  enum ShowType { constant, intermittent }; // constant means no intermission; constantly running show.
  ShowType showType;
  bool isRunning;
  unsigned long runDuration;
  unsigned long runStart;
  unsigned long runEnd;
  unsigned long intermissionDuration;
  unsigned long intermissionStart;
  unsigned long intermissionEnd;
  unsigned long prevIntermissionEnd;
  Event events[]; // A show can have 1+ events
};

Show showA; // intermittent burst ... the flood/spot lights, plus some christmas lights
Show showB; // constant slow ... the sign

void setup() {
  Serial.begin(9600);

  // Set up the show
  showA.showType = Show::ShowType::intermittent;
  showA.isRunning = false;
  showA.runDuration = 5000;
  showA.runStart = 0;
  showA.runEnd = 0;
  showA.intermissionDuration = 10000;
  showA.intermissionStart = 0;
  showA.intermissionEnd = 0;
  showA.prevIntermissionEnd = 0;

  // Set up burst light event. Only 1 event because all will be on/off at same time, in (near) unison.
  showA.events[0].onDurationMs = 2000;
  showA.events[0].offDurationMs = 1000;
  showA.events[0].prevTime = 0;
  showA.events[0].currState = UNSET;
  
  for (int i = 0; i < BURST_SIZE; i++){
    showA.events[0].pins[i] = burst[i];
  }
  
/*
  // Set up gentle lights
  events[0].onDurationMs = 2000;
  events[0].offDurationMs = 1000;
  events[0].prevTime = 0;
  events[0].currState = UNSET;

  for (int i = 0; i < GENTLE_SIZE; i++){
    events[0].pins[i] = gentle[i];
  }

*/

  
}

unsigned long prevShowStartTime = 0;
unsigned long prevShowEndTime = 0;
unsigned long currentTime = 0;

/*
 *   showstarttime....showendtime,interstarttime...interendtime
 * 
 */

void loop() {
  // put your main code here, to run repeatedly:
  currentTime = millis();

  if ( (showA.isRunning && (currentTime - showA.runStart < showA.runDuration)) || (!showA.isRunning && (currentTime - showA.intermissionStart < showA.intermissionDuration))){
    // Start show or keep running
    showA.isRunning = true;
    Serial.println("Running");
    
  } else {
    Serial.println("Intermission");
    showA.isRunning = false;
    
  }

  
}

/* Easily set all PINS to LOW or HIGH. */
void flipAllPins(int value) {
  for (int i = 0; i < ALL_PINS_COUNT; i++ ) {
    digitalWrite(all_pins[i], value);
    //all[i].currState = value;
  }
}
