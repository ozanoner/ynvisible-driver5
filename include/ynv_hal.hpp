#pragma once

#include "esp_err.h"

namespace ynv
{
namespace driver
{
class HALBase
{
   public:
    HALBase();
    ~HALBase();

    virtual esp_err_t digitalWrite(int pin, bool high, int delay = 10, int comm = MAX_SEGMENT_VOLTAGE) = 0;
    virtual int       analogRead(int pin)                                                              = 0;

    static const int MAX_SEGMENT_VOLTAGE = 1400;  // Maximum voltage to apply on an ECD segment (mV)
    static const int HIGH_PIN_VOLTAGE    = 3300;  // ESP32 pin HIGH value (mV)
};

}  // namespace driver
}  // namespace ynv
