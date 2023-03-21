
void writeRegister(int deviceAddress, byte address, byte val) {
    Wire.beginTransmission(deviceAddress); // start transmission to device
    Wire.write(address);       // send register address
    Wire.write(val);           // send value to write
    Wire.endTransmission();    // end transmission
}
 
