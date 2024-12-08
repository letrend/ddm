/*
    ddm.h - Test library for digital diagnostic monitoring - description
    Copyright (c) 2017 OAKKAR7.  All rights reserved.

    By      : oakkar7, Myanmar
    blog    : okelectronic.wordpress.com
    code    : https://github.com/oakkar7
*/

// ensure this library description is only included once
#ifndef DDM_H
#define DDM_H

// include types & constants of Wiring core API
#include <Arduino.h>

// library interface description
class ddm
{
    public:
        float getRX();
        float getTX();
        float getTemp();
        float getVolts();
        void getALM(boolean &temp_hi, boolean &temp_low, boolean &vcc_hi, boolean &vcc_low, boolean &tx_hi, boolean &tx_low, boolean &rx_hi, boolean &rx_low);
        void getWARN(boolean &temp_hi_warn, boolean &temp_low_warn, boolean &vcc_hi_warn, boolean &vcc_low_warn, boolean &tx_hi_warn, boolean &tx_low_warn, boolean &rx_hi_warn, boolean &rx_low_warn);
        uint16_t calibrateRXpower();
        void getINFO();
        void debugDDM();    
        void DDM_All_Read();

    private:
        uint8_t DDM_Read(uint8_t address);
};

#endif
