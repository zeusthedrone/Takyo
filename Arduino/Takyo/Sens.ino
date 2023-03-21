
void updateSensors() {   
  if ((currentMillis - sensorMillis) > sensorInterval) {
    // pass
  } else {
    return;
  }
  sensorMillis = currentMillis;
  getIrLft();delay(10);
  getSonar(); delay(10);
  getIrRht();
  getTof();
  
  getAverageHitTimes();
  getCompassHeading(); 
   
  lastRhtTime    = findLastRhtTime();
  lastLftTime    = findLastLftTime();
  lastLftLftTime = findLastLastLftTime(lastLftTime);  
  lastRhtRhtTime = findLastLastRhtTime(lastRhtTime);
  lastRhtInterval    = currentMillis - lastRhtTime;
  lastLftInterval    = currentMillis - lastLftTime;
  lastLftLftInterval = lastLftTime - lastLftLftTime;
  lastRhtRhtInterval = lastRhtTime - lastRhtRhtTime; 
}
//
// maxbotics sonar
//
void getSonar() { 
  if (sensorHit[SONAR]) { return; }
  tempDistance = readMaxPing();
  sensorValue[SONAR] = (int)tempDistance;
  sensorTime[SONAR] = currentMillis;
  sensorHit[SONAR] = (sensorValue[SONAR] < 16) ? true : false;
} 

uint16_t readMaxPing() {
  maxRaw = analogRead(A0);
  if (maxRaw > 100) {maxRaw = 40; }
  if (maxRaw < 10) { maxRaw = 40; }
  //ldistance = maxRaw * 5;
  
  //ldistance = pulseIn(P_MAXPING, HIGH);  
  //ldistance = ldistance / 10; // cm 
  //maxRaw = ldistance;
  //if (ldistance < 120) { ldistance = 600; }
  //if (ldistance > 600) { ldistance = 600; }
  //ldistance = ldistance / 10; // cm 
  return maxRaw;
}
 
void getIrLft() { 
  if (sensorHit[IRLFT]) { return; }
  sensorValue[IRLFT] = readIr(P_IRLFT);
  sensorTime[IRLFT] = currentMillis;
  sensorHit[IRLFT] = (sensorValue[IRLFT] < 18) ? true : false;
}

void getIrRht() {
  if (sensorHit[IRRHT]) { return; }
  sensorValue[IRRHT] = readIr(P_IRRHT);
  sensorTime[IRRHT] = currentMillis;
  sensorHit[IRRHT] = (sensorValue[IRRHT] < 18) ? true : false;
}

uint8_t readIr(uint8_t mPin) {
  int val = analogRead(mPin);
  fdistance = 187754 * pow(val, -1.51);
  fdistance = fdistance > 40 ? 40: fdistance; 
  fdistance = fdistance < 6 ? 40: fdistance;
  return (int)fdistance;
}

 
void getTof() { 
  // range to floor if between 96-400 we have a cliff  
  sensorValue[TOF] = readTof();
  sensorTime[TOF] = currentMillis;
  if ((sensorValue[TOF] > 120) && (sensorValue[TOF] < 400)) {
    sensorHit[TOF] = true;
  } else {
    sensorHit[TOF] = false;
  }
}

int readTof() {
  //ldistance = tof.readRangeSingleMillimeters();
  //if (tof.timeoutOccurred()) { 
  //  Serial.print(" TOF TIMEOUT"); 
  //  return 0;
  //}

  ldistance = tof.readRangeContinuousMillimeters();
  if (tof.timeoutOccurred()) { 
    Serial.print(" TOF TIMEOUT");
    return 100;
  }
  
  ldistance = ldistance > 400 ? 400: ldistance; 
  ldistance = ldistance < 10 ? 400: ldistance;
  return (int)ldistance;
} 

void getCompassHeading() {
  pitchRoll();
  sensorValue[COMPSS] = (int)readCompass(); 
  sensorTime[COMPSS] = currentMillis;
  sensorHit[COMPSS] = false;
}

