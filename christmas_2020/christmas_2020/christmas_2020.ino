/*
 * Christmas_2020 Arduino-controlled light show by Michael Schwartz.
 * The light show almost guaranteed to cause an aneurysm. I give 
 * you permission to use it for purposes other than Christmas light 
 * shows. Use at your own risk. Don't fry your relays!
 */

/* Duration of lights on (the "show"!) */
unsigned long showDuration = 3000;

/* Duration of Intermission between shows. */
unsigned long intermissionDuration = 10000;

/* Various light types to be aware of. */
enum light {flood, strand};

const unsigned long ONE_SEC = 1000;

// Elegoo 8 channel Relay module (relay to uno digital pins 12 to 5)
const int D1 = 12; // flood
const int D2 = 11; // flood
const int D3 = 10; // flood
const int D4 = 9;
const int D5 = 8;
const int D6 = 7;
const int D7 = 6;
const int D8 = 5;

// Used by the commonCascade. Might deprecate.
const int all[] = {D1, D2, D3, D4, D5, D6, D7, D8};
#define NUM_ALL (sizeof(all)/sizeof(char *))

struct event
{
  enum light type;
  int pin;
  unsigned long onDuration;
  unsigned long offDuration;
  unsigned long prevTime;
  int currState;
} dp[8];

#define NUM_EVENT_STRUCTS (sizeof(dp)/sizeof(dp[0]))

/* Possible light state of UNSET (in addition to HIGH, LOW) */
const int UNSET = -1;

void setup()
{
  Serial.begin(9600);

  dp[0].type = flood;
  dp[0].pin = D1;
  dp[0].onDuration = ONE_SEC;
  dp[0].offDuration = ONE_SEC;
  dp[0].prevTime = 0;
  dp[0].currState = UNSET;

  dp[1].type = flood;
  dp[1].pin = D2;
  dp[1].onDuration = ONE_SEC; //250;
  dp[1].offDuration = ONE_SEC; // 1000;
  dp[1].prevTime = 0;
  dp[1].currState = UNSET;

  dp[2].type = flood;
  dp[2].pin = D3;
  dp[2].onDuration = ONE_SEC; //400;
  dp[2].offDuration = ONE_SEC; //1500;
  dp[2].prevTime = 0;
  dp[2].currState = UNSET;

  dp[3].type = strand;
  dp[3].pin = D4;
  dp[3].onDuration = ONE_SEC; //400;
  dp[3].offDuration = ONE_SEC; //875;
  dp[3].prevTime = 0;
  dp[3].currState = UNSET;

  dp[4].type = strand;
  dp[4].pin = D5;
  dp[4].onDuration = ONE_SEC; //50;
  dp[4].offDuration = ONE_SEC; //1500;
  dp[4].prevTime = 0;
  dp[4].currState = UNSET;

  dp[5].type = strand;
  dp[5].pin = D6;
  dp[5].onDuration = ONE_SEC; //300;
  dp[5].offDuration = ONE_SEC; //1200;
  dp[5].prevTime = 0;
  dp[5].currState = UNSET;

  dp[6].type = strand;
  dp[6].pin = D7;
  dp[6].onDuration = ONE_SEC; //225;
  dp[6].offDuration = ONE_SEC; //700;
  dp[6].prevTime = 0;
  dp[6].currState = UNSET;

  dp[7].type = strand;
  dp[7].pin = D8;
  dp[7].onDuration = ONE_SEC; //125;
  dp[7].offDuration = ONE_SEC; //400;
  dp[7].prevTime = 0;
  dp[7].currState = UNSET;

  for (int i = 0; i < sizeof(all); i++ ) {
    pinMode(all[i], OUTPUT);
  }  
  flipAllPins(LOW);

  randomSeed(analogRead(0));
}

unsigned long prevShowStartTime = 0;
unsigned long currentTime = 0;
void loop()
{
  currentTime = millis();
  if (currentTime - prevShowStartTime >= showDuration) {
    flipAllPins(HIGH);
    randomizeEventTimings(dp, NUM_EVENT_STRUCTS);
    delay(intermissionDuration);
    prevShowStartTime = millis();
  }
  // TODO: Maybe randomize this order.
  // Roll own array shuffler since STL not great in Arduino (per Internets).

    handleState(dp[3]);
    handleState(dp[0]);
    handleState(dp[7]);    
    handleState(dp[1]);
    handleState(dp[4]);
    handleState(dp[5]);
    handleState(dp[2]);
    handleState(dp[6]);
    handleState(dp[7]);
}

/*
   Sequentially turns on/off each pin (light), the starts over.
   Uses delay, so only good as it's own show.
*/
void commonCascade(unsigned long onDur, unsigned long offDur) {
  do {
    for (int i = 0; i < NUM_ALL; i++ ) {
      digitalWrite(all[i], HIGH);
      delay(onDur);
      digitalWrite(all[i], LOW);
      delay(offDur);
    }
  } while (true);
}

/* Easily set all PINS to LOW or HIGH. */
void flipAllPins(int value) {
  for (int i = 0; i < NUM_EVENT_STRUCTS; i++ ) {
    digitalWrite(dp[i].pin, value);
    dp[i].currState = value;
  }
}

/* Set all pins to LOW (OFF).
    TODO: Define proper interface sans globals.
*/
void setPinsLow() {
  for (int i = 0; i < NUM_EVENT_STRUCTS; i++ ) {
    digitalWrite(dp[i].pin, LOW);
    dp[i].currState = LOW;
  }
}

/* Reset event ON/OFF durations to random values,
 *  recognizing strand vs floods (they should be different).
 */
void randomizeEventTimings(event arr[], int size){
  Serial.println(">>> START RANDOMIZE");
  for (int i=0; i<size; i++){
    if (arr[i].type == flood) {
      arr[i].onDuration = random(25, 200);
      arr[i].offDuration = random(250, 750);
    } else if  (arr[i].type == strand) {
      arr[i].onDuration = random(25, 200);
      arr[i].offDuration = random(250, 750);            
    }
    Serial.println(arr[i].type);
    Serial.println(arr[i].onDuration);
    Serial.println(arr[i].offDuration);
  }  
  Serial.println(">>> EXIT RANDOMIZE");  
}

/* Set all pins to HIGH (ON).
    TODO: Define proper interface sans globals.
*/
void setPinsHigh() {
  for (int i = 0; i < NUM_EVENT_STRUCTS; i++ ) {
    digitalWrite(dp[i].pin, HIGH);
    dp[i].currState = HIGH;
  }
}

/* Handle state change of each pin (light) structure. */
void handleState(event& e) {
  unsigned long currentTime = millis();

  // OFF to ON
  if ((e.currState == LOW || e.currState == UNSET) &&
      currentTime - e.prevTime >= e.onDuration) {
    digitalWrite(e.pin, HIGH);
    e.currState = HIGH;
    e.prevTime = millis();
  }

  // ON to OFF
  if (e.currState == HIGH &&
      currentTime - e.prevTime >= e.offDuration) {
    digitalWrite(e.pin, LOW);
    e.currState = LOW;
    e.prevTime = millis();
  }
}
