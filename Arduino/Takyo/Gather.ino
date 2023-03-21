
void evaluateSensors() {
  if (isRobotOn) {
    awakeConditions(); 
  } else {
    sleepCondtions();
  }    
}

void sleepCondtions() {
  if ((sensorHit[IRLFT]) && (sensorHit[SONAR]) && (sensorHit[IRRHT])) { 
    isRobotOn = true;  
    Serial.println("RBOTON");
  }
}

void awakeConditions() {  
  
  /*
  if ((sensorValue[ROLL] > 10) || (sensorValue[ROLL] < -10)) {
    moveMotors(STP); delay(STOPDELAY);  
    moveMotors(BCK); delay(BACKDELAY);delay(BACKDELAY); 
    Serial.print(" ROL ");printFlag = true;
    moveMotors(STP); delay(STOPDELAY);  
    moveMotors(STP); delay(STOPDELAY);  
    moveMotors(STP); delay(STOPDELAY);  
    moveMotors(STP); delay(STOPDELAY);  
    function111();
  }*/
  
  saveOrigHit = origHit;
  decision = "";
  decisionT = 0;
  origHit = "";
  
  if (sensorHit[TOF]) {
    moveMotors(STP); delay(STOPDELAY);  
    int tmp = (BACKDELAY / 3) * 2;
    moveMotors(BCK); delay(tmp);//delay(BACKDELAY);delay(BACKDELAY);delay(BACKDELAY);
    origHit = "TOF";
    int temp1 = TURNDELAY * 3;
    int temp2 = TURNDELAY * 4;
    turnDelay = random(temp1, temp2); 
    function010();
    turnDelay = TURNDELAY;     
  
  } else 
  if ((sensorHit[IRLFT]) && (sensorHit[SONAR]) && (sensorHit[IRRHT])) {        // 111
    origHit = "111";
    function111();
  }
  else if ((!sensorHit[IRLFT]) && (!sensorHit[SONAR]) && (sensorHit[IRRHT])) { // 001
    origHit = "001";
    function001(); 
  }
  else if ((!sensorHit[IRLFT]) && (sensorHit[SONAR]) && (!sensorHit[IRRHT])) { // 010
    origHit = "010";
    function010();
  }
  else if ((!sensorHit[IRLFT]) && (sensorHit[SONAR]) && (sensorHit[IRRHT])) {  // 011
    origHit = "011";
    function011();
  }
  else if ((sensorHit[IRLFT]) && (!sensorHit[SONAR]) && (!sensorHit[IRRHT])) { // 100
    origHit = "100";
    function100();
  }
  else if ((sensorHit[IRLFT]) && (!sensorHit[SONAR]) && (sensorHit[IRRHT])) {  // 101
    origHit = "101";
    function101();
  }
  else if ((sensorHit[IRLFT]) && (sensorHit[SONAR]) && (!sensorHit[IRRHT])) {  // 110
    origHit = "110";
    function110();
  }
  else if ((!sensorHit[IRLFT]) && (!sensorHit[SONAR]) && (!sensorHit[IRRHT])) { // 000
    //origHit = (saveOrigHit == "000") ? "" : "000"; 
    function000();
  } 

  //if (sensorHit[PITCH]) { Serial.print(" PITCH "); }
  //if (sensorHit[ROLL]) { Serial.print(" ROLL "); }
}

void function000() { 
  if (currentDirection == STP) {
    moveMotors(FWD);
    origHit = "000";
    decision = "FWD"; 
    forwardMillis = currentMillis;
  } 
  
  // wheels are spinning but we are not moving 
  // in a large room will randomly spin after 14000 millis
  //if ((currentMillis - forwardMillis) > 14000) {
  //  Serial.print(" FTOUT "); printFlag = true;
  //  ctgFlag = false;
  //  int temp1 = TURNDELAY * 2;
  //  int temp2 = TURNDELAY * 4;
  //  turnDelay = random(temp1, temp2); 
  //  function010();
  //  turnDelay = TURNDELAY;
  //}
  
  // after any sensor hit go forward 6000 millis before declaring that we are 
  // free of the object
  
  if (conditionHigh) {
    ctgFlag = false; 
    if ((currentMillis - forwardMillis) > CTGINTERVAL) { // TINTERVAL) {
      conditionHigh = false;
      ctgFlag = true;
      ctgMillis = currentMillis;
      clearToGo(true);
    }
  }  
  if (ctgFlag) {
    clearToGo(false);
  }
  
} 

