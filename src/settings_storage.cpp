#include "settings_storage.h"

#include <Arduino.h>
#include <EEPROM.h>

#include "settings.pb.h"

#include <pb_encode.h>
#include <pb_decode.h>

size_t BUFFER_SIZE = 128;

Settings SettingsStorage::read(){
    EEPROM.begin();
    uint8_t buffer[BUFFER_SIZE];
    this->readFromEEPROM(buffer, sizeof(buffer));
    pb_istream_t stream = pb_istream_from_buffer(buffer, sizeof(buffer));
    Settings data = Settings_init_zero;
    pb_decode(&stream, Settings_fields, &data);
    EEPROM.end();
    return data;
}

void SettingsStorage::save(const Settings &newData) {
    EEPROM.begin();
    uint8_t buffer[BUFFER_SIZE];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    bool status = pb_encode(&stream, Settings_fields, &newData);
    size_t message_length = stream.bytes_written;
    Serial.print("MESSAGE SIZE: ");
    Serial.println(message_length);
    if(status){
        this->saveToEEPROM(buffer, message_length);
    }else {
        Serial.print("Cannot encode message:");
        Serial.println(stream.errmsg);
    }
    EEPROM.end();
}

void SettingsStorage::readFromEEPROM(uint8_t* buffer, size_t size) {
    EEPROM.begin();
    for(size_t i = 0;i<size;i++){
        buffer[i] = EEPROM.read(i);
    }
    EEPROM.end();
}

void SettingsStorage::saveToEEPROM(uint8_t* buffer, size_t size) {
    EEPROM.begin();
    for(size_t i = 0;i<size;i++){
        EEPROM.write(i, buffer[i]);
    }
    EEPROM.end();
}



