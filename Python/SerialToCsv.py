# This program runs on demand not concurrently on a Raspberry Pi Zero W. 
# It captures the serial output from the Arduino and writes it to an sd card
# The rlog1.csv is later transferred in batch using FTP to a larger computer
# where Classification is performed. zeusthedrone@gmail.com
import serial
import os
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=5)
ser.flushInput()
full_path = "/home/pi/Zuisse/rlog1.csv"
fd = os.open(full_path, os.O_CREAT | os.O_RDWR)
cnt = 0

while True:
    try:
        ser_bytes = ser.readline()
        decoded_bytes = ser_bytes.decode("utf-8") 
        print(cnt)
        cnt = cnt +1
        edata = str.encode(decoded_bytes)
        
        ret = os.write(fd, edata)
        if cnt > 2000:
            os.close(fd)
            break
    except KeyboardInterrupt:
        print("Keyboard Interrupt")
        os.close(fd)
        break
    except:
        os.close(fd)
        import traceback
        traceback.print_exc()
        break