
#include "Pedals.h"

#include "HID-Project.h"

void Pedals::setupThrottle(uint8_t channel) {
    this->throttle.setPin(channel);
    this->throttle.begin();
}

void Pedals::setupBrake() {
    this->brake.begin();
}

void Pedals::setupNAU(TwoWire *wire) {
    Serial.println("SETUP1");
    while (!this->nau.begin(wire)) {
        delay(100);
        Serial.println("Failed to find NAU7802");
    }
    Serial.println("Found NAU7802");
    this->nau.setLDO(NAU7802_4V5);
    Serial.print("LDO voltage set to 3.3V");
    Serial.print("Gain set to 64");
    this->nau.setGain(NAU7802_GAIN_2);
    Serial.print("Conversion rate set to 320SPS");
    this->nau.setRate(NAU7802_RATE_320SPS);
    for (uint8_t i=0; i<10; i++) {
        while (! nau.available()) delay(1);
        nau.read();
    }
    Serial.println("SETUP4");
    while (! nau.calibrate(NAU7802_CALMOD_INTERNAL)) {
    Serial.println("Failed to calibrate internal offset, retrying!");
    delay(1000);
  }
  Serial.println("Calibrated internal offset");

  while (! nau.calibrate(NAU7802_CALMOD_OFFSET)) {
    Serial.println("Failed to calibrate system offset, retrying!");
    delay(1000);
  }
  Serial.println("Calibrated system offset");
    Serial.println("SETUP5");
}

void Pedals::setupADS(TwoWire *wire) {
    while(!this->ads.begin(ADS1X15_ADDRESS, wire)) {
        Serial.println("Cannot init ADS");
    }
    Serial.println("ADS FOUND");
    this->ads.setGain(GAIN_TWOTHIRDS);
    this->ads.setDataRate(250);
    Serial.println("ADS INIT FINISHED");
}

void Pedals::setup(TwoWire *wire) {
    Gamepad.begin();

    Serial.println("Setup NAU");
    this->setupNAU(wire);
    this->setupADS(wire);
    
    this->brake.setNAU(&this->nau);
    this->throttle.setADS(&this->ads);
}

void Pedals::updateValues() {
    uint16_t throttle = this->throttle.read_value();
    uint16_t brake = this->brake.read_value();

    Gamepad.xAxis(throttle);
    Gamepad.yAxis(brake-32768);
    Gamepad.write();

    Serial.print("Throttle: ");
    Serial.println(throttle);
    Serial.print("Brake: ");
    Serial.println(brake);
}