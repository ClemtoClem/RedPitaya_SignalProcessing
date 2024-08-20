#include "Demodulator.hpp"
#include "rp.h"
#include "utils.hpp"

Demodulator::Demodulator() : _freqFilter(0.0), _freqOscillator(0.0), _filter(), _isSetup(false)
{}

Demodulator::Demodulator(double freq_filter, double freq_oscillator) : _freqFilter(freq_filter), _freqOscillator(freq_oscillator), _filter(), _isSetup(false)
{
    if (_filter.set(4, _freqFilter, 0, FilterGabarit::LOW_PASS, AnalogFilter::BUTTERWORTH) == false) {
        throw std::invalid_argument("Error while setting filter");
    }
}

bool Demodulator::set(double freq_filter, double freq_oscillator) {
	_freqFilter = freq_filter;
	_freqOscillator = freq_oscillator;
	return _filter.set(4, _freqFilter, 0, FilterGabarit::LOW_PASS, AnalogFilter::BUTTERWORTH);
}

void Demodulator::setup() {
	_filter.setup();
	_isSetup = true;
}

void Demodulator::apply(Signal &signal, Signal &outputAmplitude, Signal &outputPhase, bool rms) {
	if (_isSetup == false) {
		throw std::invalid_argument("Demodulator not setup");
	}

	Signal sinus(signal.size());
	Signal cosinus(signal.size());
	outputAmplitude.resize(signal.size());
	outputPhase.resize(signal.size());

	sinus.generateWaveform(RP_WAVEFORM_SINE, 1.0, _freqOscillator);
	cosinus.generateWaveform(RP_WAVEFORM_SINE, 1.0, _freqOscillator, M_PI/2.0);

	Signal tempA = signal * sinus;
	tempA = _filter.apply(tempA);

	Signal tempPhi = signal * cosinus;
	tempPhi = _filter.apply(tempPhi);
	
	for (size_t i = 0; i<tempPhi.size(); i++) {
		if (rms) {
			outputAmplitude[i] = std::sqrt(tempA[i]*tempA[i] + tempPhi[i]*tempPhi[i])*sqrt(2.0);
		} else {
			outputAmplitude[i] = std::sqrt(2.0*tempA[i]*tempA[i] + 2.0*tempPhi[i]*tempPhi[i])*sqrt(2.0);
		}
		outputPhase[i] = (tempA[i] != 0.0)? modulo(atan2(tempPhi[i], tempA[i]), 2*M_PI) : 0.0;
	}
}
