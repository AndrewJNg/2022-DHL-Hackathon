import serial
import time

# https://roboticsbackend.com/raspberry-pi-arduino-serial-communication/

if __name__ == '__main__':
    # ser = serial.Serial('/dev/ttyUSB0',115200,timeout=0.1)
    ser = serial.Serial('COM5',115200,timeout=1)
    ser.reset_input_buffer()


    while True:


        # if ser.in_waiting >0:
            # line = ser.readline().decode('utf-8').rstrip()
            # print(line)
            ser.write(b"R_7")
            time.sleep(3)
            ser.write(b"L_2")
            # print(b"R_5")
            # time.sleep(0.1)
            time.sleep(2)
            # ser.write(b"L_17\n")
            # time.sleep(5)

