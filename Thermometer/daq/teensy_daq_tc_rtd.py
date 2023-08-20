# %pip install pyserial
# 'pyserial' should be insstalled to import serial

from os.path import exists
import time
import serial
import pandas as pd
import csv

# seonsor = serial.Serial( '/dev/cu.usbmodem116218601', 115200 )
seonsor = serial.Serial( '/dev/cu.HC-05', 115200 )
# 'COM5' is the port name that the Arduino is connected.
# '9600' is the Baudrate.

fName = '/Users/yuntse/data/arduino/TempSensor/TempSensor_20221107.cvs'
# data = pd.DataFrame()
if not exists( fName ):
    f = open( fName, mode = 'w', newline = '' )
    CSVwriter = csv.DictWriter( f, ( 'Time_Stamp', 'TC_Temperature', 'Cold_Junction_Temperature', 'RTD_Value', 'RTD_Ratio', 'RTD_Resistance', 'RTD_Temperature' ))
    # CSVwriter = csv.DictWriter( f, ( 'Time_Stamp', 'RTD_Resistance', 'RTD_Temperature' ))
    CSVwriter.writeheader()
    f.close()

while True:
    signal = seonsor.readline()
    if not type(signal[0]) == int:
        continue

    print('sensor', time.strftime('%Y-%m-%d %H:%M:%S'), signal)
    temp = signal.decode('utf-8').strip().split(' ')
    signalread = [ time.strftime('%Y-%m-%d %H:%M:%S'), temp[0], temp[1], temp[2], temp[3], temp[4], temp[5] ]
    # signalread = [ time.strftime('%Y-%m-%d %H:%M:%S'), temp[0], temp[1] ]

    f = open( fName, 'a', newline = '' )
    wr = csv.writer(f)
    wr.writerow(signalread)
    f.close()

    time.sleep(1)