void pitchRoll() {
   
  //float signOfZ = compass.a.z >= 0 ? 1.0F : -1.0F;
  //float t_roll = compass.a.x * compass.a.x + compass.a.z * compass.a.z;
  //float roll = (float)atan2(compass.a.y, sqrt(t_roll)) * 180 / PI;
  //float t_pitch = compass.a.y * compass.a.y + compass.a.z * compass.a.z;
  //float pitch = (float)atan2(compass.a.x, signOfZ * sqrt(t_pitch)) * 180 / PI;
 
  //sensorValue[PITCH] = (int)(pitch * 10);
  //sensorValue[ROLL] = (int)(roll * 10);
  //x range(-1400 <> -940)  y range(-81 <> 113)
  
  if (!(compass.a.x == 0)) {
    sensorValue[PITCH] = (int)(compass.a.x);
  }
  if (!(compass.a.y == 0)) {
    sensorValue[ROLL] = (int)(compass.a.y);  
  }
  if ((sensorValue[PITCH] < -3000) || (sensorValue[PITCH] > 600)) {
    sensorHit[PITCH] = true;
  }
  if ((sensorValue[ROLL] < -3000) || (sensorValue[ROLL] > 3000)) {
    sensorHit[ROLL] = true;
  }  
}

float readCompass() {
  compass.read();
   
  return compass.heading();
  
}

// push right hit
void pushRhtSensor(int mValue, unsigned long mTime) {  
  // pop one off the end  
  for (uint8_t i = HITBUFFSIZE; i >= 1; i--) {
    mhitValue[i] = hitRhtValue[i-1]; 
    mhitTime[i]  = hitRhtTime[i-1];
  }
  
  // push one on the beginning
  mhitValue[0] = mValue;
  mhitTime[0]  = mTime;  

  // copy temp marray back to original
  for (uint8_t i = 0; i < HITBUFFSIZE; i++) {
    hitRhtValue[i] = mhitValue[i];
    hitRhtTime[i] = mhitTime[i];  
  }

  if (!hitBuffRhtFull) {
    hitBuffRhtCnt ++;
  }
  if (hitBuffRhtCnt > HITBUFFSIZE) {
    hitBuffRhtFull = true;
  }
  if ((hitBuffRhtFull) && (hitBuffLftFull)) {
      hitBuffsFull = true;
  }  
}

// push left hit
void pushLftSensor(int mValue, unsigned long mTime) {  
  // pop one off the end  
   
  for (uint8_t i = HITBUFFSIZE; i >= 1; i--) {
    mhitValue[i] = hitLftValue[i-1]; 
    mhitTime[i]  = hitLftTime[i-1];
  }
  
  // push one on the beginning
  mhitValue[0] = mValue;
  mhitTime[0]  = mTime;  

  for (uint8_t i = 0; i < HITBUFFSIZE; i++) {
    hitLftValue[i] = mhitValue[i];
    hitLftTime[i]  = mhitTime[i];  
  }
    
  if (!hitBuffLftFull) {
    hitBuffLftCnt ++;
  }
  if (hitBuffLftCnt > HITBUFFSIZE) {
    hitBuffLftFull = true;
  }
  if (hitBuffLftFull) {
    if (hitBuffRhtFull) {
      hitBuffsFull = true;
    }
  }    
}

void resetHitBuff() {
  for (uint8_t i = 0; i < HITBUFFSIZE; i++) {
    hitRhtValue[i] = 0;
    hitRhtTime[i] = 0;
    hitLftValue[i] = 0;
    hitLftTime[i] = 0;
  }
  hitBuffLftFull = false;
  hitBuffLftCnt  = 0;
  hitBuffRhtFull = false;
  hitBuffRhtCnt  = 0;
  hitBuffsFull = false;
}

void resetSensors() {
  
  //sensorMillis = currentMillis;
  maxRaw = 0;
  sensorValue[SONAR] = 40;
  sensorHit[SONAR] = false;
  
  sensorValue[IRLFT] = 40;
  sensorHit[IRLFT] = false;
  
  sensorValue[IRRHT] = 40;
  sensorHit[IRRHT] = false;
  
  sensorHit[PITCH] = false;
  sensorHit[ROLL] = false;
}

 