void clearToGo(boolean mForce) {
   
  if ((mForce) || ((currentMillis - ctgMillis) > CTGINTERVAL)) {
    //pass
  } else {
    return;
  }
  ctgMillis = currentMillis;
  //forwardMillis = currentMillis;
  //if (printFlag) {
  //  printEOL();
  //}
  origHit = "CTG";
  //Serial.print (" CTG force="); Serial.print(mForce);Serial.print(" ");// clear to go  
   
  //Serial.print(" compass=");Serial.print(formatNumber(sensorValue[COMPSS]));

  int diff = sensorValue[COMPSS] - targetHeading;
  
  if (diff > 180) {
    diff = -360 + diff;
  } else if (diff < -180) {
    diff = 360 + diff;
  }
  
  //Serial.print(" diff=");
  //Serial.print(formatNumber(diff));
  //Serial.print(" target=");Serial.print(formatNumber(targetHeading)); 

 if ((sensorValue[COMPSS] > targetLow) && (sensorValue[COMPSS] < targetHigh)) {
    //Serial.print(" ONTARGET ");    
    //Serial.print(" target=");Serial.print(formatNumber(targetHeading));
    //Serial.print(" compass=");Serial.println(formatNumber((int)readCompass()));
    //ctgFlag = false;
    decision = "CCT";
    return;
  }
  
  //diff = map(diff, -180, 180, -255, 255);
  if (diff > 0) {
    moveMotors(STP); 
    moveMotors(LFT);delay(turnDelay);
    decision = "CLF"; 
    decisionT = turnDelay;
    moveMotors(STP);
    //Serial.print(" TURNLEFT ");
    //Serial.println(formatNumber((int)readCompass()));
  } else {
    moveMotors(STP); 
    moveMotors(RHT);delay(turnDelay);
    decision = "CRT";
    decisionT = turnDelay;
    moveMotors(STP);
    //Serial.print(" TURNRIGHT ");
    //Serial.println(formatNumber((int)readCompass()));
  }    
}

void function111() {
  moveMotors(STP); delay(STOPDELAY);
  turnDelay = random(TURNDELAY, TURNDELAY * 3);
  function010();
  turnDelay = TURNDELAY;
  delay(STOPDELAY);  delay(STOPDELAY);
}

void function001() {  
  moveMotors(STP); delay(STOPDELAY);
  conditionHigh = true;
  boolean dback = false;
  
  if ((lastLftInterval > 0) && (lastLftInterval < DINTERVAL)) {
    if (hitBuffLftFull) {
      if ((lavg > 0) && (lavg < TINTERVAL)) {
         dback = true;
      }
    }
  } 
   
  if ((lastRhtInterval > 0) && (lastRhtInterval < DINTERVAL)) {
    if (hitBuffRhtFull) {
      if ((ravg > 0) && (ravg < TINTERVAL)) {
         dback = true;
      }
    }
  } 
  
  // last two intervals very close together
  if ((lastLftInterval > 0) && (lastLftLftInterval > 0)) {
    if ((lastLftInterval < DINTERVAL) && (lastLftLftInterval < DINTERVAL)) {
      dback = true;
      int temp = random(TURNDELAY, TURNDELAY * 2);
      turnDelay = (turnDelay == TURNDELAY) ? temp : turnDelay;
    }
  }
  
  if (dback) {     
      moveMotors(BCK); delay(BACKDELAY);       
      moveMotors(STP);  
  }
  
  // increase arc do not override previous
  if ((lastRhtInterval > 0) && (lastRhtInterval < TINTERVAL)) {
    int temp = random(TURNDELAY, TURNDELAY * 2);
    turnDelay = (turnDelay == TURNDELAY) ? temp : turnDelay;  
  }

  // hit on the right
  // if last hit on the left was less than DINTERVAL millis ago
  //   continue to go right
  // otherwise 
  //   go left
  
  if ((lastLftInterval > 0) && (lastLftInterval < DINTERVAL)) {
    moveMotors(RHT); delay(turnDelay);
    decision = "0RT";
    decisionT = turnDelay;
    pushLftSensor(sensorValue[IRRHT], sensorTime[IRRHT]);
  } else {
    moveMotors(LFT); delay(turnDelay);
    decision = "0LF";
    decisionT = turnDelay;
    pushRhtSensor(sensorValue[IRRHT], sensorTime[IRRHT]);
  }

  // reset turnDelay
  turnDelay = TURNDELAY;
  
  moveMotors(STP);  
  //resetSensors();
}

