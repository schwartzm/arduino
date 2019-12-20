/*
  This program blinks pin 13 of the Arduino (the
  built-in LED)
*/

enum light{flood, strand};

// 8 channel Relay module (map module relay to uno digital pins 12 to 5)
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

/* Possible light states (e.g., UNSET, HIGH, LOW) */
const int UNSET = -1;

void setup()
{
  Serial.begin(9600);
  for(int i = 0; i < sizeof(all); i++ ) {
    pinMode(all[i], OUTPUT);
    digitalWrite(all[i], LOW); // default to LOW / OFF
  }
  
dp[0].type = flood;
dp[0].pin = D1;
dp[0].onDuration = 40;
dp[0].offDuration = 2000;
dp[0].prevTime = 0;
dp[0].currState = UNSET;

dp[1].type = flood;
dp[1].pin = D2;
dp[1].onDuration = 1000;
dp[1].offDuration = 1000;
dp[1].prevTime = 0;
dp[1].currState = UNSET;

dp[2].type = flood;
dp[2].pin = D3;
dp[2].onDuration = 100;
dp[2].offDuration = 900;
dp[2].prevTime = 0;
dp[2].currState = UNSET;

dp[3].type = strand;
dp[3].pin = D4;
dp[3].onDuration = 500;
dp[3].offDuration = 875;
dp[3].prevTime = 0;
dp[3].currState = UNSET;

dp[4].type = strand;
dp[4].pin = D5;
dp[4].onDuration = 1000;
dp[4].offDuration = 2000;
dp[4].prevTime = 0;
dp[4].currState = UNSET;

dp[5].type = strand;
dp[5].pin = D6;
dp[5].onDuration = 900;
dp[5].offDuration = 3000;
dp[5].prevTime = 0;
dp[5].currState = UNSET;

dp[6].type = strand;
dp[6].pin = D7;
dp[6].onDuration = 200;
dp[6].offDuration = 800;
dp[6].prevTime = 0;
dp[6].currState = UNSET;

dp[7].type = strand;
dp[7].pin = D8;
dp[7].onDuration = 88;
dp[7].offDuration = 1000;
dp[7].prevTime = 0;
dp[7].currState = UNSET;

}

unsigned long showDuration = 20000;
unsigned long intermissionDuration = 20000;
unsigned long prevShowStartTime = 0;
unsigned long currentTime = 0;
void loop()
{
  currentTime = millis();
  if (currentTime - prevShowStartTime >= showDuration){
    setPinsHigh(); // TODO: Debug why HIGH turns off, rather than LOW.
    delay(intermissionDuration);
    prevShowStartTime = millis();
  }
  // TODO: Maybe randomize this order.
  // Roll own array shuffler since STL not great in Arduino (per Internets).
  handleState(dp[0]);
  handleState(dp[1]);
  handleState(dp[2]);
  handleState(dp[3]);
  handleState(dp[4]);
  handleState(dp[5]);
  handleState(dp[6]);
  handleState(dp[7]);
}

/*
 * Sequentially turns on/off each pin (light), the starts over.
 * Uses delay, so only good as it's own show.
 */
void commonCascade(unsigned long onDur, unsigned long offDur) {
  do {
    for (int i=0; i < NUM_ALL; i++ ){
      digitalWrite(all[i], HIGH);
      delay(onDur);
      digitalWrite(all[i], LOW);
      delay(offDur);
    }
  } while (true);
}

/* Easily set all PINS to LOW or HIGH.
 *  TODO: Debug why passing in HIGH turns lights off, rather than passing LOW.
 */
void flipAllPins(int HILOW){
  for(int i = 0; i < NUM_ALL; i++ ) {
    digitalWrite(all[i], HILOW);
  }
}

/* Set all pins to LOW (OFF).
 *  TODO: Define proper interface.
 */
void setPinsLow(){
  for(int i = 0; i < NUM_ALL; i++ ) {
    digitalWrite(all[i], LOW);
  }
}

/* Set all pins to HIGH (ON).
 *  TODO: Define proper interface.
 */
void setPinsHigh(){
  for(int i = 0; i < NUM_ALL; i++ ) {
    digitalWrite(all[i], HIGH);
  }
}

/* Handle state change of each pin (light) structure. */
void handleState(event& e){
  unsigned long currentTime = millis();
  
  // OFF to ON
  if((e.currState == LOW || e.currState == UNSET) &&
    currentTime - e.prevTime >= e.onDuration){
    digitalWrite(e.pin, HIGH);
    e.currState = HIGH;
    e.prevTime = millis();
  }
  
  // ON to OFF
  if(e.currState == HIGH &&
    currentTime - e.prevTime >= e.offDuration){
    digitalWrite(e.pin, LOW);
    e.currState = LOW;
    e.prevTime = millis();
  }  
}
