#include "RedPitaya.hpp"
#include "rp_hw-profiles.h"

RedPitaya::RedPitaya() : _version(1, 0, 0, 1), _isInitialized(false), _api_state(0) {
    LOG(LogLevel::Info, "RedPitaya C++ library version %s", _version.toString().c_str());
}

RedPitaya::~RedPitaya() {
}

// Initialize the Red Pitaya
bool RedPitaya::Init(bool reset) {
    if (!_isInitialized) {
        _mutex.lock();

        _memDev = std::make_shared<MemoryDevice>();
        if (!_memDev->Init()) {
            LOG(LogLevel::Fatal, "Failed to initialize memory device");
            return false;
        }

        _calib = std::make_shared<Calibration>();
        if (!_calib->Init()) {
            LOG(LogLevel::Fatal, "Failed to initialize calibration");
            return false;
        }

        _houseKeeping = std::make_shared<HouseKeeping>(_memDev);
        if (!_houseKeeping->Init()) {
            LOG(LogLevel::Fatal, "Failed to initialize housekeeping");
            return false;
        }

        _ams = std::make_shared<AnalogMixedSignal>(_memDev);
        if (!_ams->Init()) {
            LOG(LogLevel::Fatal, "Failed to initialize assembly");
            return false;
        }

        _daisy = std::make_shared<Daisy>();
        if (!_daisy->Init()) {
            LOG(LogLevel::Fatal, "Failed to initialize daisy");
            return false;
        }

        if (HasFastDAC()) {
            _fastDAC = std::make_shared<FastDAC>();
            if (!_fastDAC->Init()) {
                LOG(LogLevel::Fatal, "Failed to initialize FastDAC");
                return false;
            }
        }

        if (HasFastADC()) {
            _fastADC = std::make_shared<FastADC>();
            if (!_fastADC->Init()) {
                LOG(LogLevel::Fatal, "Failed to initialize FastADC");
                return false;
            }
        }

        if (reset) {
            if (!Reset()) {
                LOG(LogLevel::Fatal, "Failed to reset the Red Pitaya");
                return false;
            }
        }

        _isInitialized = true;
        _mutex.unlock();
    }
    return _isInitialized;    
}

bool RedPitaya::Reset() {
    if (!DigitalPinReset()) {
        LOG(LogLevel::Error, "Failed to reset Digital Pins");
        return false;
    }

    if (!AnalogPinReset()) {
        LOG(LogLevel::Error, "Failed to reset Analog Pins");
        return false;
    }

    if (!FastDACReset()) {
        LOG(LogLevel::Error, "Failed to reset Fast DAC");
        return false;
    }

    if (!FastADCReset()) {
        LOG(LogLevel::Error, "Failed to reset Fast ADC");
        return false;
    }
    return false;
}

bool RedPitaya::Release() {
    _mutex.lock();

    bool ret = true;

    if (!_fastDAC->Release()) {
        LOG(LogLevel::Error, "Failed to release Fast DAC");
        ret = false;
    }
    if(!_fastADC->Release()) {
        LOG(LogLevel::Error, "Failed to release Fast ADC");
        ret = false;
    }
    if (!_ams->Release()) {
        LOG(LogLevel::Error, "Failed to release assembly");
        ret = false;
    }
    if (!_houseKeeping->Release()) {
        LOG(LogLevel::Error, "Failed to release housekeeping");
        ret = false;
    }
    if (!_daisy->Release()) {
        LOG(LogLevel::Error, "Failed to release daisy");
        ret = false;
    }
    if (!_calib->Release()) {
        LOG(LogLevel::Error, "Failed to release calibration");
        ret = false;
    }
    if (!_memDev->Release()) {
        LOG(LogLevel::Error, "Failed to release memory device");
        ret = false;
    }

    _isInitialized = false;
    _mutex.unlock();
    
    return ret;
}

bool RedPitaya::IsInitialized() const {
    return _isInitialized;
}

const Version &RedPitaya::GetVersion() const {
    return _version;
}

uint32_t RedPitaya::GetID() const {
    return _houseKeeping->GetID();
}

uint64_t RedPitaya::GetDNA() const {
    return _houseKeeping->GetDNA();
}

bool RedPitaya::DigitalPinReset() {
    return _houseKeeping->DigitalPinReset();
}

bool RedPitaya::SetLEDState(uint32_t state) {
    return _houseKeeping->SetLEDState(state);
}

uint32_t RedPitaya::GetLEDState() const {
    return _houseKeeping->GetLEDState();
}

