# Takyo
![Takyo](takyo.jpg)
## Sensors
* Ultrasonic Sensor            - MB1040
* 2 Infrared Sensors           - GP2Y0A21YK0F
* Time of Flight Sensor        - VL53L0x
* Compass / Gyro Accelerometer - LSM303
## Processors
* Arduino Nano
* Raspberry Pi Zero W 
## Data Generator
Takyo generates the following data for Classification.
Column indexes to lower: 
* org     - The original sensor hit
* compss  - Current compass reading
* pitch   - Pitch
* roll    - Roll
* cliff   - Edge detection, also called Cliff, also called TOF
* lft     - Sensor value at time of hit
* ctr     - Sensor value at time of hit
* rht     - Sensor value at time of hit
* lastlft - Time since last hit (maximum 10000 ms)
* lastrht - Time since last hit
* lftavg  - Average times since last hit (mean)
* rhtavg  - Average times since last hit (mean)
* dturn   - Decision Turn delay - this column is dropped 
* ddir    - Decision Turn Type (ddir is what we are trying to predict)
  
