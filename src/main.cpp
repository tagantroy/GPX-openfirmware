#include <Arduino.h>

#include "Pedals.h"
#include "config.h"
#include "settings_storage.h"

Pedals pedals;

void setup() {
  Serial.begin(115200);

  pedals.setThrottleOn(true);
  pedals.setThrottleBits(24, 15);
  pedals.setThrottleLoadcell(THROTTLE_DATA, THROTTLE_SCK);

  pedals.setBrakeOn(true);
  pedals.setBrakeBits(24, 15);
  pedals.setBrakeLoadcell(BRAKE_DATA, BRAKE_SCK);

  pedals.setClutchOn(true);
  pedals.setClutchBits(24, 15);
  pedals.setClutchLoadcell(CLUTCH_DATA, CLUTCH_SCK);

  pedals.setup();
}

void loop() {
  unsigned long start = micros();
  pedals.loop();
  unsigned long end = micros();
  unsigned long delta = end - start;
  Serial.println(delta);
}
