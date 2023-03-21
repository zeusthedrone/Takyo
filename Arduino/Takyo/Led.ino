
void updateHeartbeat() {
  if ((currentMillis - heartbeatMillis) > heartbeatInterval) {
    // pass
  } else {
    return;
  } 
  heartbeatMillis = currentMillis;
  heartbeatToggle = (heartbeatToggle) ? false : true;
  if (heartbeatToggle) {
    digitalWrite(P_13, HIGH);
  } else {
    digitalWrite(P_13, LOW);
  }
}
 
