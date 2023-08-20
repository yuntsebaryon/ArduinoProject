// Basic example using one-shot measurement.
// The call to readThermocoupleTemperature() is blocking for O(100ms)

#include <Adafruit_MAX31856.h>
// ILI9341 display, not using the touch screen functions
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>


// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(3, 4, 5, 6);
// use hardware SPI, just pass in the CS pin
//Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(10);

// Use the touch screen
#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

int cy = tft.height() / 3;


void setup() {
  
  Serial.begin(115200);
  // while (!Serial) delay(10);
  Serial.println("MAX31856 thermocouple test");

  maxthermo.begin();

  maxthermo.setThermocoupleType(MAX31856_TCTYPE_T);

  Serial.print("Thermocouple type: ");
  switch (maxthermo.getThermocoupleType() ) {
    case MAX31856_TCTYPE_B: Serial.println("B Type"); break;
    case MAX31856_TCTYPE_E: Serial.println("E Type"); break;
    case MAX31856_TCTYPE_J: Serial.println("J Type"); break;
    case MAX31856_TCTYPE_K: Serial.println("K Type"); break;
    case MAX31856_TCTYPE_N: Serial.println("N Type"); break;
    case MAX31856_TCTYPE_R: Serial.println("R Type"); break;
    case MAX31856_TCTYPE_S: Serial.println("S Type"); break;
    case MAX31856_TCTYPE_T: Serial.println("T Type"); break;
    case MAX31856_VMODE_G8: Serial.println("Voltage x8 Gain mode"); break;
    case MAX31856_VMODE_G32: Serial.println("Voltage x8 Gain mode"); break;
    default: Serial.println("Unknown"); break;
  }

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
  tft.setTextSize(4);
  
}

void loop() {
  float CJTTemp = maxthermo.readCJTemperature();
  // Serial.print("Cold Junction Temp: ");
  // Serial.println(CJTTemp);

  float TCTemp = maxthermo.readThermocoupleTemperature();
  // Serial.print("Thermocouple Temp: ");
  Serial.print(TCTemp);
  Serial.print(" ");
  Serial.println(CJTTemp);

  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, cy);
  tft.print("TC: ");
  tft.print(TCTemp);
  tft.println(" C");
  tft.print("CJT: ");
  tft.print(CJTTemp);
  tft.println(" C");
  
  // Check and print any faults
  uint8_t fault = maxthermo.readFault();
  if (fault) {
    if (fault & MAX31856_FAULT_CJRANGE) Serial.println("Cold Junction Range Fault");
    if (fault & MAX31856_FAULT_TCRANGE) Serial.println("Thermocouple Range Fault");
    if (fault & MAX31856_FAULT_CJHIGH)  Serial.println("Cold Junction High Fault");
    if (fault & MAX31856_FAULT_CJLOW)   Serial.println("Cold Junction Low Fault");
    if (fault & MAX31856_FAULT_TCHIGH)  Serial.println("Thermocouple High Fault");
    if (fault & MAX31856_FAULT_TCLOW)   Serial.println("Thermocouple Low Fault");
    if (fault & MAX31856_FAULT_OVUV)    Serial.println("Over/Under Voltage Fault");
    if (fault & MAX31856_FAULT_OPEN)    Serial.println("Thermocouple Open Fault");
  }
  delay(1000);
}
