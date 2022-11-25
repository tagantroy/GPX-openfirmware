#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h> 
#include <SPI.h>

#include "Pedals.h"

Pedals pedals;

#define WIRE Wire

void setup() {
  Serial.begin(115200);
  pedals.setup(&Wire);
  pedals.setupThrottle(1);
  pedals.setupBrake();
}

void loop() {
  pedals.updateValues();
}
