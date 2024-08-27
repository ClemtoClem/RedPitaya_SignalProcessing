#ifndef __DEMODULATOR_HPP
#define __DEMODULATOR_HPP

#include <iostream>
#include <complex>
#include "Signal.hpp"
#include "Filter.hpp"

class Demodulator {
public:
	Demodulator();
	Demodulator(double freq_filter, double freq_oscillator);

	/**
	 * @brief Set the parameters of the demodulator
	 * @param freq_filter Frequency of the filter
	 * @param freq_oscillator Frequency of the oscillator
	 * @return true if the parameters are valid, false otherwise
	 */
	bool set(double freq_filter, double freq_oscillator);

	/**
	 * @brief Setup the demodulator
	 */
	void setup();

	/**
	 * @brief Demodulate a signal
	 * @param signal Input signal
	 * @param outputAmplitude Output signal containing amplitude of the signal
	 * @param outputPhase Output signal containing phase of the signal
	 * @param rms If true, output amplitude will be the RMS value of the signal
	 */
	void apply(Signal &signal, Signal &outputAmplitude, Signal &outputPhase, bool rms = false);
private:
	double _freqFilter, _freqOscillator;
	IIRFilter _filter;
	bool _isSetup;
};

#endif // __DEMODULATOR_HPP