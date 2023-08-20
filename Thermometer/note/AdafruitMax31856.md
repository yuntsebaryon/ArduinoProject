Thermocouple Readout with Adafruit MAX31856
===========================================

### 2022.8.21

- Reference website: [Adafruit MAX31856](https://learn.adafruit.com/adafruit-max31856-thermocouple-amplifier/wiring-and-test?view=all)
- Wiring:

| MAX31856 Pin | Teensy 4.1 Pin | Note                        |
| ------------ | -------------- | --------------------------- |
| Vin          | Vin            |                             |
| GND          | GND next to pin 13 |                         |
| SCK          | 6              | SPI Clock                   |
| SDO          | 5              | Serial Data Out / Microcontroller In Sensor Out pin, for data sent from the MAX31856 to your processor |
| SDI          | 4              | Serial Data In / Microcontroller Out Sensor In pin, for data sent from your processor to the MAX31856 |
| CS           | 3              | Chip Select pin, drop it low to start an SPI transaction. Its an input to the chip |

- In the code (copied from the Adafruit MAX31856 example), changed to `Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(3, 4, 5, 6);`
- Code in `/Users/yuntse/work/slarchetto/arduino/TCSensor/max31856_oneshot/`

----------------------------------------------------------

### DAQ: 2022.8.22

Use `pySerial` to read the data from the serial port

- Install pySerial \
  `pip install pyserial`
- Python script in `/Users/yuntse/work/arduino_project/TCSensor/daq/teensy_daq.py`
- Note that it will complain `resource busy` and crash if running the python DAQ script with the terminal monitor of Arduino IDE open.
- The data taking rate from the python script should be consistent with the data sending rate from the arduino board, because the python script is basically reading from the temporary file of the serial port line by line.  It will read the data taken long time ago if the two rates differ a lot.

----------------------------------------------------------
