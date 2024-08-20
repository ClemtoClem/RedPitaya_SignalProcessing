#include "MemoryDevice.hpp"
#include "Utils.hpp"
#include "Logger.hpp"

MemoryDevice::MemoryDevice() : fd(-1) {}

MemoryDevice::~MemoryDevice() {
    Release();
}

bool MemoryDevice::Init() {
    if (fd == -1) {
        if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) {
            LOG_CODE(LogLevel::Fatal, CodeError::EOMD, "Failed to open /dev/mem");
            return false;
        }
    }
    return true;
}

bool MemoryDevice::Release() {
    if (fd != -1) {
        if (close(fd) < 0) {
            LOG_CODE(LogLevel::Fatal, CodeError::ECMD, "Failed to close /dev/mem");
            return ;
        }
        fd = -1;
    }
    return true;
}

bool MemoryDevice::Map(size_t size, size_t offset, void** mapped) {
    if (fd == -1) {
        LOG_CODE(LogLevel::Fatal, CodeError::EMMD);
    }

    *mapped = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset);

    if (*mapped == MAP_FAILED) {
        LOG_CODE(LogLevel::Fatal, CodeError::EMMD);
        return false;
    }

    return true;
}

bool MemoryDevice::Unmap(size_t size, void** mapped) {
    if (fd == -1) {
        LOG_CODE(LogLevel::Fatal, CodeError::EUMD);
        return false;
    }

    if (*mapped == MAP_FAILED || *mapped == nullptr) {
        LOG_CODE(LogLevel::Fatal, CodeError::EUMD);
        return false;
    }

    if (munmap(*mapped, size) < 0) {
        LOG_CODE(LogLevel::Fatal, CodeError::EUMD);
        return false;
    }
    *mapped = nullptr;
    return true;
}

void MemoryDevice::SetShiftedValue(volatile uint32_t* field, uint32_t value, uint32_t mask, uint32_t bitsToSetShift) {
    validate_bits(value, mask);
    uint32_t currentValue;
    GetValue(field, &currentValue, 0xffffffff);
    currentValue &= ~(mask << bitsToSetShift); // Clear all bits at specified location
    currentValue |= (value << bitsToSetShift); // Set value at specified location
    *field = currentValue;
}

void MemoryDevice::SetValue(volatile uint32_t* field, uint32_t value, uint32_t mask) {
    SetShiftedValue(field, value, mask, 0);
}

void MemoryDevice::GetShiftedValue(volatile uint32_t* field, uint32_t* value, uint32_t mask, uint32_t bitsToSetShift) {
    *value = (*field >> bitsToSetShift) & mask;
}

void MemoryDevice::GetValue(volatile uint32_t* field, uint32_t* value, uint32_t mask) {
    GetShiftedValue(field, value, mask, 0);
}

void MemoryDevice::SetBits(volatile uint32_t* field, uint32_t bits, uint32_t mask) {
    validate_bits(bits, mask);
    *field |= bits;
}

void MemoryDevice::UnsetBits(volatile uint32_t* field, uint32_t bits, uint32_t mask) {
    validate_bits(bits, mask);
    *field &= ~bits;
}

void MemoryDevice::AreBitsSet(volatile uint32_t field, uint32_t bits, uint32_t mask, bool* result) {
    validate_bits(bits, mask);
    *result = ((field & bits) == bits);
}

/* 32 bit integer comparator */
int MemoryDevice::intcmp(const void* v1, const void* v2) {
    return (*(int*)v1 - *(int*)v2);
}

/* 16 bit integer comparator */
int MemoryDevice::int16cmp(const void* aa, const void* bb) {
    const int16_t* a = (const int16_t*)aa, * b = (const int16_t*)bb;
    return (*a < *b) ? -1 : (*a > *b);
}

/* Float comparator */
int MemoryDevice::floatCmp(const void* a, const void* b) {
    float fa = *(const float*)a, fb = *(const float*)b;
    return (fa > fb) - (fa < fb);
}

float MemoryDevice::CalibFullScaleToVoltage(uint32_t fullScaleGain) {
    /* no scale */
    if (fullScaleGain == 0) {
        return 1;
    }
    return static_cast<float>(fullScaleGain * 100.0 / static_cast<uint64_t>(1) << 32);
}

uint32_t MemoryDevice::CalibFullScaleFromVoltage(float voltageScale) {
    return static_cast<uint32_t>(voltageScale / 100.0 * (static_cast<uint64_t>(1) << 32));
}

float MemoryDevice::ConvertCalibCountsToVoltage(uint32_t field_len, int32_t calib_cnts, float adc_max_v, float calibScale, float user_dc_off) {
    return (calib_cnts * (adc_max_v / static_cast<float>((1 << field_len) - 1)) / calibScale) + user_dc_off;
}

float MemoryDevice::ConvertCountsToVoltage(uint32_t field_len, uint32_t cnts, float adc_max_v, uint32_t calibScale, int calib_dc_off, float user_dc_off) {
    int32_t calib_cnts = CalibCounts(field_len, cnts, calib_dc_off);
    return ConvertCalibCountsToVoltage(field_len, calib_cnts, adc_max_v, CalibFullScaleToVoltage(calibScale), user_dc_off);
}

uint32_t MemoryDevice::ConvertVToCounts(uint32_t field_len, float voltage, float adc_max_v, bool calibFS_LO, uint32_t calib_scale, int calib_dc_off, float user_dc_off) {
    uint32_t cnt;
    if (calibFS_LO) {
        cnt = static_cast<uint32_t>((((voltage - user_dc_off) * CalibFullScaleToVoltage(calib_scale)) / adc_max_v) * ((1 << field_len) - 1));
    } else {
        cnt = static_cast<uint32_t>((voltage * CalibFullScaleToVoltage(calib_scale) / adc_max_v) * ((1 << field_len) - 1));
    }

    int32_t calib_cnt = cnt + calib_dc_off;
    if (calib_cnt < 0) calib_cnt = 0;
    if (calib_cnt >= static_cast<int32_t>((1 << field_len) - 1)) calib_cnt = static_cast<int32_t>((1 << field_len) - 1);

    return static_cast<uint32_t>(calib_cnt);
}

int32_t MemoryDevice::CalibCounts(uint32_t field_len, uint32_t cnts, int calib_dc_off) {
    return cnts - calib_dc_off;
}