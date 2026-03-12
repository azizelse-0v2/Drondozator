#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <AccelStepper.h>

// --- PIN DEFINITIONS ---
const int RX_PIN = 4; // To SpeedyBee TX (Use voltage divider!)
const int TX_PIN = 5; // To SpeedyBee RX
const int STEP_PIN = 2;
const int DIR_PIN = 3;

// --- CONFIGURATION ---
const float STEPS_PER_METER = 200.0; // Adjust based on your mechanics
static const uint32_t GPSBaud = 57600;

// Objects
TinyGPSPlus gps;
SoftwareSerial ss(RX_PIN, TX_PIN);
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// Variables for calculation
double startLat = 0, startLon = 0;
bool homeSet = false;

void setup() {
  Serial.begin(115200);
  ss.begin(GPSBaud);
  
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(500);
  
  Serial.println(F("System Initialized. Waiting for GPS fix..."));
}

void loop() {
  // Read from SpeedyBee
  while (ss.available() > 0) {
    if (gps.encode(ss.read())) {
      processGPS();
    }
  }

  stepper.run(); // Keep the motor moving smoothly
}

void processGPS() {
  if (gps.location.isValid()) {
    // 1. Set "Home" on first valid fix
    if (!homeSet) {
      startLat = gps.location.lat();
      startLon = gps.location.lng();
      homeSet = true;
      Serial.println(F("Home Position Set!"));
    }

    // 2. Calculate distance from Home in meters
    double distanceTotal = TinyGPSPlus::distanceBetween(
      gps.location.lat(),
      gps.location.lng(),
      startLat,
      startLon
    );

    // 3. Map distance to stepper position
    // Example: Motor turns proportional to distance from home
    long targetPosition = (long)(distanceTotal * STEPS_PER_METER);
    stepper.moveTo(targetPosition);

    // Debugging
    Serial.print(F("Distance: "));
    Serial.print(distanceTotal);
    Serial.print(F("m | Motor Target: "));
    Serial.println(targetPosition);
  }
}