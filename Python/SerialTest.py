import serial
import os
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=5)
ser.flushInput()
full_path = "/home/pi/Zuisse/rlog1.log"
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