/*
  This program blinks pin 13 of the Arduino (the
  built-in LED)
*/

// 8 channel Relay module (map module relay to uno digital pins 12 to 5)
const int D1 = 12;
const int D2 = 11; // flood
const int D3 = 10;
const int D4 = 9;
const int D5 = 8; // flood
const int D6 = 7;
const int D7 = 6; // flood
const int D8 = 5;

const int all[] = {D1, D2, D3, D4, D5, D6, D7, D8};
#define NUM_ALL (sizeof(all)/sizeof(char *))

/* Event timings */
const unsigned long eventTime_D1_A = 70;
const unsigned long eventTime_D2_A = 100;
const unsigned long eventTime_D3_A = 100;
const unsigned long eventTime_D4_A = 100;
const unsigned long eventTime_D5_A = 100;
const unsigned long eventTime_D6_A = 100;
const unsigned long eventTime_D7_A = 100;
const unsigned long eventTime_D8_A = 100;

const unsigned long eventTime_D1_B = 50;
const unsigned long eventTime_D2_B = 100;
const unsigned long eventTime_D3_B = 100;
const unsigned long eventTime_D4_B = 100;
const unsigned long eventTime_D5_B = 100;
const unsigned long eventTime_D6_B = 100;
const unsigned long eventTime_D7_B = 100;
const unsigned long eventTime_D8_B = 100;


unsigned long previousTime_D1_A = 0;
unsigned long previousTime_D2_A = 0;
unsigned long previousTime_D3_A = 0;
unsigned long previousTime_D4_A = 0;
unsigned long previousTime_D5_A = 0;
unsigned long previousTime_D6_A = 0;
unsigned long previousTime_D7_A = 0;
unsigned long previousTime_D8_A = 0;

unsigned long previousTime_D1_B = 0;
unsigned long previousTime_D2_B = 0;
unsigned long previousTime_D3_B = 0;
unsigned long previousTime_D4_B = 0;
unsigned long previousTime_D5_B = 0;
unsigned long previousTime_D6_B = 0;
unsigned long previousTime_D7_B = 0;
unsigned long previousTime_D8_B = 0;

unsigned long currentTime = millis();

/* Last PIN Write */
int lastPinWrite_D1 = -1;

void setup()
{
  Serial.begin(9600);
  for(int i = 0; i < sizeof(all); i++ ) {
    pinMode(all[i], OUTPUT);
    digitalWrite(all[i], LOW); // default to LOW / OFF
  }
}

void loop()
{
  currentTime = millis();
  
  /* Event D1_A: TURN ON */
  if (currentTime - previousTime_D1_A >= eventTime_D1_A){
    // do stuff here
    Serial.println(lastPinWrite_D1);
    if (lastPinWrite_D1 == HIGH){
      digitalWrite(D1, LOW);
      lastPinWrite_D1 = LOW;
    }else{
      digitalWrite(D1, HIGH);
      lastPinWrite_D1 = HIGH;
    }
    
    // reset prev time
    previousTime_D1_A = currentTime;
  }

/*
  if (currentTime - previousTime_D1_B >= eventTime_D1_B){
    // do stuff here
    digitalWrite(D1, LOW);
    previousTime_D1_B = currentTime;
  }
  */
  
/*
  if (currentTime - previousTime_D2_A >= eventTime_D2_A){
    // do stuff here
    previousTime_D2_A = currentTime;
  }
  

  if (currentTime - previousTime_D2_B >= eventTime_D2_B){
    // do stuff here
    previousTime_D2_B = previousTime_D2_A;
  }  
  */
}

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

void flipAllPins(int HILOW){
  for(int i = 0; i < NUM_ALL; i++ ) {
    digitalWrite(all[i], HILOW); // default to LOW / OFF
  }
}

/* blink causes an individual light (flood or strand) to blink
 * for count times.
 */
void blink(int pin, unsigned long onDur, unsigned long offDur, int count) {
  
  
}
