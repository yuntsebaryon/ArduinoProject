RTD Readout with Adafruit 31865
===============================

### 2022.10.29

- Reference website: [Adafruit 31865](https://learn.adafruit.com/adafruit-max31865-rtd-pt100-amplifier)
- Wiring

| MAX31856 Pin | Teensy 4.1 Pin | Note                        |
| ------------ | -------------- | --------------------------- |
| Vin          | Vin            |                             |
| GND          | GND next to pin 13 |                         |
| CLK          | 36             | SPI Clock                   |
| SDO          | 35             | Serial Data Out / Microcontroller In Sensor Out pin, for data sent from the MAX31856 to your processor |
| SDI          | 34             | Serial Data In / Microcontroller Out Sensor In pin, for data sent from your processor to the MAX31856 |
| CS           | 33             | Chip Select pin, drop it low to start an SPI transaction. Its an input to the chip |

- Code in `/Users/yuntse/work/ArduinoProject/Thermometer/tc_rtd_display_bt/tc_rtd_display_bt.ino`
- DAQ code `/Users/yuntse/work/ArduinoProject/Thermometer/daq/teensy_daq_tc_rtd.py`
- Bluetooth doesn't work

----------------------------------------------------

### 2022.11.7

- After soldering the two points and cut the trace on the breakout board, as described in the [Adafruit 31865 technote](https://learn.adafruit.com/adafruit-max31865-rtd-pt100-amplifier/rtd-wiring-config), it works!
- And the HC05 bluetooth module also works.