bool RedPitaya::SetGPIOPinDirection(uint32_t direction) {
    return _houseKeeping->SetGPIOPinDirection(direction);
}

uint32_t RedPitaya::GetGPIOPinDirection() const {
    return _houseKeeping->GetGPIOPinDirection();
}

bool RedPitaya::SetGPIOPinState(uint32_t state) {
    return _houseKeeping->SetGPIOPinState(state);
}

uint32_t RedPitaya::GetGPIOPinState() const {
    return _houseKeeping->GetGPIOPinState();
}

bool RedPitaya::SetDigitalPinDirection(RpDigitalPin pin, RpPinDirection direction) {
    uint32_t tmp;
    uint32_t pin_ = static_cast<uint32_t>(pin);
    if (pin < RpDigitalPin::DIO0_P) {
        // LEDS
        if (direction == RpPinDirection::Output) return true;
        else {
            LOG_CODE(LogLevel::Error, CodeError::ELID);
        }
    } else if (pin < RpDigitalPin::DIO0_N) {
        // DIO_P
        tmp = _houseKeeping->GetGPIOPinDirection();
        pin_ -= static_cast<uint32_t>(RpDigitalPin::DIO0_P);
        tmp &= ~(1 << pin_);
        tmp |= (direction << pin_) & (1 << pin_);
        return _houseKeeping->SetGPIOPinDirection(tmp);
    } else {
        // DIO_N
        tmp = _houseKeeping->GetGPIOPinDirection();
        pin_ -= static_cast<uint32_t>(RpDigitalPin::DIO0_N);
        tmp &= ~(1 << pin_);
        tmp |= (direction << pin_) & (1 << pin_);
        return _houseKeeping->SetGPIOPinDirection(tmp);
    }
}

RpPinDirection RedPitaya::GetDigitalPinDirection(RpDigitalPin pin) const {
    uint32_t tmp;
    uint32_t pin_ = static_cast<uint32_t>(pin);
    if (pin < RpDigitalPin::DIO0_P) {
        // LEDS
        return RpPinDirection::Input;
    } else if (pin < RpDigitalPin::DIO0_N) {
        // DIO_P
        tmp = _houseKeeping->GetGPIOPinDirection();
        pin_ -= static_cast<uint32_t>(RpDigitalPin::DIO0_P);
        return (tmp & (1 << pin_)) ? RpPinDirection::Output : RpPinDirection::Input;
    } else {
        // DIO_N
        tmp = _houseKeeping->GetGPIOPinDirection();
        pin_ -= static_cast<uint32_t>(RpDigitalPin::DIO0_N);
        return (tmp & (1 << pin_)) ? RpPinDirection::Output : RpPinDirection::Input;
    }
}

bool RedPitaya::SetDigitalPinState(RpDigitalPin pin, RpPinState state) {
    uint32_t tmp;
    uint32_t pin_ = static_cast<uint32_t>(pin);
    RpPinDirection direction = GetDigitalPinDirection(pin);
    if (direction == RpPinDirection::Output) {
        if (pin < RpDigitalPin::DIO0_P) {
            // LEDS
            tmp = _houseKeeping->GetLEDState();
            tmp &= ~(1 << pin_);
            tmp |= (state << pin_) & (1 << pin_);
            return _houseKeeping->SetLEDState(tmp);
        } else if (pin < RpDigitalPin::DIO0_N) {
            // DIO_P
            tmp = _houseKeeping->GetGPIOPinState();
            pin_ -= static_cast<uint32_t>(RpDigitalPin::DIO0_P);
            tmp &= ~(1 << pin_);
            tmp |= (state << pin_) & (1 << pin_);
            return _houseKeeping->SetGPIOPinState(tmp);
        } else {
            // DIO_N
            tmp = _houseKeeping->GetGPIOPinState();
            pin_ -= static_cast<uint32_t>(RpDigitalPin::DIO0_N);
            tmp &= ~(1 << pin_);
            tmp |= (state << pin_) & (1 << pin_);
            return _houseKeeping->SetGPIOPinState(tmp);
        }
    } else {
        LOG_CODE(LogLevel::Error, CodeError::EWID);
    }
}

