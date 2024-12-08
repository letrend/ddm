/*
    DDM.C - library for digital diagnostic monitoring - implementation
    Copyright (c) 2017 OAKKAR7. All rights reserved.
    
    By      : oakkar7, Myanmar
    blog    : okelectronic.wordpress.com
    code    : https://github.com/oakkar7
*/

#include "ddm.h"
#include <Wire.h>
#include <math.h>

const byte EEPROM_ID = 0x51;
byte DDM[128]; // All DDM message
float _temp_t;
float _volts;
float _optical_tx;
float _optical_rx;

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

// Get RX power
float ddm::getRX() {
        float _optical_rx2 = (DDM[104] << 8 | DDM[105]) * 0.0001;
        _optical_rx = 10 * log10(_optical_rx2);
        return _optical_rx;
}

// Get TX power
float ddm::getTX() {
        float _optical_tx1 = (DDM[102] << 8 | DDM[103]);
        float _optical_tx2 = _optical_tx1 * 0.0001;
        _optical_tx = 10 * log10(_optical_tx2);
        return _optical_tx;
}

// Get temperature
float ddm::getTemp() {
        _temp_t = DDM[96] + (float)DDM[97] / 256;
        return _temp_t;
}

// Get voltages
float ddm::getVolts() {
        _volts = (DDM[98] << 8 | DDM[99]) * 0.0001;
        return _volts;
}

// Check warning status
void ddm::getALM(boolean & _temp_hi, boolean & _temp_low, boolean & _vcc_hi, boolean & _vcc_low, boolean & _tx_hi, boolean & _tx_low, boolean & _rx_hi, boolean & _rx_low) {
        _temp_hi = bitRead(DDM[112], 7);
        _temp_low = bitRead(DDM[112], 6);
        _vcc_hi = bitRead(DDM[112], 5);
        _vcc_low = bitRead(DDM[112], 4);
        _tx_hi = bitRead(DDM[112], 1);
        _tx_low = bitRead(DDM[112], 0);
        _rx_hi = bitRead(DDM[113], 7);
        _rx_low = bitRead(DDM[113], 6);

        Serial.print("TEMP_HI: ");
        Serial.println(_temp_hi);
        Serial.print("VCC_LOW: ");
        Serial.println(_vcc_low);
        Serial.print("TX_LOW: ");
        Serial.println(_tx_low);
        Serial.print("RX_LOW: ");
        Serial.println(_rx_low);
}

// Get warning bits
void ddm::getWARN(boolean & _temp_hi_warn, boolean & _temp_low_warn, boolean & _vcc_hi_warn, boolean & _vcc_low_warn, boolean & _tx_hi_warn, boolean & _tx_low_warn, boolean & _rx_hi_warn, boolean & _rx_low_warn) {
        _temp_hi_warn = bitRead(DDM[116], 7);
        _temp_low_warn = bitRead(DDM[116], 6);
        _vcc_hi_warn = bitRead(DDM[116], 5);
        _vcc_low_warn = bitRead(DDM[116], 4);
        _tx_hi_warn = bitRead(DDM[116], 1);
        _tx_low_warn = bitRead(DDM[116], 0);
        _rx_hi_warn = bitRead(DDM[117], 7);
        _rx_low_warn = bitRead(DDM[117], 6);

        Serial.println("");
        Serial.print("TEMP_HI_WARN: ");
        Serial.println(_temp_hi_warn);
        Serial.print("VCC_LOW_WARN: ");
        Serial.println(_vcc_low_warn);
        Serial.print("TX_LOW_WARN: ");
        Serial.println(_tx_low_warn);
        Serial.print("RX_LOW_WARN: ");
        Serial.println(_rx_low_warn);
}

// Calibrate RX power
uint16_t ddm::calibrateRXpower() {
        float temp = 0;
        int _optical_rx_raw = (DDM[104] << 8 | DDM[105]);

        temp = (DDM[56] << 24 | DDM[57] << 16 | DDM[58] << 8 | DDM[59]) * _optical_rx_raw;
        temp += (DDM[60] << 24 | DDM[61] << 16 | DDM[62] << 8 | DDM[63]) * _optical_rx_raw;
        temp += (DDM[64] << 24 | DDM[65] << 16 | DDM[66] << 8 | DDM[67]) * _optical_rx_raw;
        temp += (DDM[68] << 24 | DDM[69] << 16 | DDM[70] << 8 | DDM[71]) * _optical_rx_raw;
        temp += (DDM[72] << 24 | DDM[73] << 16 | DDM[74] << 8 | DDM[75]);

        Serial.print("RXcalibrated: ");
        Serial.println(temp);

        return (int16_t)temp;
}

// Debug message/data
void ddm::debugDDM() {
        Serial.print("DDM Calibration Data 56:92 (HEX)> ");
        for (int i = 56; i < 93; i++) {
                Serial.print(DDM[i]);
                Serial.print(" ");
        }
        Serial.println(" ");

        Serial.print("DDM Data 96:105 (HEX)> ");
        for (int i = 96; i < 106; i++) {
                Serial.print(DDM[i]);
                Serial.print(" ");
        }
        Serial.println(" ");

        Serial.print("DDM Data 96:105 (BIN)> ");
        for (int i = 96; i < 106; i++) {
                Serial.print(DDM[i], BIN);
                Serial.print(" ");
        }
        Serial.println(" ");

        int _optical_rx_raw = (DDM[104] << 8 | DDM[105]);
        Serial.print("_optical_rx_raw : ");
        Serial.println(_optical_rx_raw, HEX);

        Serial.print("TX dbm : ");
        Serial.println(_optical_tx);
        Serial.print("RX dbm : ");
        Serial.println(_optical_rx);
        Serial.println(" ");
}

// DDM Alarms and Controls Registers
void ddm::DDM_All_Read() {
        for (int i = 56; i < 117; i++) {
                DDM[i] = DDM_Read(i);
        }
}

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

// DDM Read Registers
uint8_t ddm::DDM_Read(uint8_t address) {
        byte data;
        Wire.beginTransmission(EEPROM_ID);
        Wire.write(address);
        Wire.endTransmission();
        Wire.requestFrom(EEPROM_ID, (byte)1);
        data = Wire.read();
        return data;
}
