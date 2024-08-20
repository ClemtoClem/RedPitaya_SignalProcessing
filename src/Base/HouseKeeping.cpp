#include "HouseKeeping.hpp"
#include "Logger.hpp"
#include "Utils.hpp"

HouseKeeping::HouseKeeping(std::shared_ptr<MemoryDevice> memDev) : _isInitialized(false), _memDev(memDev) {
	_registersMap.v1 = nullptr;
}

HouseKeeping::~HouseKeeping()
{
}

bool HouseKeeping::Init(bool reset) {
	if (!_isInitialized) {  
		rp_HPeModels_t c = STEM_125_14_v1_0;
		if (rp_HPGetModel(&c) != RP_HP_OK){
			LOG(LogLevel::Fatal, "Can't get board model");
			return false;
		}

		switch (c) {
			case STEM_125_10_v1_0:
			case STEM_125_14_v1_0:
			case STEM_125_14_v1_1:
			case STEM_125_14_LN_v1_1:
			case STEM_125_14_Z7020_v1_0:
			case STEM_125_14_Z7020_LN_v1_1:
				_version = HKVersion::V1;
				break;

			case STEM_122_16SDR_v1_0:
			case STEM_122_16SDR_v1_1:
				_version = HKVersion::V1;
				break;

			case STEM_125_14_Z7020_4IN_v1_0:
			case STEM_125_14_Z7020_4IN_v1_2:
			case STEM_125_14_Z7020_4IN_v1_3:
				_version = HKVersion::V2;
				break;

			case STEM_250_12_v1_0:
			case STEM_250_12_v1_1:
			case STEM_250_12_v1_2:
			case STEM_250_12_v1_2a:
			case STEM_250_12_v1_2b:
			case STEM_250_12_120:
				_version = HKVersion::V3;
				break;
			default:
				LOG(LogLevel::Fatal, "Can't get board model");
				return false;
		}

		switch (_version) {
			case HKVersion::V1:
				_memDev->Map(HOUSEKEEPING_BASE_SIZE, HOUSEKEEPING_BASE_ADDR, (void**)&_registersMap.v1);
				break;
			case HKVersion::V2:
				_memDev->Map(HOUSEKEEPING_BASE_SIZE, HOUSEKEEPING_BASE_ADDR, (void**)&_registersMap.v2);
				break;
			case HKVersion::V3:
				_memDev->Map(HOUSEKEEPING_BASE_SIZE, HOUSEKEEPING_BASE_ADDR, (void**)&_registersMap.v3);
				break;
			default:
				LOG(LogLevel::Fatal, "Can't get board model");
				return false;
		}

		if (rp_HPGetIsPLLControlEnableOrDefault() && reset) {
			SetPllControlEnable(false);
		}
	}

	return _isInitialized;
}

bool HouseKeeping::Release() {
	if (_isInitialized) {
		bool ret = true;
		switch (_version) {
			case HKVersion::V1:
				ret = _memDev->Unmap(HOUSEKEEPING_BASE_SIZE, (void**)&_registersMap.v1);
				break;
			case HKVersion::V2:
				ret = _memDev->Unmap(HOUSEKEEPING_BASE_SIZE, (void**)&_registersMap.v2);
				break;
			case HKVersion::V3:
				ret = _memDev->Unmap(HOUSEKEEPING_BASE_SIZE, (void**)&_registersMap.v3);
				break;
			default:
				LOG(LogLevel::Fatal, "Can't get board model");
				return false;
		}
		if (!ret) {
			LOG(LogLevel::Fatal, "Failed to unmap HouseKeeping registers");
		}
		_isInitialized = false;
	}
	return true;
}

bool HouseKeeping::IsInitialized() const {
	return _isInitialized;
}

HKVersion HouseKeeping::GetVersion() const {
   return _version;
}

uint32_t HouseKeeping::GetId() const {
	switch (_version) {
		case HKVersion::V1:
			return ioread32(&(_registersMap.v1)->id);
		case HKVersion::V2:
			return ioread32(&(_registersMap.v2)->id);
		case HKVersion::V3:
			return ioread32(&(_registersMap.v3)->id);
		default:
			LOG_CODE(LogLevel::Fatal, CodeError::NOTS, "Can't get board model");
			return 0U;
	}
}

