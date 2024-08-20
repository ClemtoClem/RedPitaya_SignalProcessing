#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <cstdint>

const uint32_t ADC_BUFFER_SIZE = 16 * 1024; // 16k samples
const uint32_t DAC_BUFFER_SIZE = 16 * 1024; // 16k samples

// unmasked IO read/write (p - pointer, v - value)
#define ioread32(p) (*(volatile uint32_t *)(p))
#define iowrite32(v,p) (*(volatile uint32_t *)(p) = (v))

/**
 * Type representing digital input output pins.
 */
enum class RpDigitalPin {
	LED0,       // LED 0
	LED1,       // LED 1
	LED2,       // LED 2
	LED3,       // LED 3
	LED4,       // LED 4
	LED5,       // LED 5
	LED6,       // LED 6
	LED7,       // LED 7
	DIO0_P,     // DIO_P 0
	DIO1_P,     // DIO_P 1
	DIO2_P,     // DIO_P 2
	DIO3_P,     // DIO_P 3
	DIO4_P,     // DIO_P 4
	DIO5_P,     // DIO_P 5
	DIO6_P,     // DIO_P 6
	DIO7_P,	   // DIO_P 7
	DIO0_N,     // DIO_N 0
	DIO1_N,     // DIO_N 1
	DIO2_N,     // DIO_N 2
	DIO3_N,     // DIO_N 3
	DIO4_N,     // DIO_N 4
	DIO5_N,     // DIO_N 5
	DIO6_N,     // DIO_N 6
	DIO7_N      // DIO_N 7
};

/**
 * Type representing pin's high or low state (on/off).
 */
enum class RpPinState {
	Low,		// Low state 1:1
	High		// High state 1:20
};

/**
 * Type representing pin's input or output direction.
 */
enum class RpPinDirection {
    Input,		// Input direction
    Output		// Output direction
};

/**
 * Type representing pin's high or low state (on/off).
 */
enum class RpOutputTriggerMode {
    ADC = 0,	// ADC trigger
    DAC = 1		// DAC trigger
};

/**
 * Type representing analog input output pins.
 */
enum class RpAnalogPin {
    AOUT0,      // Analog output 0
    AOUT1,      // Analog output 1
    AOUT2,      // Analog output 2
    AOUT3,      // Analog output 3
    AIN0,       // Analog input 0
    AIN1,       // Analog input 1
    AIN2,       // Analog input 2
    AIN3        // Analog input 3
};

/**
 * Type representing Input/Output channels.
 */
enum class RpChannel {
	Channel1 = 0,
	Channel2 = 1,
	Channel3 = 2,
	Channel4 = 3
};

enum class RpChannelGain {
	Lower,	// Low state 1:1
	High	// High state 1:20
};

enum class RpWaveform {
	Sine,			// Wave form sine
	Cosine,			// Wave form cosine
	Square,			// Wave form square
	Triangle,		// Wave form triangle
	RampUp,			// Wave form sawtooth (/|)
	RampDown,		// Wave form reversed sawtooth (|\)
	DC,				// Wave form dc
	NegativeDC,		// Wave form negative dc
	PWM,			// Wave form pwm
	Arbitrary,		// Use defined wave form
	Sweep			// Wave form sweep
};

enum class RpADCMode {
	AC, // AC mode
	DC, // DC mode
};

/**
 * Type representing acquire signal sampling rate.
 */
enum class RpADCSampleRate {
	sampling_125M,		// Sample rate 125Msps; Buffer time length 131us; Decimation 1
	sampling_15_625M,	// Sample rate 15.625Msps; Buffer time length 1.048ms; Decimation 8
	sampling_1_953M,	// Sample rate 1.953Msps; Buffer time length 8.388ms; Decimation 64
	sampling_122_070K,	// Sample rate 122.070ksps; Buffer time length 134.2ms; Decimation 1024
	sampling_15_258K,	// Sample rate 15.258ksps; Buffer time length 1.073s; Decimation 8192
	sampling_1_907K		// Sample rate 1.907ksps; Buffer time length 8.589s; Decimation 65536
};

inline uint32_t RpADCSampleRateToFactor(RpADCSampleRate rate) {
	switch (rate) {
		case RpADCSampleRate::sampling_125M:
			return 125000000;
		case RpADCSampleRate::sampling_15_625M:
			return  15625000;
		case RpADCSampleRate::sampling_1_953M:
			return   1953000;
		case RpADCSampleRate::sampling_122_070K:
			return    122070;
		case RpADCSampleRate::sampling_15_258K:
			return     15258;
		case RpADCSampleRate::sampling_1_907K:
			return      1907;
		default:
			return 1;
	}
}

inline RpADCSampleRate SamplingRateFactorToEnum(uint32_t samplingRateFactor) {
	if (samplingRateFactor == 125000000) {
		return RpADCSampleRate::sampling_125M;
	} else if (samplingRateFactor == 15625000) {
		return RpADCSampleRate::sampling_15_625M;
	} else if (samplingRateFactor == 1953000) {
		return RpADCSampleRate::sampling_1_953M;
	} else if (samplingRateFactor == 122070) {
		return RpADCSampleRate::sampling_122_070K;
	} else if (samplingRateFactor == 15258) {
		return RpADCSampleRate::sampling_15_258K;
	} else if (samplingRateFactor == 1907) {
		return RpADCSampleRate::sampling_1_907K;
	} else {
		return RpADCSampleRate::sampling_125M;
	}
}

enum class RpDecimation {
	x1      = 1,
	x2      = 2,
	x4      = 4,
	x8      = 8,
	x16     = 16,
	x32     = 32,
	x64     = 64,
	x128    = 128,
	x256    = 256,
	x512    = 512,
	x1024   = 1024,
	x2048   = 2048,
	x4096   = 4096,
	x8192   = 8192,
	x16384  = 16384,
};

inline uint32_t DecimationEnumToFactor(RpDecimation decimation) {
	return static_cast<uint32_t>(decimation);
}

inline RpDecimation DecimationFactorToEnum(uint32_t decimationFactor) {
	return static_cast<RpDecimation>(decimationFactor);
}

#endif /* UTILS_HPP */