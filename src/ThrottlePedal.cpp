#include "ThrottlePedal.h"


void ThrottlePedal::begin() {
    Pedal::begin();
}

uint16_t ThrottlePedal::read_raw_value() {
    int32_t value = this->ads->readADC_SingleEnded(this->pin);
    return value;
}

bool ThrottlePedal::is_available() {
    bool complete = this->ads->conversionComplete();
    if(!complete) {
        Serial.println("ADS NOT COMPLETE YET");
    }
    return complete;
}