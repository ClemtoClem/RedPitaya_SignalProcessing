#include <limits>
#include <cmath>
#include "Signal.hpp"
#include "Spectrum.hpp"
#include "utils.hpp"

Signal::Signal(const std::string &name) : std::vector<double>(BUFFER_SIZE, 0.0), mName(name) {}

Signal::Signal(size_t size, const std::string &name) : std::vector<double>(size), mName(name) {}

Signal::Signal(const std::vector<double> &values, const std::string &name) : std::vector<double>(values), mName(name) {}

Signal::Signal(const Signal &other) : std::vector<double>(other), mName(other.mName) {}

Signal &Signal::operator=(const Signal &other) {
	if (this != &other) {
		std::vector<double>::operator=(other);
	}
	return *this;
}

/* ------------------------------- */

const std::string &Signal::getName() const {
	return mName;
}

void Signal::setName(const std::string &name) {
	mName = name;
}

bool Signal::hasInfitityValue() const {
	for (size_t i = 0; i < size(); i++) {
		if (((*this)[i] == INFINITY) || ((*this)[i] == -INFINITY)) {
			return true;
		}
	}
	return false;
}

/* ------------------------------- */

Signal &Signal::addValue(double value) {
	std::vector<double>::push_back(value);
	return *this;
}

Signal &Signal::fill(double value)
{
	for (size_t i = 0; i < size(); i++) {
		(*this)[i] = value;
	}
	return *this;
}

/* ------------------------------- */

Signal Signal::operator+(const Signal &other) const {
	Signal output(other.size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = (*this)[i] + other[i];
	}
	return output;
}

Signal Signal::operator-(const Signal &other) const {
	Signal output(other.size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = (*this)[i] - other[i];
	}
	return output;
}

Signal Signal::operator*(const Signal &other) const {
	Signal output(other.size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = (*this)[i] * other[i];
	}
	return output;
}

Signal Signal::operator/(const Signal &other) const {
	Signal output(other.size());
	for (size_t i = 0; i < size(); i++) {
		if (other[i] == 0) output[i] = INFINITY;
		else output[i] = (*this)[i] / other[i];
	}
	return output;
}

Signal &Signal::operator+=(const Signal &other) {
	for (size_t i = 0; i < size(); i++) {
		(*this)[i] += other[i];
	}
	return *this;
}

Signal &Signal::operator-=(const Signal &other) {
	for (size_t i = 0; i < size(); i++) {
		(*this)[i] -= other[i];
	}
	return *this;
}

Signal &Signal::operator*=(const Signal &other) {
	for (size_t i = 0; i < size(); i++) {
		(*this)[i] *= other[i];
	}
	return *this;
}

Signal &Signal::operator/=(const Signal &other) {
	for (size_t i = 0; i < size(); i++) {
		if (other[i] == 0) (*this)[i] = INFINITY;
		else (*this)[i] /= other[i];
	}
	return *this;
}

/* ------------------------------- */

Signal Signal::operator+(double value) const {
	Signal output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = (*this)[i] + value;
	}
	return output;
}

Signal Signal::operator-(double value) const {
	Signal output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = (*this)[i] - value;
	}
	return output;
}

Signal Signal::operator*(double value) const {
	Signal output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = (*this)[i] * value;
	}
	return output;
}

Signal Signal::operator/(double value) const {
	Signal output(size());
	for (size_t i = 0; i < size(); i++) {
		if (value == 0) output[i] = INFINITY;
		else output[i] = (*this)[i] / value;
	}
	return output;
}

Signal &Signal::operator+=(double value) {
	for (size_t i = 0; i < size(); i++) {
		(*this)[i] += value;
	}
	return *this;
}

Signal &Signal::operator-=(double value) {
	for (size_t i = 0; i < size(); i++) {
		(*this)[i] -= value;
	}
	return *this;
}

Signal &Signal::operator*=(double value) {
	for (size_t i = 0; i < size(); i++) {
		(*this)[i] *= value;
	}
	return *this;
}

Signal &Signal::operator/=(double value) {
	for (size_t i = 0; i < size(); i++) {
		if (value == 0) (*this)[i] = INFINITY;
		else (*this)[i] /= value;
	}
	return *this;
}

