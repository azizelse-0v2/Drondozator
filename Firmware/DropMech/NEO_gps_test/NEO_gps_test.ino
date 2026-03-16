#include <Arduino.h>
#include <U8g2lib.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SPI.h>

// GPS Pins
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

// Display Setup for SPI (Pin 8:CS, 10:DC, 9:Reset)
// Hardware SPI uses Pin 11 (MOSI) and Pin 13 (SCK) automatically
U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, 8, 10, 9);

void setup() {
  ss.begin(GPSBaud);
  u8g2.begin();
}

void loop() {
  // Feed GPS data to the parser
  while (ss.available() > 0) {
    if (gps.encode(ss.read())) {
      updateDisplay();
    }
  }

  // Optional: Update display even if no new valid encoding (to show 0)
  if (millis() % 1000 == 0) {
    updateDisplay();
  }
}

void updateDisplay() {
  u8g2.clearBuffer();
  
  // Header text
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(25, 15, "SATELLITES");
  
  // Large Number for Satellites
  u8g2.setFont(u8g2_font_logisoso32_tn); // Nice, big numbers
  
  int satCount = 0;
  if (gps.satellites.isValid()) {
    satCount = gps.satellites.value();
  }
  
  // Center the number roughly
  u8g2.setCursor(50, 55);
  u8g2.print(satCount);
  
  u8g2.sendBuffer();
}