uint64_t HouseKeeping::GetDNA() const {
	switch (_version) {
		case HKVersion::V1: {
			return (static_cast<uint64_t> (ioread32(&(_registersMap.v1)->dna_hi)) << 32) | (static_cast<uint64_t> (ioread32(&(_registersMap.v1)->dna_lo)) <<  0);
		}
		case HKVersion::V2 : {
			return (static_cast<uint64_t> (ioread32(&(_registersMap.v2)->dna_hi)) << 32) | (static_cast<uint64_t> (ioread32(&(_registersMap.v2)->dna_lo)) <<  0);
		}
		case HKVersion::V3 : {
			return (static_cast<uint64_t> (ioread32(&(_registersMap.v3)->dna_hi)) << 32) | (static_cast<uint64_t> (ioread32(&(_registersMap.v3)->dna_lo)) <<  0);
		}
		default:
			LOG_CODE(LogLevel::Fatal, CodeError::NOTS, "Can't get board model");
			return 0U;
	}
}

bool HouseKeeping::ResetRegistersMap()
{
	return false;
}

void HouseKeeping::SetPllControlEnable(bool enable) {
	if (rp_HPGetIsPLLControlEnableOrDefault()) {
		switch (_version) {
			case HKVersion::V1:
				LOG_CODE(LogLevel::Fatal, CodeError::NOTS, "This model does not support PLL control");
				break;
			case HKVersion::V2:
				iowrite32(enable ? 1 : 0, &(_registersMap.v2)->pll_control.enable);
				break;
			case HKVersion::V3:
				iowrite32(enable ? 1 : 0, &(_registersMap.v3)->pll_control.enable);
				break;
			default:
				LOG_CODE(LogLevel::Fatal, CodeError::NOTS, "Can't get board model");
				break;
		}
	} else {
		LOG(LogLevel::Fatal, CodeError::NOTS, "PLL control is not enabled");
	}
}

bool HouseKeeping::GetPllControlEnable() const {
	if (rp_HPGetIsPLLControlEnableOrDefault()) {
		switch (_version) {
			case HKVersion::V1:
				LOG_CODE(LogLevel::Fatal, CodeError::NOTS, "This model does not support PLL control");
				break;
			case HKVersion::V2:
				return ioread32(&(_registersMap.v2)->pll_control.enable) == 1;
			case HKVersion::V3:
				return ioread32(&(_registersMap.v3)->pll_control.enable) == 1;
			default:
				LOG_CODE(LogLevel::Fatal, CodeError::NOTS, "Can't get board model");
				break;
		}
	} else {
		LOG(LogLevel::Fatal, CodeError::NOTS, "PLL control is not enabled");
	}
	return false;
}

bool HouseKeeping::GetPllControlLocked() const {
    if (rp_HPGetIsPLLControlEnableOrDefault()) {
		switch (_version) {
			case HKVersion::V1:
				LOG_CODE(LogLevel::Fatal, CodeError::NOTS, "This model does not support PLL control");
				break;
			case HKVersion::V2:
				return ioread32(&(_registersMap.v2)->pll_control.locked) == 1;
			case HKVersion::V3:
				return ioread32(&(_registersMap.v3)->pll_control.locked) == 1;
			default:
				LOG_CODE(LogLevel::Fatal, CodeError::NOTS, "Can't get board model");
				break;
		}
	} else {
		LOG(LogLevel::Fatal, CodeError::NOTS, "PLL control is not enabled");
	}
	return false;
}

bool HouseKeeping::SetEnableDaisyChainSync(bool enable) {
	switch (_version) {
		case HKVersion::V1:
			iowrite32(enable ? 1 : 0, &(_registersMap.v1)->ext_trigger.enable);
			return true;
		case HKVersion::V2:
			iowrite32(enable ? 1 : 0, &(_registersMap.v2)->ext_trigger.enable);
			return true;
		case HKVersion::V3:
			iowrite32(enable ? 1 : 0, &(_registersMap.v3)->ext_trigger.enable);
			return true;
		default:
			LOG_CODE(LogLevel::Fatal, CodeError::NOTS, "Can't get board model");
			return false;
	}
}

bool HouseKeeping::GetEnableDaisyChainSync() const {
    switch (_version) {
		case HKVersion::V1:
			return ioread32(&(_registersMap.v1)->ext_trigger.enable) == 1;
		case HKVersion::V2:
			return ioread32(&(_registersMap.v2)->ext_trigger.enable) == 1;
		case HKVersion::V3:
			return ioread32(&(_registersMap.v3)->ext_trigger.enable) == 1;
		default:
			LOG_CODE(LogLevel::Fatal, CodeError::NOTS, "Can't get board model");
			return false;
	}
}

