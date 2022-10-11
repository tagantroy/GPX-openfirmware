#ifndef Pedals_h
#define Pedals_h

#include <Joystick.h>
#include <MultiMap.h>
#include <SoftwareReset.hpp>
#include <EEPROM.h>

#include "UtilLibrary.h"

#include "Pedal.h"

#define E_INIT 1023
#define E_CLUTCH 0
#define E_THROTTLE 30
#define E_BRAKE 60
#define E_CALIBRATION_C 90
#define E_CALIBRATION_B 120
#define E_CALIBRATION_T 150
#define E_PEDAL_INVERTED_MAP 200
#define E_PEDAL_SMOOTH_MAP 210
#define SERIAL_RANGE 100

String cm = ",";
String dash = "-";

Joystick_ _joystick(
  JOYSTICK_DEFAULT_REPORT_ID,
  JOYSTICK_TYPE_GAMEPAD,
  0, 0,                 // Button Count, Hat Switch Count
  false, false, false,  // X and Y, Z Axis
  true, true, true,     // Rx, Ry, or Rz
  false, false,         // rudder or throttle
  false, false, false   // accelerator, brake, or steering
); 

// create the pedals
Pedal _throttle = Pedal("T:");
Pedal _brake = Pedal("B:");
Pedal _clutch = Pedal("C:");

class Pedals {
  public:
    //initialise pedal

    void setup() {
      loadEEPROMSettings();
      _joystick.begin();

      _joystick.setRxAxis(0);
      _joystick.setRxAxisRange(0, (_throttle_hid_bit - 1));
      _throttle.setBits((_throttle_raw_bit - 1), (_throttle_hid_bit - 1));

      _joystick.setRyAxis(0);
      _joystick.setRyAxisRange(0, (_brake_hid_bit - 1));
      _brake.setBits((_brake_raw_bit - 1), (_brake_hid_bit - 1));

      _joystick.setRzAxis(0);
      _joystick.setRzAxisRange(0, (_clutch_hid_bit - 1));
      _clutch.setBits((_clutch_raw_bit - 1), (_clutch_hid_bit - 1));
    }

    void loop() {
      if (Serial.available() > 0) {
        String msg = Serial.readStringUntil('\n');

        if (msg.indexOf("clearEEPROM") >= 0) {
          for (uint16_t i = 0; i < EEPROM.length(); i++) {
            EEPROM.write(i, 0);
          }
          Serial.println("done");
        }

        resetDevice(msg);
        getUsage(msg);
        getMap(msg);
        getInverted(msg);
        getSmooth(msg);
        getCalibration(msg);
        getBits(msg);

        if (msg.indexOf("CMAP:") >= 0) {
          String cmap = msg;
          cmap.replace("CMAP:", "");
          _clutch.setOutputMapValues(cmap, E_CLUTCH);
        }

        if (msg.indexOf("BMAP:") >= 0) {
          String bmap = msg;
          bmap.replace("BMAP:", "");
          _brake.setOutputMapValues(bmap, E_BRAKE);
        }

        if (msg.indexOf("TMAP:") >= 0) {
          String tmap = msg;
          tmap.replace("TMAP:", "");
          _throttle.setOutputMapValues(tmap, E_THROTTLE);
        }

        if (msg.indexOf("CALIRESET") >= 0) {
          _clutch.resetCalibrationValues(E_CALIBRATION_C);
          _brake.resetCalibrationValues(E_CALIBRATION_B);
          _throttle.resetCalibrationValues(E_CALIBRATION_T);
        }

        if (msg.indexOf("CCALI:") >= 0 && msg.indexOf("BCALI:") >= 0 && msg.indexOf("TCALI:") >= 0) {
          String splitTCALI = utilLib.getValue(msg, ',', 0);
          splitTCALI.replace("TCALI:", "");
          _throttle.setCalibrationValues(splitTCALI, E_CALIBRATION_T);

          String splitBCALI = utilLib.getValue(msg, ',', 1);
          splitBCALI.replace("BCALI:", "");
          _brake.setCalibrationValues(splitBCALI, E_CALIBRATION_B);

          String splitCCALI = utilLib.getValue(msg, ',', 2);
          splitCCALI.replace("CCALI:", "");
          _clutch.setCalibrationValues(splitCCALI, E_CALIBRATION_C);
        }

        updateInverted(msg);
        updateSmooth(msg);
      }

      String SerialString = "";

      if(_throttle_on){
        _throttle.readValues();
        _joystick.setRxAxis(_throttle.getAfterHID());
          SerialString += _throttle.getPedalString();
      }
      
      if(_brake_on){
         _brake.readValues();
         _joystick.setRyAxis(_brake.getAfterHID());
         SerialString += _brake.getPedalString();
      }

      if(_clutch_on){
        _clutch.readValues();
        _joystick.setRzAxis(_clutch.getAfterHID());
        SerialString += _clutch.getPedalString();
      }

      _joystick.sendState(); // Update the Joystick status on the PC

      if (Serial.availableForWrite()) {
        Serial.println(SerialString);
      }
    }

