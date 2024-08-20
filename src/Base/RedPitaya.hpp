#ifndef RED_PITAYA_HPP
#define RED_PITAYA_HPP

#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <cmath>
#include <thread>
#include <mutex>

#include "Logger.hpp"
#include "Version.hpp"
#include "Utils.hpp"

#include "MemoryDevice.hpp"
#include "Calibration.hpp"
#include "HouseKeeping.hpp"
#include "Asm.hpp"
#include "Daisy.hpp"

class RedPitaya {
public:
    RedPitaya();
    ~RedPitaya();

    bool Init(bool reset = true);
    bool Reset();
    bool Release();
    bool IsInitialized() const;

    const Version &GetVersion() const;

    uint32_t GetID() const;
    uint64_t GetDNA() const;

/* -------- DIGITAL PINS -------- */
    /// @brief Reset the Digital Pins
    /// @return true if the Digital Pins were reset, false otherwise
    bool DigitalPinReset();

    /// @brief Set the state of all the LEDs
    /// @param state The state of the LEDs (0x00 - 0xFF)
    bool SetLEDState(uint32_t state);

    uint32_t GetLEDState() const;

	bool SetGPIOPinDirection(uint32_t direction);

	uint32_t GetGPIOPinDirection() const;

	bool SetGPIOPinState(uint32_t state);

	uint32_t GetGPIOPinState() const;

	bool SetDigitalPinDirection(RpDigitalPin pin, RpPinDirection direction);

    RpPinDirection GetDigitalPinDirection(RpDigitalPin pin) const;

    bool SetDigitalPinState(RpDigitalPin pin, RpPinState state);

    RpPinState GetDigitalPinState(RpDigitalPin pin) const;

    /// @brief Enable/Disable the Digital Loop
    /// @param enable true to enable the Digital Loop, false to disable it
    bool SetDigitalLoopEnable(bool enable);

/* -------- ANALOG PINS -------- */
    /// @brief Reset the Analog Pins
    /// @return true if the Analog Pins were reset, false otherwise
    bool AnalogPinReset();

    bool IsOutputAnalogPin(RpAnalogPin pin) const;

    bool IsInputAnalogPin(RpAnalogPin pin) const;

    bool SetAnalogPinValue(RpAnalogPin pin, float value);

    float GetAnalogPinValue(RpAnalogPin pin) const;

    bool SetAnalogPinRawValue(RpAnalogPin pin, uint32_t value);

    uint32_t GetAnalogPinRawValue(RpAnalogPin pin) const;

/* ---------- FAST ADC --------- */
    /// @brief Check if the Red Pitaya has a Fast DAC
    /// @retval true if the Red Pitaya has a Fast DAC, false otherwise
    bool HasFastDAC() const;

    /// @brief Get the number of channels of the Fast DAC
    /// @retval The number of channels of the Fast DAC
    uint8_t GetFastDACChannelsCount() const;

    /// @brief Reset the Fast DAC
    /// @return true if the Fast DAC was reset, false otherwise
    bool FastDACReset();

/* ---------- FAST ADC --------- */
    /// @brief Check if the Red Pitaya has a Fast ADC
    /// @retval true if the Red Pitaya has a Fast ADC, false otherwise
    bool HasFastADC() const;

    /// @brief Get the number of channels of the Fast ADC
    /// @retval The number of channels of the Fast ADC
    uint8_t GetFastADCChannelsCount() const;

    /// @brief Reset the Fast ADC
    /// @return true if the Fast ADC was reset, false otherwise
    bool FastADCReset();

private:
    Version _version;

    bool _isInitialized; // Flag to check if the Red Pitaya is initialized

    std::mutex _mutex;

    std::shared_ptr<MemoryDevice> _memDev;
    std::shared_ptr<Calibration>  _calib;
    std::shared_ptr<HouseKeeping> _houseKeeping;
    std::shared_ptr<AnalogMixedSignal>     _ams;
    std::shared_ptr<Daisy>        _daisy;
    std::shared_ptr<FastDAC>      _fastDAC;
    std::shared_ptr<FastADC>      _fastADC;
};

#endif /* RED_PITAYA_HPP */