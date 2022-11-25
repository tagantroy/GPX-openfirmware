#include "BrakePedal.h"


void BrakePedal::begin() {
    Pedal::begin();
}

uint16_t BrakePedal::read_raw_value() {
    int32_t value = this->nau->read();
    if(value<0) {
        value = 0;
    }
    return value;
}

bool BrakePedal::is_available() {
    bool available = this->nau->available();
    return available;
}