# %pip install pyserial
# 'pyserial' should be insstalled to import serial

from os.path import exists
import time
import serial
import pandas as pd
import csv

seonsor = serial.Serial( '/dev/cu.usbmodem116218601', 115200 )
# 'COM5' is the port name that the Arduino is connected.
# '9600' is the Baudrate.

fName = '/Users/yuntse/data/arduino/TCSensor/TCSensor_20220822.cvs'
# data = pd.DataFrame()
if not exists( fName ):
    f = open( fName, mode = 'w', newline = '' )
    CSVwriter = csv.DictWriter( f, ( 'Time_Stamp', 'TC_Temperature', 'Cold_Junction_Temperature' ))
    CSVwriter.writeheader()
    f.close()

while True:
    signal = seonsor.readline()
    if not type(signal[0]) == int:
        continue

    # print('sensor', time.strftime('%Y-%m-%d %H:%M:%S'), signal)
    temp = signal.decode('utf-8').strip().split(' ')
    signalread = [ time.strftime('%Y-%m-%d %H:%M:%S'), temp[0], temp[1] ]

    f = open( fName, 'a', newline = '' )
    wr = csv.writer(f)
    wr.writerow(signalread)
    f.close()

    time.sleep(1)