    ///////////////////////// throttle /////////////////////////
    void setThrottleOn(bool on) {
      _throttle_on = on;
    }

    void setThrottleBits(int rawBit, int hidBit) {
      _throttle_raw_bit = getBit(rawBit);
      _throttle_hid_bit = getBit(hidBit);
    }

    void setThrottleLoadcell(int DOUT, int CLK) {
      _throttle.ConfigLoadCell(DOUT, CLK);
    }

    ///////////////////////// brake /////////////////////////
    void setBrakeOn(bool on) {
      _brake_on = on;
    }

    void setBrakeBits(int rawBit, int hidBit) {
      _brake_raw_bit = getBit(rawBit);
      _brake_hid_bit = getBit(hidBit);
    }

    void setBrakeLoadcell(int DOUT, int CLK) {
      _brake.ConfigLoadCell(DOUT, CLK);
    }

    ///////////////////////// clutch /////////////////////////
    void setClutchOn(bool on) {
      _clutch_on = on;
    }

    void setClutchBits(int rawBit, int hidBit) {
      _clutch_raw_bit = getBit(rawBit);
      _clutch_hid_bit = getBit(hidBit);
    }

    void setClutchLoadcell(int DOUT, int CLK) {
      _clutch.ConfigLoadCell(DOUT, CLK);
    }

  private:
    ////// throttle config //////
    bool _throttle_on = false;
    long _throttle_raw_bit = 65535; // default 16bit
    long _throttle_hid_bit = 65535; // default 16bit

    ////// brake config //////
    bool _brake_on = false;
    long _brake_raw_bit = 65535; // default 16bit
    long _brake_hid_bit = 65535; // default 16bit

    ////// clutch config //////
    bool _clutch_on = false;
    long _clutch_raw_bit = 65535; // default 16bit
    long _clutch_hid_bit = 65535; // default 16bit

    int ipow(int base, int exp)
    {
      int result = 1;
      for (;;)
      {
        if (exp & 1)
          result *= base;
          exp >>= 1;
          if (!exp) {
            break;
          }
          base *= base;
      }
      return result;
    }

    long getBit(int bits) {
      if(bits < 8 || bits > 24) {
        return 65535;
      }
      return ipow(2, bits) - 1;
    }

    /////////////////////////////////////////////

    void loadEEPROMSettings() {
      if (EEPROM.read(E_INIT) == 'T') {
        loadDeviceSettings();
      } else {
        resetDeviceSettings();
      }
    }

    void loadDeviceSettings() {
      _clutch.getEEPROMOutputMapValues(E_CLUTCH);
      _brake.getEEPROMOutputMapValues(E_BRAKE);
      _throttle.getEEPROMOutputMapValues(E_THROTTLE);


      String EEPROM_InvertedMap = utilLib.readStringFromEEPROM(E_PEDAL_INVERTED_MAP);
      String INVER = "INVER:";
      updateInverted(INVER + EEPROM_InvertedMap);

      String EEPROM_SmoothMap = utilLib.readStringFromEEPROM(E_PEDAL_SMOOTH_MAP);
      String SMOOTH = "SMOOTH:";
      updateSmooth(SMOOTH + EEPROM_SmoothMap);


      _clutch.getEEPROMCalibrationValues(E_CALIBRATION_C);
      _brake.getEEPROMCalibrationValues(E_CALIBRATION_B);
      _throttle.getEEPROMCalibrationValues(E_CALIBRATION_T);

    }

