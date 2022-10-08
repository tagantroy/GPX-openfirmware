#include <Arduino.h>

#include "Pedals.h"

Pedals pedals;

int THROTTLE_SCK = 9;
int THROTTLE_DATA = 8;

int BRAKE_SCK = 7;
int BRAKE_DATA = 6;

int CLUTCH_SCK = 5;
int CLUTCH_DATA = 4;

void setup() {
  Serial.begin(115200);

  pedals.setThrottleOn(true);
  pedals.setThrottleBits("17bit", "15bit");
  pedals.setThrottleLoadcell(THROTTLE_DATA, THROTTLE_SCK);

  pedals.setBrakeOn(true);
  pedals.setBrakeBits("17bit", "15bit");
  pedals.setBrakeLoadcell(BRAKE_DATA, BRAKE_SCK);

  pedals.setClutchOn(true);
  pedals.setClutchBits("17bit", "15bit");
  pedals.setClutchLoadcell(CLUTCH_DATA, CLUTCH_SCK);

  pedals.setup();
}

void loop() {
  pedals.loop();
}
