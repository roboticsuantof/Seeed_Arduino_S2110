#ifndef _SENSOR_LOUDNESS_H
#define _SENSOR_LOUDNESS_H

#include "sensorClass.hpp"
#include "Loudness.h"

class sensorLoudness : public sensorClass
{
public:
    sensorLoudness(uint8_t pin = SENSOR_DIGITAL_PIN) : sensorClass("Loudness")
    {
        loudness = new Loudness(pin);
    };
    ~sensorLoudness(){};
    uint16_t init(uint16_t reg, bool i2c_available);
    virtual bool connected();
    virtual bool sample();

    enum
    {
        LOUDNESS_CM = 0x00,
        MAX
    };

private:
    Loudness *loudness;
};

uint16_t sensorLoudness::init(uint16_t reg, bool i2c_available)
{
    uint16_t t_reg = reg;
    for (uint16_t i = 0; i < sensorLoudness::MAX; i++)
    {
        sensorClass::reg_t value;
        value.addr = t_reg;
        value.type = sensorClass::regType_t::REG_TYPE_S32_ABCD;
        value.value.s32 = 0;
        t_reg += sensorClass::valueLength(value.type);
        m_valueVector.emplace_back(value);
    }

    _connected = i2c_available ? false : true;
    
    return t_reg - reg;
    ;
}

bool sensorLoudness::sample()
{
    GROVE_SWITCH_ADC;

    int value = (int)loudness->MeasureInCentimeters();
    m_valueVector[LOUDNESS_CM].value.s32 = value * SCALE;

    return true;
}

bool sensorLoudness::connected()
{
    return _connected;
}

#endif