    void resetDeviceSettings() {
      // write
      EEPROM.write(E_INIT, 'T');

      _clutch.resetOutputMapValues(E_CLUTCH);
      _brake.resetOutputMapValues(E_THROTTLE);
      _throttle.resetOutputMapValues(E_BRAKE);

      // 0 = false / 1 = true
      utilLib.writeStringToEEPROM(E_PEDAL_INVERTED_MAP, "0-0-0");

      // 0 = false / 1 = true
      utilLib.writeStringToEEPROM(E_PEDAL_SMOOTH_MAP, "1-1-1");

      _clutch.resetCalibrationValues(E_CALIBRATION_C);
      _brake.resetCalibrationValues(E_CALIBRATION_B);
      _throttle.resetCalibrationValues(E_CALIBRATION_T);

      softwareReset::standard();
    }

    void resetDevice(String msg) {
      if (msg.indexOf("ResetDevice") >= 0) {
        resetDeviceSettings();
      }
    }

    void getUsage(String msg) {
      if (msg.indexOf("GetUsage") >= 0) {
        String USAGE = "USAGE:";
        Serial.println(USAGE + _throttle_on + dash + _brake_on + dash + _clutch_on);
      }
    }

    void getMap(String msg) {
      if (msg.indexOf("GetMap") >= 0) {
        Serial.println(
            _throttle.getOutputMapValues("TMAP:") + cm +
            _brake.getOutputMapValues("BMAP:") + cm +
            _clutch.getOutputMapValues("CMAP:")
        );
      }
    }

    void getSmooth(String msg) {
      if (msg.indexOf("GetSmooth") >= 0) {
        String SMOOTH = "SMOOTH:";
        Serial.println(
            SMOOTH +
            _throttle.getSmoothValues() + dash +
            _brake.getSmoothValues() + dash +
            _clutch.getSmoothValues()
       );
      }
    }

    void getBits(String msg) {
      if (msg.indexOf("GetBits") >= 0) {
        String BITS = "BITS:";
        Serial.println(
            BITS +
            _throttle_raw_bit + dash + _throttle_hid_bit + dash +
            _brake_raw_bit + dash + _brake_hid_bit + dash +
            _clutch_raw_bit+ dash + _clutch_hid_bit
        );
      }
    }


    void getInverted(String msg) {
      if (msg.indexOf("GetInverted") >= 0) {
        String INVER = "INVER:";
        Serial.println(
            INVER +
            _throttle.getInvertedValues() + dash +
            _brake.getInvertedValues() + dash +
            _clutch.getInvertedValues()
        );
      }
    }

    void getCalibration(String msg) {
      if (msg.indexOf("GetCali") >= 0) {
        String cm = ",";
        Serial.println(
            _throttle.getCalibrationValues("TCALI:") + cm +
            _brake.getCalibrationValues("BCALI:") + cm +
            _clutch.getCalibrationValues("CCALI:")
        );
      }
    }

    void updateSmooth(String msg) {
      if (msg.indexOf("SMOOTH:") >= 0) {
        String splitSMOOTH = utilLib.getValue(msg, ',', 0);
        splitSMOOTH.replace("SMOOTH:", "");
        _throttle.enableSmoothing(utilLib.getValue(splitSMOOTH, '-', 0).toInt());
        _brake.enableSmoothing(utilLib.getValue(splitSMOOTH, '-', 1).toInt());
        _clutch.enableSmoothing(utilLib.getValue(splitSMOOTH, '-', 2).toInt());

        utilLib.writeStringToEEPROM(E_PEDAL_SMOOTH_MAP, splitSMOOTH);
      }
    }

    void updateInverted(String msg) {
      if (msg.indexOf("INVER:") >= 0) {
        String splitINVER = utilLib.getValue(msg, ',', 0);
        splitINVER.replace("INVER:", "");
        _throttle.setInvertedValues(utilLib.getValue(splitINVER, '-', 0).toInt());
        _brake.setInvertedValues(utilLib.getValue(splitINVER, '-', 1).toInt());
        _clutch.setInvertedValues(utilLib.getValue(splitINVER, '-', 2).toInt());

        utilLib.writeStringToEEPROM(E_PEDAL_INVERTED_MAP, splitINVER);
      }
    }

};
#endif
