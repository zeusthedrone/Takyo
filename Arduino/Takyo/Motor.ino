
void moveMotors(uint8_t mDir) {
  if (mDir == BCK) {
    mBackward() ;
  } else 
  if (mDir == FWD) {
    mForward() ; 
  } else
  if (mDir == LFT) {
    mLeft() ;
  } else
  if (mDir == RHT) {
    mRight() ;
  } else 
  if (mDir == STP) {
    allStop() ;
  }
}

void mBackward() {
  currentDirection = BCK;
  if (!motorsOn) { return; }
  digitalWrite(P_BIN2, HIGH); // left
  digitalWrite(P_BIN1, LOW);
  digitalWrite(P_AIN2, HIGH); // right
  digitalWrite(P_AIN1, LOW);
  
}

void mForward(){
  currentDirection = FWD;
  if (!motorsOn) { return; }
  analogWrite( P_BIN1, 246);   // left
  digitalWrite(P_BIN2, LOW);  
  analogWrite( P_AIN1, 250);   // right
  digitalWrite(P_AIN2, LOW);    
  
}
 
void mRight() {
  if (!motorsOn) { return; }
  digitalWrite(P_BIN1, HIGH); // left
  digitalWrite(P_BIN2, LOW);  
  digitalWrite(P_AIN1, LOW);  // right
  digitalWrite(P_AIN2, HIGH);
}

void mLeft() {
  if (!motorsOn) { return; }
  digitalWrite(P_BIN1, LOW); // left
  digitalWrite(P_BIN2, HIGH);  
  digitalWrite(P_AIN1, HIGH); // right
  digitalWrite(P_AIN2, LOW);  
}
  
void allStop() {
  currentDirection = STP;
  if (!motorsOn) { return; }
  digitalWrite(P_AIN1, LOW);
  digitalWrite(P_AIN2, LOW);
  digitalWrite(P_BIN1, LOW);
  digitalWrite(P_BIN2, LOW);
}
