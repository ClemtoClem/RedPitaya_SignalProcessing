#ifndef MemoryDevice_HPP
#define MemoryDevice_HPP


#include <iostream>
#include <cstdint>
#include <cstdio>
#include <cmath>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <cstring>
#include <stdexcept>
#include <memory>
#include "rp.h"

class MemoryDevice {
public:
    MemoryDevice();
    ~MemoryDevice();

    bool Init();
    bool Release();

    bool Map(size_t size, size_t offset, void** mapped);
    bool Unmap(size_t size, void** mapped);

    void SetBits(volatile uint32_t* field, uint32_t bits, uint32_t mask);
    void UnsetBits(volatile uint32_t* field, uint32_t bits, uint32_t mask);
    void SetValue(volatile uint32_t* field, uint32_t value, uint32_t mask);
    void SetShiftedValue(volatile uint32_t* field, uint32_t value, uint32_t mask, uint32_t bitsToSetShift);
    void GetValue(volatile uint32_t* field, uint32_t* value, uint32_t mask);
    void GetShiftedValue(volatile uint32_t* field, uint32_t* value, uint32_t mask, uint32_t bitsToSetShift);
    void AreBitsSet(volatile uint32_t field, uint32_t bits, uint32_t mask, bool* result);

    static int intcmp(const void* a, const void* b);
    static int int16cmp(const void* aa, const void* bb);
    static int floatCmp(const void* a, const void* b);

    /**
     * @brief Converts calibration Full scale to volts. Scale is usually read from EPROM calibration parameters.
     * If parameter is 0, a factor 1 is returned -> no scaling.
     * @param[in] fullScaleGain value of full voltage scale
     * @retval Scale in volts
    */
    float CalibFullScaleToVoltage(uint32_t fullScaleGain);

    /**
     * @brief Converts calibration Full scale to volts. Scale is usually read from EPROM calibration parameters.
     * If parameter is 0, a factor 1 is returned -> no scaling.
     * @param[in] fullScaleGain value of full voltage scale
     * @retval Scale in volts
    */
    uint32_t CalibFullScaleFromVoltage(float voltageScale);

    /**
     * @brief Calibrates ADC/DAC/Buffer counts and checks for limits
     * Function is used to publish captured signal data to external world in calibrated +- units.
     * Calculation is based on ADC/DAC inputs and calibrated and user defined DC offsets.
     * @param[in] field_len Number of field (ADC/DAC/Buffer) bits
     * @param[in] cnts Captured Signal Value, expressed in ADC/DAC counts
     * @param[in] calib_dc_off Calibrated DC offset, specified in ADC/DAC counts
     * @retval Calibrated counts
     */
    int32_t CalibCounts(uint32_t field_len, uint32_t cnts, int calib_dc_off);

    /**
    * @brief Converts calibration counts to voltage.
    * @param[in] field_len      number of bits in counter
    * @param[in] calib_cnts     counts
    * @param[in] adc_max_v      max voltage value for ADC (max count value)
    * @param[in] calibScale     Calibration Scale
    * @param[in] user_dc_off    User DC offset
    * @retval Voltage
    */
    float ConvertCalibCountsToVoltage(uint32_t field_len, int32_t calib_cnts, float adc_max_v, float calibScale, float user_dc_off);

    /**
    * @brief Converts counts to voltage.
    * @param[in] field_len      number of bits in counter
    * @param[in] cnts           counts
    * @param[in] adc_max_v      max voltage value for ADC (max count value)
    * @param[in] calibScale     Calibration Scale
    * @param[in] calib_dc_off   Calibration DC Offset
    * @param[in] user_dc_off    User DC Offset
    * @retval Voltage
    */
    float ConvertCountsToVoltage(uint32_t field_len, uint32_t cnts, float adc_max_v, uint32_t calibScale, int calib_dc_off, float user_dc_off);

    /**
    * @brief Converts voltage to counts.
    * @param[in] field_len      number of bits in counter
    * @param[in] voltage        Voltage
    * @param[in] adc_max_v      max voltage value for ADC (max count value)
    * @param[in] calibFS_LO     Full scale calibration
    * @param[in] calib_scale    Calibration Scale
    * @param[in] calib_dc_off   Calibration DC Offset
    * @param[in] user_dc_off    User DC Offset
    * @retval Counts
    */
    uint32_t ConvertVToCounts(uint32_t field_len, float voltage, float adc_max_v, bool calibFS_LO, uint32_t calib_scale, int calib_dc_off, float user_dc_off);

private:
    void validate_bits(uint32_t bits, uint32_t mask) {
        if ((bits & ~mask) != 0) throw std::out_of_range("Bits out of range");
    }

    int fd;
};

#endif /* MemoryDevice_HPP */