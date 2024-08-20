#include "Calibration.hpp"

#define CALIB_MAGIC 0xAABBCCDD
static const char eeprom_device[]  = "/sys/bus/i2c/devices/0-0050/eeprom";
static const int  eeprom_calib_off = 0x0008;

Calibration::Calibration()
{

}

Calibration::~Calibration()
{
}

int Calibration::Init()
{
    return 0;
}

int Calibration::Release()
{
    return 0;
}
