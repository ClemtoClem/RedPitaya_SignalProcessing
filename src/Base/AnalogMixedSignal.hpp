#ifndef ANALOG_MIXED_SIGNAL_HPP
#define ANALOG_MIXED_SIGNAL_HPP

#include <cstdint>
#include <memory>
#include "Logger.hpp"
#include "MemoryDevice.hpp"
#include "Utils.hpp"
#include "rp_hw-profiles.h"

// Base Analog Mixed Signals address
static const int ANALOG_MIXED_SIGNALS_BASE_ADDR = 0x00400000;
static const int ANALOG_MIXED_SIGNALS_BASE_SIZE = 0x30;

struct AMSRegistersMap {
    uint32_t aif[4];
    uint32_t reserved[4];
    uint32_t dac[4];
};

class AnalogMixedSignal {
public:
    AnalogMixedSignal(std::shared_ptr<MemoryDevice> memDev);
    ~AnalogMixedSignal();

    bool Init();
    bool Release();
    bool IsInitialized() const { return _isInitialized; }

    uint32_t GetDAC(RpAnalogPin pin) const;

private:
    bool _isInitialized;
    std::shared_ptr<MemoryDevice> _memDev;
    volatile AMSRegistersMap *_registersMap;
}