#include <Arduino.h>

#include "Pedals.h"

Pedals pedals;

int THROTTLE_SCK = 9;
int THROTTLE_DATA = 8;

int BRAKE_SCK = 7;
int BRAKE_DATA = 6;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  pedals.setThrottleOn(true);
  pedals.setThrottleBits("10bit", "15bit");
  pedals.setThrottleLoadcell(THROTTLE_SCK,THROTTLE_DATA);

  pedals.setBrakeOn(true);
  pedals.setBrakeBits("20bit", "15bit");
  pedals.setBrakeLoadcell(BRAKE_SCK,BRAKE_DATA);

  pedals.setClutchOn(false);
  // pedals.setClutchBits("10bit", "15bit");
  // pedals.setClutchLoadcell(8,5);

  pedals.setup();
}

// the loop routine runs over and over again forever:
void loop() {
  //  timing
//  unsigned long start = micros();

  pedals.loop();

//  unsigned long end = micros();
//  unsigned long delta = end - start;
//  Serial.println(delta);
}
