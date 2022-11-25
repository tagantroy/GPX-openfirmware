#ifndef ThrottlePedal_h
#define ThrottlePedal_h

#include <Arduino.h>
#include <Adafruit_ADS1X15.h>
#include "Pedal.h"
#include <Smoothed.h>

class ThrottlePedal : public Pedal {
    public:
        ThrottlePedal() {};
        virtual ~ThrottlePedal() {};
        void setADS(Adafruit_ADS1X15* ads) {
            this->ads = ads;
        }
        void setPin(uint16_t pin) {
            this->pin = pin;
        }
        virtual void begin() override;
        virtual bool is_available() override;
        virtual uint16_t read_raw_value() override;
    private: 
        Adafruit_ADS1X15* ads;
        uint16_t pin;
};
#endif