#include <Wire.h>
#include <LSM303.h>
#include <VL53L0X.h>

// arduino rfnano new bootloader NRF24L01

LSM303 compass;
VL53L0X tof;

int targetHeading = 107;
int targetLow = targetHeading - 20;
int targetHigh = targetHeading + 20;

// compass north 270
// compass east 0 /360
// compass south 90
// compass west 180
#define P_RX0     0 // serial
#define P_TX0     1 // serial
#define P_2       2 
#define P_AIN1    3  
#define P_TRESET  4  
#define P_AIN2    5
#define P_BIN1    6
#define P_7       7
#define P_8       8
#define P_9       9
#define P_10     10 
#define P_BIN2   11
#define P_12     12 
#define P_13     13

#define P_MAXPING A0
#define P_LLFT    A1
#define P_IRLFT   A2
#define P_IRRHT   A3 
#define P_SDA     A4
#define P_SCL     A5
#define P_LCTR    A6
#define P_LRHT    A7 

// intervals
#define STOPDELAY 80
const int TURNDELAY = 370;
int turnDelay = TURNDELAY;
#define BACKDELAY 400 
#define DINTERVAL 3600
#define TINTERVAL 4200
#define CTGINTERVAL 5000
#define sensorInterval 90

// directions
#define STP     1 // ruki direction stop
#define FWD     2 // ruki forward direction
#define BCK     3 //  
#define LFT     4
#define RHT     5
#define CTR     6

// sensors
#define SBUFFSIZE 8
int           sensorValue[SBUFFSIZE];
unsigned long sensorTime[SBUFFSIZE];
boolean       sensorHit[SBUFFSIZE];

// short term memory
#define HITBUFFSIZE 20 
int           hitRhtValue[HITBUFFSIZE];
unsigned long hitRhtTime[HITBUFFSIZE];
int           hitLftValue[HITBUFFSIZE];
unsigned long hitLftTime[HITBUFFSIZE];

// temp buffer
uint8_t       mhitIdx[HITBUFFSIZE];
int           mhitValue[HITBUFFSIZE];
unsigned long mhitTime[HITBUFFSIZE];

// buffer helpers
boolean hitBuffLftFull = false;
uint8_t hitBuffLftCnt  = 0;
boolean hitBuffRhtFull = false;
uint8_t hitBuffRhtCnt  = 0;
boolean hitBuffsFull = false;
uint8_t spinToggle = LFT;

unsigned long lastRhtInterval;    
unsigned long lastLftInterval;    
unsigned long lastLftLftInterval; 
unsigned long lastRhtRhtInterval;
unsigned long lastRhtTime;
unsigned long lastLftTime;
unsigned long lastLftLftTime;
unsigned long lastRhtRhtTime;

// array index used in sensorValue(mIdx)  
#define IRLFT  1
#define SONAR  2
#define IRRHT  3
#define COMPSS 4
#define PITCH  5
#define ROLL   6
#define TOF    7
float fdistance;
//int sensorValue;
long ldistance;
long maxRaw = 0;
long tempDistance;

// leds
#define heartbeatInterval 200
int ledtoggle = false;
boolean heartbeatToggle = false;

// timers
unsigned long currentMillis,
              sensorMillis, 
              forwardMillis,
              heartbeatMillis,
              lastCtrTime;

// globals
boolean motorsOn = true;
uint8_t currentDirection = STP;
boolean conditionHigh = true;
boolean printFlag = false;
boolean ctgFlag = false;
unsigned long ctgMillis;
String origHit = "";
String saveOrigHit = "";
String decision = "";
int decisionT = 0;
boolean printHeader = true;

// averaging variables 
float rtot = 0;
float rseed;
int rcnt=0;
unsigned long ravg;

float ltot = 0;
float lseed;
int lcnt=0;
unsigned long lavg;

int mLft;
int mRht;
int mLftAvg;
int mRhtAvg;


// I2C assignments
int I2C_GYRO       = 106; //I2C address of the L3GD20
int I2C_SOUND      =   8; //I2C address of Sound Card

boolean isRobotOn = false;
 

void setup(){
  Wire.begin();
  Serial.begin(115200);
  
  pinMode(P_LLFT, OUTPUT);

  pinMode(P_LCTR, OUTPUT);
  pinMode(P_LRHT, OUTPUT);
  pinMode(P_13, OUTPUT);
  pinMode(P_TRESET, OUTPUT);
  
  digitalWrite(P_LLFT, HIGH);
  digitalWrite(P_LCTR, HIGH);
  digitalWrite(P_LRHT, HIGH);  
  digitalWrite(P_13, LOW);
  digitalWrite(P_TRESET, LOW);
  delay(2);
  digitalWrite(P_TRESET, HIGH);
   
  delay(2);
  
  pinMode(P_AIN1, OUTPUT);
  pinMode(P_AIN2, OUTPUT);
  pinMode(P_BIN1, OUTPUT);
  pinMode(P_BIN2, OUTPUT);
  moveMotors(STP);
  
  compass.init();
  compass.enableDefault();
  
  /*
  Calibration values; the default values of +/-32767 for each axis
  lead to an assumed magnetometer bias of 0. Use the Calibrate example
  program to determine appropriate values for your particular unit.
  */
  //compass.m_min = (LSM303::vector<int16_t>){-295, -900, -401};
  //compass.m_max = (LSM303::vector<int16_t>){+468, -188, +335};
  compass.m_min = (LSM303::vector<int16_t>){-509, -1084, -683};
  compass.m_max = (LSM303::vector<int16_t>){662, 20, 543};

  //Serial.println("Nready");
  tof.setTimeout(500);
  if (!tof.init()) {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }
  tof.startContinuous();
 
  delay(8000);
  
  currentMillis = millis();
  sensorMillis = currentMillis;
  forwardMillis = currentMillis;
  heartbeatMillis = currentMillis;

  resetSensors();

  hardFlashAll(6);
  
  //Serial.println();
  //Serial.println("READY");   
}


void loop(){ 
  currentMillis = millis();
  
  updateSensors();
  evaluateSensors();
  printDataLine();
  updateHeartbeat();
  resetSensors();
  
  digitalWrite(P_LLFT, HIGH);
  digitalWrite(P_LCTR, HIGH);
  digitalWrite(P_LRHT, HIGH);  
  
  delay(1);
}

void hardFlashAll(int cnt) {
  for (int i = 0; i < cnt; i++) {
    digitalWrite(P_LLFT, LOW);
    digitalWrite(P_LCTR, LOW);
    digitalWrite(P_LRHT, LOW); 
    delay(200);
    digitalWrite(P_LLFT, HIGH);
    digitalWrite(P_LCTR, HIGH);
    digitalWrite(P_LRHT, HIGH);
    delay(200);
  }
} 
void testForward() {
  moveMotors(FWD); delay(5000);
  moveMotors(STP);delay(5000);
}

void testLEDs() {
  ledtoggle = (ledtoggle) ? false : true;
  if (ledtoggle) {
    digitalWrite(P_LCTR, HIGH);
    digitalWrite(P_LLFT, HIGH);
    digitalWrite(P_LRHT, HIGH);
  } else {
    digitalWrite(P_LCTR, LOW);
    digitalWrite(P_LLFT, LOW);
    digitalWrite(P_LRHT, LOW);
  }
  delay(150);

}