/* ------------------------------- */

Signal operator+(double value, const Signal &input) {
	Signal output(input.size());
	for (size_t i = 0; i < input.size(); i++) {
		output[i] = input[i] + value;
	}
	return output;
}

Signal operator-(double value, const Signal &input) {
	Signal output(input.size());
	for (size_t i = 0; i < input.size(); i++) {
		output[i] = input[i] - value;
	}
	return output;
}

Signal operator*(double value, const Signal &input) {
	Signal output(input.size());
	for (size_t i = 0; i < input.size(); i++) {
		output[i] = input[i] * value;
	}
	return output;
}

Signal operator/(double value, const Signal &input) {
	Signal output(input.size());
	for (size_t i = 0; i < input.size(); i++) {
		if (value == 0) output[i] = INFINITY;
		else output[i] = input[i] / value;
	}
	return output;
}

/* ------------------------------- */

Signal Signal::cos() const {
	Signal output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::cos((*this)[i]);
	}
	return output;
}

Signal Signal::sin() const {
	Signal output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::sin((*this)[i]);
	}
	return output;
}

Signal Signal::tan() const {
	Signal output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::tan((*this)[i]);
	}
	return output;
}

Signal Signal::cosh() const {
	Signal output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::cosh((*this)[i]);
	}
	return output;
}

Signal Signal::sinh() const {
	Signal output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::sinh((*this)[i]);
	}
	return output;
}

Signal Signal::tanh() const {
	Signal output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::tanh((*this)[i]);
	}
	return output;
}

Signal Signal::acos() const {
	Signal output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::acos((*this)[i]);
	}
	return output;
}

Signal Signal::asin() const {
	Signal output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::asin((*this)[i]);
	}
	return output;
}

Signal Signal::atan() const {
	Signal output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::atan((*this)[i]);
	}
	return output;
}

Signal Signal::acosh() const {
	Signal output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::acosh((*this)[i]);
	}
	return output;
}

Signal Signal::asinh() const {
	Signal output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::asinh((*this)[i]);
	}
	return output;
}

Signal Signal::atanh() const {
	Signal output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::atanh((*this)[i]);
	}
	return output;
}

/* ------------------------------- */

Signal Signal::abs() const {
	Signal output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::abs((*this)[i]);
	}
	return output;
}

Signal Signal::square() const {
	Signal output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = (*this)[i] * (*this)[i];
	}
	return output;
}

Signal Signal::pow(double exponent) const {
	Signal output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::pow((*this)[i], exponent);
	}
	return output;
}

Signal Signal::sqrt() const {
	Signal output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::sqrt((*this)[i]);
	}
	return output;
}

Signal Signal::log() const {
	Signal output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::log((*this)[i]);
	}
	return output;
}

Signal Signal::log2() const {
	Signal output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::log2((*this)[i]);
	}
	return output;
}

Signal Signal::log10() const {
	Signal output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::log10((*this)[i]);
	}
	return output;
}

Signal Signal::exp() const {
	Signal output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::exp((*this)[i]);
	}
	return output;
}

/* ------------------------------- */

double Signal::max(size_t index) const {
	if (this->empty()) {
		return NAN;
	}
	double maxElem = (*this)[index];
	for (size_t i = index; i<this->size(); i++) {
		if (maxElem < (*this)[i]) maxElem = (*this)[i];
	}
	return maxElem;
}

double Signal::min(size_t index) const {
	if (this->empty()) {
		return NAN;
	}
	double minElem = (*this)[index];
	for (size_t i = index; i<this->size(); i++) {
		if (minElem > (*this)[i]) minElem = (*this)[i];
	}
	return minElem;
}

double Signal::mean(size_t index) const {
	if (this->empty()) {
		return NAN;
	}
	double sum = 0;
	for (size_t i = index; i<this->size(); i++) {
		sum += (*this)[i];
	}
	return sum / static_cast<double>(this->size()-index);
}

/* ------------------------------- */

bool Signal::operator < (const Signal &input) const {
	for (size_t i = 0; i < size(); i++) {
		if ((*this)[i] >= input[i]) return false;
	}
	return true;
}

