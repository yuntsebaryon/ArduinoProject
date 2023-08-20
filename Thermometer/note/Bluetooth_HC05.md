Bluetooth HC-05 Module
======================

### Connect to the Bluetooth HC-05 Module: 2022.10.29

Done in late September, but didn't take a note until now.

- Teensy code at `/Users/yuntse/work/ArduinoProject/Thermometer/max31856_display_bt/max31856_display_bt.ino`
- DAQ code at `/Users/yuntse/work/ArduinoProject/Thermometer/daq/teensy_daq.py`, use
```
seonsor = serial.Serial( '/dev/cu.HC-05', 115200 )
```
- HC-05 bluetooth module passkey: `1234`
- If data is not transferred, try removing the HC-05 module from the bluetooth list and search, pair it again with the laptop.
