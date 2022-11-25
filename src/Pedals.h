#ifndef Pedals_h
#define Pedals_h

#include <Arduino.h>

#include "BrakePedal.h"
#include "ThrottlePedal.h"

class Pedals {
    public:
        Pedals(){};
        void setupThrottle(uint8_t channel);
        void setupBrake();
        void setup(TwoWire *wire);
        void updateValues();
    private:
        Adafruit_ADS1115 ads;
        Adafruit_NAU7802 nau;

        ThrottlePedal throttle;
        BrakePedal brake;

        void setupNAU(TwoWire *wire);
        void setupADS(TwoWire *wire);
};
#endif