bool Signal::operator <= (const Signal &input) const {
	for (size_t i = 0; i < size(); i++) {
		if ((*this)[i] > input[i]) return false;
	}
	return true;
}

bool Signal::operator > (const Signal &input) const {
	for (size_t i = 0; i < size(); i++) {
		if ((*this)[i] <= input[i]) return false;
	}
	return true;
}

bool Signal::operator >= (const Signal &input) const {
	for (size_t i = 0; i < size(); i++) {
		if ((*this)[i] < input[i]) return false;
	}
	return true;
}

bool Signal::operator == (const Signal &input) const {
	for (size_t i = 0; i < size(); i++) {
		if ((*this)[i] != input[i]) return false;
	}
	return true;
}

bool Signal::operator != (const Signal &input) const {
	for (size_t i = 0; i < size(); i++) {
		if ((*this)[i] == input[i]) return false;
	}
	return true;
}

/* ------------------------------- */

void Signal::generateWaveform(rp_waveform_t type, double amplitude, double frequency, double phase, double offset, double delay, double duty_cycle) {
	// Vérification du duty cycle pour les formes d'onde concernées
	if (duty_cycle < 0.0 || duty_cycle > 100.0) {
		std::cerr << "Error: Duty cycle must be between 0 and 100." << std::endl;
		return;
	}
	duty_cycle /= 100;

	// Calcul de la période d'échantillonnage
	double sample_period = 1.0 / SAMPLING_FREQUENCY;
	// Conversion du délai en nombre d'échantillons
	double delay_samples = delay / 1000.0 * SAMPLING_FREQUENCY;

	// Variables pour le temps et la valeur du signal
	double t, value;
	for (size_t i = 0; i < this->size(); i++) {
		// Calcul du temps pour cet échantillon en tenant compte du décalage
		t = (i - delay_samples) * sample_period;
		value = 0;

		// Sélection du type de forme d'onde
		switch (type) {
			case RP_WAVEFORM_SINE:
				value = amplitude * std::sin(2 * M_PI * frequency * t + phase) + offset;
				break;
			case RP_WAVEFORM_SQUARE:
				if (t >= 0) {
					double period = 1.0 / frequency;
					double time_in_period = fmod(t, period);
					value = (time_in_period < 0.5 * period) ? amplitude : -amplitude;
					value += offset;
				}
				break;
			case RP_WAVEFORM_PWM:
				if (t >= 0) {
					double period = 1.0 / frequency;
					double time_in_period = fmod(t, period);
					value = (time_in_period < duty_cycle * period) ? amplitude : -amplitude;
					value += offset;
				}
				break;
			case RP_WAVEFORM_TRIANGLE:
				if (t >= 0) {
					double period = 1.0 / frequency;
					double time_in_period = fmod(t, period);
					double ramp_up_duration = duty_cycle * period;
					if (time_in_period < ramp_up_duration) {
						value = (2 * amplitude / ramp_up_duration) * time_in_period - amplitude;
					} else {
						value = (-2 * amplitude / (period - ramp_up_duration)) * (time_in_period - ramp_up_duration) + amplitude;
					}
					value += offset;
				}
				break;
			case RP_WAVEFORM_RAMP_UP:
				if (t >= 0) {
					double period = 1.0 / frequency;
					double time_in_period = fmod(t, period);
					value = (2 * amplitude / period) * time_in_period - amplitude + offset;
				}
				break;
			case RP_WAVEFORM_RAMP_DOWN:
				if (t >= 0) {
					double period = 1.0 / frequency;
					double time_in_period = fmod(t, period);
					value = (-2 * amplitude / period) * time_in_period + amplitude + offset;
				}
				break;
			case RP_WAVEFORM_DC:
				value = amplitude + offset;
				break;
			case RP_WAVEFORM_DC_NEG:
				value = -amplitude + offset;
				break;
			default:
				std::cerr << "Error: Unknown waveform type." << std::endl;
				return;
		}

		// Stockage de l'échantillon
		(*this)[i] = value;
	}
}


/* ------------------------------- */

