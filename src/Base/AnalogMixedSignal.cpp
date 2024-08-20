#include "AnalogMixedSignal.hpp"

AnalogMixedSignal::AnalogMixedSignal(std::shared_ptr<MemoryDevice> memDev) : _isInitialized(false), _memDev(memDev), _registersMap(nullptr)
{
}

AnalogMixedSignal::~AnalogMixedSignal() {
    Release();
}

bool AnalogMixedSignal::Init() {
    if (!_isInitialized) {
        if (!_memDev->Map(ANALOG_MIXED_SIGNALS_BASE_SIZE, ANALOG_MIXED_SIGNALS_BASE_ADDR, (void**)&_registersMap)) {
            LOG(LogLevel::Fatal, "Failed to map AMS registers");
            return false;
        }

        _isInitialized = true;
    }
    return _isInitialized;
}

bool AnalogMixedSignal::Release() {
    if (_isInitialized) {
        if (!_memDev->Unmap(ANALOG_MIXED_SIGNALS_BASE_SIZE, (void**)&_registersMap)) {
            LOG(LogLevel::Fatal, "Failed to unmap AMS registers");
            return false;
        }
        _isInitialized = false;
    }
    return true;
}

uint32_t AnalogMixedSignal::GetDAC(RpAnalogPin pin) const {
    return _registersMap->dac[static_cast<uint32_t>(pin)];
}