void function100() {    
  moveMotors(STP); delay(STOPDELAY);
  conditionHigh = true;
  boolean dback = false;
  
  if ((lastLftInterval > 0) && (lastLftInterval < DINTERVAL)) {
    if (hitBuffLftFull) {
      if ((lavg > 0) && (lavg < TINTERVAL)) {
         dback = true;
      }
    }
  } 
   
  if ((lastRhtInterval > 0) && (lastRhtInterval < DINTERVAL)) {
    if (hitBuffRhtFull) {
      if ((ravg > 0) && (ravg < TINTERVAL)) {
         dback = true;
      }
    }
  } 
  
  // last two intervals very close together
  if ((lastRhtInterval > 0) && (lastRhtRhtInterval > 0)) {
    if ((lastRhtInterval < DINTERVAL) && (lastRhtRhtInterval < DINTERVAL)) {
      dback = true;
      int temp = random(TURNDELAY, TURNDELAY * 2);
      turnDelay = (turnDelay == TURNDELAY) ? temp : turnDelay;
    }
  }

  if (dback) {     
      moveMotors(BCK); delay(BACKDELAY);     
      moveMotors(STP);   
  }
  
  // increase arc do not override previous
  if ((lastLftInterval > 0) && (lastLftInterval < TINTERVAL)) {
    int temp = random(TURNDELAY, TURNDELAY * 2);
    turnDelay = (turnDelay == TURNDELAY) ? temp : turnDelay;       
  }

  // hit on the left
  // if last hit on the right was less than DINTERVAL millis ago
  //   continue to go left
  // otherwise 
  //   go right

  if ((lastRhtInterval > 0) && (lastRhtInterval < DINTERVAL)) {
    moveMotors(LFT); delay(turnDelay);
    decision = "1LF";
    decisionT = turnDelay;
    pushRhtSensor(sensorValue[IRLFT], sensorTime[IRLFT]);
  } else {
    moveMotors(RHT); delay(turnDelay);
    decision = "1RT";
    decisionT = turnDelay;
    pushLftSensor(sensorValue[IRLFT], sensorTime[IRLFT]);
  }
  turnDelay = TURNDELAY;
  moveMotors(STP);  
  //resetSensors();

}

