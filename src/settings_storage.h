#ifndef SettingsStorage_h
#define SettingsStorage_h

#include "settings.pb.h"

class SettingsStorage {
    public:
        SettingsStorage(){};
        void save(const Settings &newData);
        Settings read();
    private:
        void saveToEEPROM(uint8_t buffer[], size_t size);
        void readFromEEPROM(uint8_t* buffer, size_t size);
};
#endif