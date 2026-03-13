#include <AccelStepper.h>

// Updated to your specific pins
const int stepPin = 6; 
const int dirPin = 7;

AccelStepper stepper(1, stepPin, dirPin);

void setup() {
  stepper.setMaxSpeed(400);     // Start slow for testing
  stepper.setAcceleration(200); 
  stepper.moveTo(400);          // 400 steps = 1 rev in half-step mode
}

void loop() {
  if (stepper.distanceToGo() == 0) {
    stepper.moveTo(-stepper.currentPosition()); // Flip-flop position
  }
  stepper.run();
}