RpPinState RedPitaya::GetDigitalPinState(RpDigitalPin pin) const {
    uint32_t tmp;
    uint32_t pin_ = static_cast<uint32_t>(pin);
    if (pin < RpDigitalPin::DIO0_P) {
        // LEDS
        tmp = _houseKeeping->GetLEDState();
        return (tmp & (1 << pin_)) ? RpPinState::High : RpPinState::Low;
    } else if (pin < RpDigitalPin::DIO0_N) {
        // DIO_P
        tmp = _houseKeeping->GetGPIOPinState();
        pin_ -= static_cast<uint32_t>(RpDigitalPin::DIO0_P);
        return (tmp & (1 << pin_)) ? RpPinState::High : RpPinState::Low;
    } else {
        // DIO_N
        tmp = _houseKeeping->GetGPIOPinState();
        pin_ -= static_cast<uint32_t>(RpDigitalPin::DIO0_N);
        return (tmp & (1 << pin_)) ? RpPinState::High : RpPinState::Low;
    }
}

bool RedPitaya::SetDigitalLoopEnable(bool enable) {
    return _houseKeeping->SetDigitalLoopEnable(enable);
}

bool RedPitaya::AnalogPinReset() {
    // Reset output analog pins
    for (uint32_t pin=static_cast<uint32_t>(RpAnalogPin::AOUT0); pin<=static_cast<uint32_t>(RpAnalogPin::AOUT3); pin++) {
        if (!SetAnalogPinRawValue(static_cast<RpAnalogPin>(pin), 0U)) {
            LOG(LogLevel::Error, "Failed to reset analog pin " + std::to_string(pin));
            return false;
        }
    }
    return true;
}

bool RedPitaya::IsOutputAnalogPin(RpAnalogPin pin) const {
    return (pin >= RpAnalogPin::AOUT0 && pin <= RpAnalogPin::AOUT3);
}

bool RedPitaya::IsInputAnalogPin(RpAnalogPin pin) const {
    return (pin >= RpAnalogPin::AIN0 && pin <= RpAnalogPin::AIN3);
}

bool RedPitaya::SetAnalogPinValue(RpAnalogPin pin, float value) {
}

float RedPitaya::GetAnalogPinValue(RpAnalogPin pin) const {
    return 0.0f;
}

bool RedPitaya::SetAnalogPinRawValue(RpAnalogPin pin, uint32_t value) {
    if (IsOutputAnalogPin(pin)) {
        if (value < 0.0f) value = 0.0f;
        else if (value > 1.0f) value = 1.0f;

        RpChannel rp_channel;
        switch (pin) {
            case RpAnalogPin::AOUT0:
            case RpAnalogPin::AOUT0:
                rp_channel = RpChannel::Channel1;
            case RpAnalogPin::AIN1:
            case RpAnalogPin::AOUT1:
                rp_channel = RpChannel::Channel2;
            case RpAnalogPin::AIN2:
            case RpAnalogPin::AOUT2:
                rp_channel = RpChannel::Channel3;
            case RpAnalogPin::AIN3:
            case RpAnalogPin::AOUT3:
                rp_channel = RpChannel::Channel4;
        }

        uint8_t bits = 0;
        if (rp_HPGetSlowADCBits(static_cast<int>(rp_channel), &bits) != RP_HP_OK){
            LOG_CODE(LogLevel::Error, CodeError::EOOR);
            return false;
        }
        uint32_t max_value = (1 << bits);
        uint32_t mask = max_value - 1;
        if (value >= max_value) {
            LOG_CODE(LogLevel::Error, CodeError::EOOR);
        }
        iowrite32((value & mask) << 16, &_ams->GetDAC(pin));
    } else {
        LOG_CODE(LogLevel::Error, CodeError::EPN);
    }
    return false;
}

uint32_t RedPitaya::GetAnalogPinRawValue(RpAnalogPin pin) const
{
    return 0;
}

bool RedPitaya::HasFastDAC() const {
    return rp_HPIsFastDAC_PresentOrDefault();
}

uint8_t RedPitaya::GetFastDACChannelsCount() const {
    return rp_HPGetFastDACChannelsCountOrDefault();
}

bool RedPitaya::FastDACReset() {
    if (HasFastDAC()) {
        if (!_fastDAC->Reset()) {
            LOG(LogLevel::Error, "Failed to reset Fast DAC");
            return false;
        }
        return true;
    }
    return false;
}

bool RedPitaya::HasFastADC() const {
    return (rp_HPGetFastADCChannelsCountOrDefault() > 0);
}

uint8_t RedPitaya::GetFastADCChannelsCount() const {
    return rp_HPGetFastADCChannelsCountOrDefault();
}

bool RedPitaya::FastADCReset() {
    if (HasFastADC()) {
        if (!_fastADC->Reset()) {
            LOG(LogLevel::Error, "Failed to reset Fast ADC");
            return false;
        }
    }
}