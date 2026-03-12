#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Choose two pins for SoftwareSerial
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600; // Default for NEO-6M

// Create TinyGPS++ and SoftwareSerial objects
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

void setup() {
  Serial.begin(115200); // PC communication
  ss.begin(GPSBaud);    // GPS communication

  Serial.println(F("Initializing GPS..."));
}

void loop() {
  // Feed the library every character coming from the GPS module
  while (ss.available() > 0) {
    if (gps.encode(ss.read())) {
      displayInfo();
    }
  }

  // If no data is received within 5 seconds, report an error
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

void displayInfo() {
  Serial.print(F("Location: ")); 
  if (gps.location.isValid()) {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid()) {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }

  Serial.print(F(" "));
  if (gps.time.isValid()) {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
  }

  Serial.print(F(" Speed: "));
  Serial.print(gps.speed.kmph());
  Serial.println(F(" km/h"));
}