// Basic example using one-shot measurement.
// The call to readThermocoupleTemperature() is blocking for O(100ms)

// Thermocouple
#include <Adafruit_MAX31856.h>
// RTD
#include <Adafruit_MAX31865.h>
// ILI9341 display, not using the touch screen functions
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>


// Thermocouple
// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(3, 4, 5, 6);
// use hardware SPI, just pass in the CS pin
//Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(10);

// RTD
// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31865 thermo = Adafruit_MAX31865(33, 34, 35, 36);
// use hardware SPI, just pass in the CS pin
//Adafruit_MAX31865 thermo = Adafruit_MAX31865(10);

// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF      430.0
// The 'nominal' 0-degrees-C resistance of the sensor
// 100.0 for PT100, 1000.0 for PT1000
#define RNOMINAL  100.0

// Use the touch screen
#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

int cy = tft.height() / 3;

// Import the serial library
#include <SoftwareSerial.h>

// TX of the BT to pin 28; RX of the BT to pin 29
SoftwareSerial HC05(28, 29);  // RX, TX

void setup() {
  
  Serial.begin(115200);
  // while (!Serial) delay(10);
  Serial.println("MAX31856 thermocouple test");
  Serial.println("Adafruit MAX31865 PT100 Sensor Test!");
  
  maxthermo.begin();

  maxthermo.setThermocoupleType(MAX31856_TCTYPE_K);

  thermo.begin(MAX31865_3WIRE);  // set to 2WIRE or 4WIRE as necessary
  delay(500);
  
  tft.begin();

  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 
  
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setRotation(3);
  tft.setTextSize(3);

  HC05.begin(9600);
  tft.setCursor(0, cy);
  tft.print("Thermocouple type: ");
  switch (maxthermo.getThermocoupleType() ) {
    case MAX31856_TCTYPE_B: tft.println("B Type"); break;
    case MAX31856_TCTYPE_E: tft.println("E Type"); break;
    case MAX31856_TCTYPE_J: tft.println("J Type"); break;
    case MAX31856_TCTYPE_K: tft.println("K Type"); break;
    case MAX31856_TCTYPE_N: tft.println("N Type"); break;
    case MAX31856_TCTYPE_R: tft.println("R Type"); break;
    case MAX31856_TCTYPE_S: tft.println("S Type"); break;
    case MAX31856_TCTYPE_T: tft.println("T Type"); break;
    case MAX31856_VMODE_G8: tft.println("Voltage x8 Gain mode"); break;
    case MAX31856_VMODE_G32: tft.println("Voltage x8 Gain mode"); break;
    default: tft.println("Unknown"); break;
  }
  tft.println("Bluetooth ON.");
  
}

void loop() {

  // Read the thermocouple
  float CJTTemp = maxthermo.readCJTemperature();
  // Serial.print("Cold Junction Temp: ");
  // Serial.println(CJTTemp);

  float TCTemp = maxthermo.readThermocoupleTemperature();
  
  // Check and print any faults
  uint8_t tc_fault = maxthermo.readFault();
  if (tc_fault) {
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0, cy);
    if (tc_fault & MAX31856_FAULT_CJRANGE) tft.println("Cold Junction Range Fault");
    if (tc_fault & MAX31856_FAULT_TCRANGE) tft.println("Thermocouple Range Fault");
    if (tc_fault & MAX31856_FAULT_CJHIGH)  tft.println("Cold Junction High Fault");
    if (tc_fault & MAX31856_FAULT_CJLOW)   tft.println("Cold Junction Low Fault");
    if (tc_fault & MAX31856_FAULT_TCHIGH)  tft.println("Thermocouple High Fault");
    if (tc_fault & MAX31856_FAULT_TCLOW)   tft.println("Thermocouple Low Fault");
    if (tc_fault & MAX31856_FAULT_OVUV)    tft.println("Over/Under Voltage Fault");
    if (tc_fault & MAX31856_FAULT_OPEN)    tft.println("Thermocouple Open Fault");
  }

  // Read the RTD
  uint16_t rtd = thermo.readRTD();
  float ratio = rtd;
  ratio /= 32768;
  float cali_ratio = ratio*RREF;
  float RTDTemp = thermo.temperature(RNOMINAL, RREF);
  // Serial.print("Thermocouple Temp: ");
  HC05.print(TCTemp);
  HC05.print(" ");
  HC05.print(CJTTemp);
  HC05.print(" ");
  HC05.print(rtd);
  HC05.print(" ");
  HC05.print(ratio, 2);
  HC05.print(" ");
  HC05.print(cali_ratio,2);
  HC05.print(" ");
  HC05.println(RTDTemp);
  
  // Check and print any faults
  uint8_t rtd_fault = thermo.readFault();
  if (rtd_fault) {
    if (!tc_fault) {
      tft.fillScreen(ILI9341_BLACK);
      tft.setCursor(0, cy);
    }
    tft.print("Fault 0x"); tft.println(rtd_fault, HEX);
    if (rtd_fault & MAX31865_FAULT_HIGHTHRESH) {
      tft.println("RTD High Threshold"); 
    }
    if (rtd_fault & MAX31865_FAULT_LOWTHRESH) {
      tft.println("RTD Low Threshold"); 
    }
    if (rtd_fault & MAX31865_FAULT_REFINLOW) {
      tft.println("REFIN- > 0.85 x Bias"); 
    }
    if (rtd_fault & MAX31865_FAULT_REFINHIGH) {
      tft.println("REFIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (rtd_fault & MAX31865_FAULT_RTDINLOW) {
      tft.println("RTDIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (rtd_fault & MAX31865_FAULT_OVUV) {
      tft.println("Under/Over voltage"); 
    }
    thermo.clearFault();
  }

  // Display the temperatures
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.print("TC: ");
  tft.print(TCTemp);
  tft.println(" C");
  tft.print("CJT: ");
  tft.print(CJTTemp);
  tft.println(" C");
  tft.print("RTD: ");
  tft.println(rtd);
  tft.print("RTD Ratio: ");
  tft.println(ratio,8);
  tft.print("Resistance: ");
  tft.print(cali_ratio,8);
  tft.println(" Ohm");
  tft.print("RTD: ");
  tft.print(RTDTemp);
  tft.println(" C");
  

  delay(1000);
}
