#include <Arduino.h>

#include "Pedals.h"
#include "config.h"

Pedals pedals;

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