bool HouseKeeping::SetLEDState(uint32_t state) {
	switch (_version) {
		case HKVersion::V1:
			iowrite32(state, &(_registersMap.v1)->led_control);
			return true;
		case HKVersion::V2:
			iowrite32(state, &(_registersMap.v2)->led_control);
			return true;
		case HKVersion::V3:
			iowrite32(state, &(_registersMap.v3)->led_control);
			return true;
		default:
			LOG_CODE(LogLevel::Fatal, CodeError::NOTS, "Can't get board model");
			return false;
	}
}

uint32_t HouseKeeping::GetLEDState() const {
	switch (_version) {
		case HKVersion::V1:
			return ioread32(&(_registersMap.v1)->led_control);
		case HKVersion::V2:
			return ioread32(&(_registersMap.v2)->led_control);
		case HKVersion::V3:
			return ioread32(&(_registersMap.v3)->led_control);
		default:
			LOG_CODE(LogLevel::Fatal, CodeError::NOTS, "Can't get board model");
			return 0U;
	}
}

bool HouseKeeping::SetGPIOPinDirection(uint32_t direction) {
	switch (_version) {
		case HKVersion::V1:
			iowrite32(direction, &(_registersMap.v1)->ex_cd_n);
			return true;
		case HKVersion::V2:
			iowrite32(direction, &(_registersMap.v2)->ex_cd_n);
			return true;
		case HKVersion::V3:
			iowrite32(direction, &(_registersMap.v3)->ex_cd_n);
			return true;
		default:
			LOG_CODE(LogLevel::Fatal, CodeError::NOTS, "Can't get board model");
			return false;
	}
}

uint32_t HouseKeeping::GetGPIOPinDirection() const {
	switch (_version) {
		case HKVersion::V1:
			return ioread32(&(_registersMap.v1)->ex_cd_n);
		case HKVersion::V2:
			return ioread32(&(_registersMap.v2)->ex_cd_n);
		case HKVersion::V3:
			return ioread32(&(_registersMap.v3)->ex_cd_n);
    return false;
		default:
			LOG_CODE(LogLevel::Fatal, CodeError::NOTS, "Can't get board model");
			return 0U;
	}
}

bool HouseKeeping::SetGPIOPinState(uint32_t state) {
	switch (_version) {
		case HKVersion::V1:
			iowrite32(state, &(_registersMap.v1)->ex_co_n);
			return true;
		case HKVersion::V2:
			iowrite32(state, &(_registersMap.v2)->ex_co_n);
			return true;
		case HKVersion::V3:
			iowrite32(state, &(_registersMap.v3)->ex_co_n);
			return true;
		default:
			LOG_CODE(LogLevel::Fatal, CodeError::NOTS, "Can't get board model");
			return false;
	}
}

uint32_t HouseKeeping::GetGPIOPinState() const {
	switch (_version) {
		case HKVersion::V1:
			return ioread32(&(_registersMap.v1)->ex_ci_n);
		case HKVersion::V2:
			return ioread32(&(_registersMap.v2)->ex_ci_n);
		case HKVersion::V3:
			return ioread32(&(_registersMap.v3)->ex_ci_n);
		default:
			LOG_CODE(LogLevel::Fatal, CodeError::NOTS, "Can't get board model");
			return 0U;
	}
}

bool HouseKeeping::DigitalPinReset() {
	switch (_version) {
		case HKVersion::V1:
			iowrite32(0, &(_registersMap.v1)->ex_cd_p);
			iowrite32(0, &(_registersMap.v1)->ex_cd_n);
			iowrite32(0, &(_registersMap.v1)->ex_co_p);
			iowrite32(0, &(_registersMap.v1)->ex_co_n);
			iowrite32(0, &(_registersMap.v1)->led_control);
			iowrite32(0, &(_registersMap.v1)->digital_loop);
			return true;
		case HKVersion::V2:
			iowrite32(0, &(_registersMap.v2)->ex_cd_p);
			iowrite32(0, &(_registersMap.v2)->ex_cd_n);
			iowrite32(0, &(_registersMap.v2)->ex_co_p);
			iowrite32(0, &(_registersMap.v2)->ex_co_n);
			iowrite32(0, &(_registersMap.v2)->led_control);
			iowrite32(0, &(_registersMap.v2)->digital_loop);
			return true;
		case HKVersion::V3:
			iowrite32(0, &(_registersMap.v3)->ex_cd_p); 
			iowrite32(0, &(_registersMap.v3)->ex_cd_n);
			iowrite32(0, &(_registersMap.v3)->ex_co_p);
			iowrite32(0, &(_registersMap.v3)->ex_co_n);
			iowrite32(0, &(_registersMap.v3)->led_control);
			iowrite32(0, &(_registersMap.v3)->digital_loop);
			return true;
		default:
			LOG_CODE(LogLevel::Fatal, CodeError::NOTS, "Can't get board model");
			return false;
	}
}