static unsigned int reverseBits(unsigned int n, unsigned int bits) {
    unsigned int reversed = 0;
    for (unsigned int i = 0; i < bits; ++i) {
        reversed = (reversed << 1) | (n & 1);
        n >>= 1;
    }
    return reversed;
}

#define VERSION_FFT 1

void Signal::FFT(Spectrum &output_spectrum, size_t sample_offset) const {
    size_t p = BITS_PER_SAMPLE;
    size_t N = 1 << BITS_PER_SAMPLE;
    while (N > this->size() - sample_offset) {
        N >>= 1;
        p--;
    }

    std::vector<complexd> A(N);

    // Réorganisation des éléments en fonction de l'inversion des bits
    unsigned int j;
    for (size_t k = 0; k < N; ++k) {
        j = reverseBits(k, p);
        A[j] = complexd((*this)[k + sample_offset], 0);
    }

#if VERSION_FFT == 1
    // version 1
    std::vector<complexd> B(N);
    for (unsigned int q = 1; q <= p; ++q) {
        int taille = 1 << q;
        int taille_precedente = 1 << (q - 1);
        int nombre_tfd = 1 << (p - q);

        complexd phi(0, -2 * M_PI / taille);
        for (int m = 0; m < nombre_tfd; ++m) {
            int position = m * taille;
            for (int i = 0; i < taille_precedente; ++i) {
                complexd W = std::exp(phi * static_cast<double>(i));
                B[position + i] = A[position + i] + W * A[position + taille_precedente + i];
            }
            for (int i = taille_precedente; i < taille; ++i) {
                complexd W = std::exp(phi * static_cast<double>(i));
                B[position + i] = A[position + i - taille_precedente] + W * A[position + i];
            }
        }
        std::swap(A, B);
    }
    output_spectrum.resize(N);
    for (size_t k = 0; k < N; ++k) {
        output_spectrum[k] = A[k];
    }
#elif VERSION_FFT == 2
    // version 2
    for (unsigned int q = 1; q <= p; ++q) {
        size_t taille = 1 << q;
        size_t taille_precedente = 1 << (q - 1);

        complexd phi(0, -2 * M_PI / taille);
        complexd W_m(1, 0); // Initial W_m to 1 + 0i
        complexd W_m_increment = std::exp(phi);

        for (size_t m = 0; m < taille_precedente; ++m) {
            for (size_t k = m; k < N; k += taille) {
                complexd t = W_m * A[k + taille_precedente];
                complexd u = A[k];
                A[k] = u + t;
                A[k + taille_precedente] = u - t;
            }
            W_m *= W_m_increment;
        }
    }
    output_spectrum.resize(N);
    for (size_t k = 0; k < N; ++k) {
        output_spectrum[k] = A[k]/2.0;
    }
#else
    #error "Unknown version of FFT."
#endif
}

/* ------------------------------- */

double Signal::calculateNoiseRMS() const {
	double mean = this->mean();
	double noise, sum_squares = 0.0;
	for (double value : *this) {
		noise = value - mean;
		sum_squares += noise * noise;
	}
	return std::sqrt(sum_squares / this->size());
}

double Signal::getRisingTime(size_t &low_index, size_t &high_index) const
{
	double min_val = min();
	double max_val = max();
	double low_threshold = min_val + 0.1 * (max_val - min_val);
	double high_threshold = min_val + 0.9 * (max_val - min_val);

	low_index = 0;
	high_index = 0;
	for (size_t i = 1; i < size(); i++) {
		if ((*this)[i] >= high_threshold) {
			high_index = i;
			break;
		}
	}
	
	for (size_t i = 1; i < size(); i++) {
		if ((*this)[i] >= low_threshold) {
			low_index = i;
			break;
		}
	}

	double rise_time = (high_index-low_index) / SAMPLING_FREQUENCY;
	return rise_time;
}

/* ------------------------------- */

std::ostream &operator<<(std::ostream &out, const Signal &signal) {
	out << signal.getName() << "{";
	for (size_t i = 0; i < signal.size(); i++) {
		out << signal[i];
		if (i < signal.size()-1) out << ",";
	}
	out << "}";
	return out;
}