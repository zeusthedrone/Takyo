
void printEOL() {
  //int comp = (int)readCompass();
  //Serial.print(" ");Serial.print(comp);Serial.print(" ");
  //Serial.println();
  printFlag = false;  
}

void printDataLine() {
  if (!isRobotOn) {
    return;
  }
  if (origHit == "") {
    return;
  }
  // orighit, compass, pitch, roll, tof, fl, fm, fr, lastlftInterval, lastrhtInterval, lftavg, rhtavg, turndelay, dirctiondecision
  //String header = "ORG COMPSS  PITCH   ROLL  CLIFF    LFT    CTR    RHT  LASTLFT  LASTRHT LFTAVG RHTAVG  DTURN   DDIR";
  String header = "ORG,COMPSS,PITCH,ROLL,CLIFF,LFT,CTR,RHT,LASTLFT,LASTRHT,LFTAVG,RHTAVG,DTURN,DDIR";
  if (printHeader) {
    Serial.println(header);
  }
  printHeader = false;
  //Serial.print(origHit);Serial.print(" ");
  //Serial.print(formatNumber(sensorValue[COMPSS]));
  //Serial.print(formatNumber(sensorValue[PITCH]));
  //Serial.print(formatNumber(sensorValue[ROLL]));
  //Serial.print(formatNumber(sensorValue[TOF]));
  //Serial.print(formatNumber(sensorValue[IRLFT]));
  //Serial.print(formatNumber(maxRaw));
  //Serial.print(formatNumber(sensorValue[IRRHT]));
  //Serial.print(formatSTime(lastLftInterval));
  //Serial.print(formatSTime(lastRhtInterval));
  //Serial.print(formatNumber(lavg));
  //Serial.print(formatNumber(ravg));
  //Serial.print(formatNumber(decisionT));
  //Serial.print(" ");Serial.print(decision);     
  //Serial.println();
  Serial.print(origHit);Serial.print(",");
  Serial.print(sensorValue[COMPSS]);Serial.print(",");
  Serial.print(sensorValue[PITCH]);Serial.print(",");
  Serial.print(sensorValue[ROLL]);Serial.print(",");
  Serial.print(sensorValue[TOF]);Serial.print(",");
  Serial.print(sensorValue[IRLFT]);Serial.print(",");
  Serial.print(maxRaw);Serial.print(",");
  Serial.print(sensorValue[IRRHT]);Serial.print(",");
  Serial.print(lastLftInterval);Serial.print(",");
  Serial.print(lastRhtInterval);Serial.print(",");
  Serial.print(lavg);Serial.print(",");
  Serial.print(ravg);Serial.print(",");
  Serial.print(decisionT);Serial.print(",");
  Serial.print(decision);     
  Serial.println();

  //printFlag = true;
}

String formatNumber(int distance) {
  String sd = "      ";
  sd += distance;
  return sd.substring(sd.length()-6,sd.length()) + " ";  
}

String formatTime(unsigned long utime) {
  String sd = "               ";
  sd += utime;
  return sd.substring(sd.length()-14,sd.length()) + " ";  
}

String formatSTime(unsigned long utime) {
  String sd = "               ";
  sd += utime;
  return sd.substring(sd.length()-8,sd.length()) + " ";  
}
