#ifndef Pedal_h
#define Pedal_h

#include "UtilLibrary.h"

#include <HX711.h>

#include <Smoothed.h>

// init util library
UtilLib utilLib;

class Pedal
{
  public:
    //initialise pedal
    Pedal(String prefix) {
      _prefix = prefix;
    }

    void setBits (long rawBit, long hidBit) {
      _raw_bit = rawBit;
      _hid_bit = hidBit;
    }

    void ConfigLoadCell (int DOUT, int CLK)
    {
      _loadCell.begin(DOUT, CLK, _loadcell_gain);
      _loadCell.set_scale(-7050.0);  // -7050.0 This value is obtained using the SparkFun_HX711_Calibration sketch
      _loadCell.tare(_loadcell_tare_reps); // Reset values to zero
    }

    int getAfterHID() {
      return _afterHID;
    }

    String getPedalString() {
      return _pedalString;
    }

    void readValues() {
      long rawValue = 0;
      rawValue = _loadCell.get_value(1) / 256;
      if (rawValue > _loadcell_max_val) {
        rawValue = (_loadcell_max_val - 1);
      }
      if (rawValue < 0) rawValue = 0;

      updatePedal(rawValue);
    }

    void enableSmoothing(int smoothingValue) {
      _smooth = smoothingValue;
      pedalFilter.begin(SMOOTHED_AVERAGE, 5);
    }

    int getSmoothValues() {
      return _smooth;
    }

    void setInvertedValues(int invertedValues) {
      _inverted = invertedValues;
    }

    int getInvertedValues() {
      return _inverted;
    }

    ////////////////////
    void resetCalibrationValues(int EEPROMSpace) {
      long resetMap[4] = {0, _raw_bit, 0, _raw_bit};
      _calibration[0] = resetMap[0];
      _calibration[1] = resetMap[1];
      _calibration[2] = resetMap[2];
      _calibration[3] = resetMap[3];
      utilLib.writeStringToEEPROM(EEPROMSpace, utilLib.generateStringMapCali(resetMap));

    }

    void getEEPROMCalibrationValues(int EEPROMSpace) {
      String EEPROM_Map = utilLib.readStringFromEEPROM(EEPROMSpace);
      Pedal::setCalibrationValues(EEPROM_Map, EEPROMSpace);
    }

    void setCalibrationValues(String map, int EEPROMSpace) {
      _calibration[0] = long(utilLib.getValue(map, '-', 0).toInt());
      _calibration[1] = long(utilLib.getValue(map, '-', 1).toInt());
      _calibration[2] = long(utilLib.getValue(map, '-', 2).toInt());
      _calibration[3] = long(utilLib.getValue(map, '-', 3).toInt());

      // update EEPROM settings
      utilLib.writeStringToEEPROM(EEPROMSpace, map);
    }

    String getCalibrationValues(String prefix) {
      return prefix + utilLib.generateStringMapCali(_calibration);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////

    void getEEPROMOutputMapValues(int EEPROMSpace) {
      String EEPROM_Map = utilLib.readStringFromEEPROM(EEPROMSpace);
      Pedal::setOutputMapValues(EEPROM_Map, EEPROMSpace);
    }

    void setOutputMapValues(String map, int EEPROMSpace) {
      _outputMap[0] = utilLib.getValue(map, '-', 0).toInt();
      _outputMap[1] = utilLib.getValue(map, '-', 1).toInt();
      _outputMap[2] = utilLib.getValue(map, '-', 2).toInt();
      _outputMap[3] = utilLib.getValue(map, '-', 3).toInt();
      _outputMap[4] = utilLib.getValue(map, '-', 4).toInt();
      _outputMap[5] = utilLib.getValue(map, '-', 5).toInt();

      // update EEPROM settings
      utilLib.writeStringToEEPROM(EEPROMSpace, map);
    }

    String getOutputMapValues(String prefix) {
      return prefix + utilLib.generateStringMap(_outputMap);
    }

    void resetOutputMapValues(int EEPROMSpace) {
      long resetMap[6] = {0, 20, 40, 60, 80, 100};
      utilLib.writeStringToEEPROM(EEPROMSpace, utilLib.generateStringMap(resetMap));
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////

  private:
    String _prefix;
    String _pedalString;
    long _raw_bit = 65535;
    long _hid_bit = 65535;
    long _serial_range = 100;
    long _afterHID;

    // Create a new exponential filter with a weight of 10 and initial value of 0. 
    
    HX711 _loadCell;
    Smoothed<long> pedalFilter;
    int _loadcell_gain = 128;
    int _loadcell_tare_reps = 10;
    long _loadcell_max_val = 16777215; //24bit
    int _loadcell_sensitivity = 64; //Medium = 64, High = 128;

    int _inverted = 0; //0 = false / 1 - true
    int _smooth = 0; //0 = false / 1 - true
    long _inputMap[6] =  { 0, 20, 40, 60, 80, 100 };
    long _outputMap[6] = { 0, 20, 40, 60, 80, 100 };
    long _calibration[4] = {0, _raw_bit, 0, _raw_bit}; // calibration low, calibration high, deadzone low, deadzone high


    void updatePedal(long rawValue) {
      long beforeSerial;
      long afterSerial;
      long beforeHID;
      long afterHID;

      ////////////////////////////////////////////////////////////////////////////////

      if(_smooth > 0){
        pedalFilter.add(rawValue);
        rawValue = pedalFilter.get();
      }

      if (_inverted == 1) {
        rawValue = _raw_bit - rawValue;
      }

      long pedalOutput;
      long lowDeadzone = (_calibration[0] > _calibration[2]) ? _calibration[0] : _calibration[2];
      long topDeadzone = (_calibration[1] < _calibration[3]) ? _calibration[1] : _calibration[3];

      if (rawValue > topDeadzone) {
        pedalOutput = topDeadzone;
      } else if (rawValue < lowDeadzone) {
        pedalOutput = lowDeadzone;
      } else {
        pedalOutput = rawValue;
      }

      long inputMapHID[6] = {};
      utilLib.copyArray(_inputMap, inputMapHID, 6);
      utilLib.arrayMapMultiplier(inputMapHID, (_hid_bit / 100));

      long outputMapHID[6] = {};
      utilLib.copyArray(_outputMap, outputMapHID, 6);
      utilLib.arrayMapMultiplier(outputMapHID, (_hid_bit / 100));

      beforeHID = utilLib.scaleMap(pedalOutput, lowDeadzone, topDeadzone, 0, _hid_bit);
      afterHID = utilLib.scaleMultiMap(beforeHID, inputMapHID, outputMapHID, 6);

      beforeSerial = utilLib.scaleMap(pedalOutput, lowDeadzone, topDeadzone, 0, _serial_range);
      afterSerial = utilLib.scaleMultiMap(beforeSerial, _inputMap, _outputMap, 6);

      ////////////////////////////////////////////////////////////////////////////////

      String p1 = ";";
      String cm = ",";
      String stringPrefix = _prefix;
      String stringValues = beforeSerial + p1 + afterSerial + p1 + rawValue + p1 + beforeHID + cm;
      String pedalString = stringPrefix + stringValues;

      _pedalString = pedalString;
      _afterHID = afterHID;
    }

};

#endif