bool HouseKeeping::SetDigitalLoopEnable(bool enable) {
	switch (_version) {
		case HKVersion::V1:
			iowrite32(enable, &(_registersMap.v1)->digital_loop);
			return true;
		case HKVersion::V2:
			iowrite32(enable, &(_registersMap.v2)->digital_loop);
			return true;
		case HKVersion::V3:
			iowrite32(enable, &(_registersMap.v3)->digital_loop);
			return true;
		default:
			LOG_CODE(LogLevel::Fatal, CodeError::NOTS, "Can't get board model");
			return false;
	}
}

bool HouseKeeping::SetDigitalPinEnableTriggerOutput(bool enable) {
    switch (_version) {
        case HKVersion::V1:
            iowrite32(enable, &(_registersMap.v1)->ext_trigger.gpio_adc_dac);
            return true;
        case HKVersion::V2:
            iowrite32(enable, &(_registersMap.v2)->ext_trigger.gpio_adc_dac);
            return true;
        case HKVersion::V3:
            iowrite32(enable, &(_registersMap.v3)->ext_trigger.gpio_adc_dac);
            return true;
        default:
            LOG_CODE(LogLevel::Fatal, CodeError::NOTS, "Can't get board model");
            return false;
    }
}

bool HouseKeeping::GetDigitalPinEnableTriggerOutput() const {
	switch (_version) {
		case HKVersion::V1:
			return ioread32(&(_registersMap.v1)->ext_trigger.gpio_adc_dac);
		case HKVersion::V2:
			return ioread32(&(_registersMap.v2)->ext_trigger.gpio_adc_dac);
		case HKVersion::V3:
			return ioread32(&(_registersMap.v3)->ext_trigger.gpio_adc_dac);
		default:
			LOG_CODE(LogLevel::Fatal, CodeError::NOTS, "Can't get board model");
			return false;
	}
}

bool HouseKeeping::SetTriggerSource(RpOutputTriggerMode source) {
	switch (_version) {
		case HKVersion::V1:
			iowrite32(static_cast<uint32_t>(source), &(_registersMap.v1)->ext_trigger.out_selector);
			return true;
		case HKVersion::V2:
			iowrite32(static_cast<uint32_t>(source), &(_registersMap.v2)->ext_trigger.out_selector);
			return true;
		case HKVersion::V3:
			iowrite32(static_cast<uint32_t>(source), &(_registersMap.v3)->ext_trigger.out_selector);
			return true;
		default:
			LOG_CODE(LogLevel::Fatal, CodeError::NOTS, "Can't get board model");
			return false;
	}
}

RpOutputTriggerMode HouseKeeping::GetTriggerSource() const {
	switch (_version) {
		case HKVersion::V1:
			return static_cast<RpOutputTriggerMode>(ioread32(&(_registersMap.v1)->ext_trigger.out_selector));
		case HKVersion::V2:
			return static_cast<RpOutputTriggerMode>(ioread32(&(_registersMap.v2)->ext_trigger.out_selector));
		case HKVersion::V3:
			return static_cast<RpOutputTriggerMode>(ioread32(&(_registersMap.v3)->ext_trigger.out_selector));
		default:
			LOG_CODE(LogLevel::Fatal, CodeError::NOTS, "Can't get board model");
			return RpOutputTriggerMode::None;
	}
}

bool HouseKeeping::SetCANModeEnable(bool enable) {
	switch (_version) {
		case HKVersion::V1:
			iowrite32(enable, &(_registersMap.v1)->can_control.enable);
			return true;
		case HKVersion::V2:
			iowrite32(enable, &(_registersMap.v2)->can_control.enable);
			return true;
		case HKVersion::V3:
			iowrite32(enable, &(_registersMap.v3)->can_control.enable);
			return true;
		default:
			LOG_CODE(LogLevel::Fatal, CodeError::NOTS, "Can't get board model");
			return false;
	}
}

bool HouseKeeping::GetCANModeEnable() const {
	switch (_version) {
		case HKVersion::V1:
			return ioread32(&(_registersMap.v1)->can_control.enable);
		case HKVersion::V2:
			return ioread32(&(_registersMap.v2)->can_control.enable);
		case HKVersion::V3:
			return ioread32(&(_registersMap.v3)->can_control.enable);
		default:
			LOG_CODE(LogLevel::Fatal, CodeError::NOTS, "Can't get board model");
			return false;
	}
}