void function010(){
  moveMotors(STP); delay(STOPDELAY);

  lastCtrTime = currentMillis;

  int holdLft = 10000;
  int holdRht = 10000;

  if ((lastRhtInterval > 0) && (lastRhtInterval < DINTERVAL)) {
    holdRht = lastRhtInterval;
  }

  if ((lastLftInterval > 0) && (lastLftInterval < DINTERVAL)) {
    holdLft = lastLftInterval;
  }

  if ((holdLft < DINTERVAL) || (holdRht < DINTERVAL)) {
    if (holdLft < holdRht) {
      function001();
      return; 
    } else {
      function100();
      return;
    }
  }

  if (lavg < ravg) {
    moveMotors(RHT); delay(turnDelay);
    decision = "ART";
    decisionT = turnDelay;
    pushLftSensor(sensorValue[SONAR], sensorTime[SONAR]);
  } else if (ravg < lavg) {
    moveMotors(LFT); delay(turnDelay);
    decision = "ALF";
    decisionT = turnDelay;
    pushRhtSensor(sensorValue[SONAR], sensorTime[SONAR]);
  } else {
    if (spinToggle == LFT) {
      int temp = random(TURNDELAY, TURNDELAY * 2);
      turnDelay = (turnDelay = TURNDELAY) ? temp : turnDelay;
      moveMotors(LFT); delay(turnDelay);  // spin
      decision = "SLF"; 
      decisionT = turnDelay;
      turnDelay = TURNDELAY;
      spinToggle = RHT;
      pushRhtSensor(sensorValue[SONAR], sensorTime[SONAR]);
    } else {
      int temp = random(TURNDELAY, TURNDELAY * 2);
      turnDelay = (turnDelay = TURNDELAY) ? temp : turnDelay;
      moveMotors(RHT); delay(turnDelay); // spin
      decision = "SRT";
      decisionT = turnDelay;
      turnDelay = TURNDELAY;
      spinToggle = LFT;
      pushLftSensor(sensorValue[SONAR], sensorTime[SONAR]);
    }
  }  
  moveMotors(STP); delay(STOPDELAY);
  resetSensors();
}

void function011() {
  function001();
}

void function110() {  
  function100();
}

void function101() {
  function010();
}

 // should return hitRhtTime[0]
unsigned long findLastRhtTime() {
  unsigned long maxTime = 0;
  for (uint8_t i = 0; i < HITBUFFSIZE; i++) {
    if (hitRhtTime[i] > maxTime) {
      maxTime = hitRhtTime[i];
    }
  }
  return maxTime;
}

// should return hitRhtTime[1]
unsigned long findLastLastRhtTime(unsigned long mTime) {   
  unsigned long maxTime = 0;
  for (uint8_t i = 0; i < HITBUFFSIZE; i++) {
    if (hitRhtTime[i] == mTime) {
      // skip current one
    } else {
      if (hitRhtTime[i] > maxTime) {
        maxTime = hitRhtTime[i];
      }
    }
  }
  return maxTime;
}

// should return hitLftTime[0]
unsigned long findLastLftTime() {   
  unsigned long maxTime = 0;
  for (uint8_t i = 0; i < HITBUFFSIZE; i++) {
    if (hitLftTime[i] > maxTime) {
      maxTime = hitLftTime[i];
    }
  }
  return maxTime;
}

// should return hitLftTime[1]
unsigned long findLastLastLftTime(unsigned long mTime) {   
  unsigned long maxTime = 0;
  for (uint8_t i = 0; i < HITBUFFSIZE; i++) {
    if (hitLftTime[i] == mTime) {
      // pass
    } else {
      if (hitLftTime[i] > maxTime) {
        maxTime = hitLftTime[i];
      }
    }
  }
  return maxTime;
}

void getAverageHitTimes() {
   
  rtot = 0;
  rseed = currentMillis;
  rcnt = 0;
  
  ltot = 0;
  lseed = currentMillis;
  lcnt = 0;

  // calculates intervals between hits
  for (uint8_t i = 0; i < HITBUFFSIZE; i++) {    
    if (hitRhtTime[i] > 0) {
      unsigned long ltemp = (rseed - hitRhtTime[i]);
      ltemp = ltemp > 10000 ? 10000 : ltemp;
      rtot = (rtot + ltemp);
      rseed = hitRhtTime[i];
      rcnt ++;
    }
    if (hitLftTime[i] > 0) {
      unsigned long ltemp = (lseed - hitLftTime[i]);
      ltemp = ltemp > 10000 ? 10000 : ltemp;
      ltot = (ltot + ltemp);   
      lseed = hitLftTime[i];
      lcnt ++;
    }    
  } 
  float avg = ltot / lcnt;
  lavg = (unsigned long)avg;
  avg  = rtot / rcnt;
  ravg = (unsigned long)avg;
}
