#ifndef __GLOBALS_HPP
#define __GLOBALS_HPP

#include <cstdint>
#include <complex>
#include <iostream>

using complexd = std::complex<double>;

inline const int BITS_PER_SAMPLE = 14;
inline int DECIMATION = 16;
inline int MAX_SAMPLING_FREQUENCY = 125e6;
inline double SAMPLING_FREQUENCY = static_cast<double>(MAX_SAMPLING_FREQUENCY) / DECIMATION;
inline size_t BUFFER_SIZE = 1 << BITS_PER_SAMPLE;
inline const size_t MAX_BUFFER_SIZE = 1 << BITS_PER_SAMPLE;

inline void SetMaxSampleFrequency(int freq) {
	if (MAX_SAMPLING_FREQUENCY <= 0.0) {
		std::cerr << "Sampling frequency must be greater than zero" << std::endl;
		return;
	}
	MAX_SAMPLING_FREQUENCY = freq;
	SAMPLING_FREQUENCY = static_cast<double>(MAX_SAMPLING_FREQUENCY)/DECIMATION;
}

inline void SetDecimation(int decimation) {
	// vérifier que la décimation est une puissance de 2
	if (decimation <= 0 || (decimation & (decimation - 1)) != 0) {
		std::cerr << "Decimation must be a power of 2 and positive" << std::endl;
		return;
	}
	DECIMATION = decimation;
	SAMPLING_FREQUENCY = static_cast<double>(MAX_SAMPLING_FREQUENCY)/DECIMATION;
}

inline void SetBufferSize(size_t size) {
	if (size < 1) BUFFER_SIZE = 1;
	if (size > MAX_BUFFER_SIZE) BUFFER_SIZE = MAX_BUFFER_SIZE;
	else BUFFER_SIZE = size;
}

#endif // __GLOBALS_HPP