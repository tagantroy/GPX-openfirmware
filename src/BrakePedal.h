#ifndef BrakePedalh
#define BrakePedal_h

#include <Arduino.h>
#include <Adafruit_NAU7802.h>
#include "Pedal.h"

class BrakePedal : public Pedal {
    public:
        BrakePedal() {};
        virtual ~BrakePedal() {};
        void setNAU(Adafruit_NAU7802* nau) {
            this->nau = nau;
        }
        virtual void begin() override;
        virtual bool is_available() override;
        virtual uint16_t read_raw_value() override;
    private: 
        Adafruit_NAU7802* nau;
};
#endif