#ifndef Pedal_h
#define Pedal_h

#include <Arduino.h>
#include <Smoothed.h>

class Pedal {
    public:
        Pedal() {};
        virtual ~Pedal() {};
        
        virtual void begin() {
            this->smoothed.begin(SMOOTHED_AVERAGE, 3);
        }
        uint16_t read_value(){
            if(this->is_available()) {
                int32_t value = this->read_raw_value();
                smoothed.add(value);
            }
            return smoothed.get();
        }
        virtual bool is_available() { return false; };
        virtual uint16_t read_raw_value() { return 0; };
    private:
        Smoothed<uint16_t> smoothed;
};
#endif