#include <limits>
#include <cmath>
#include "Spectrum.hpp"
#include "Signal.hpp"

Spectrum::Spectrum(const std::string &name) : std::vector<complexd>(BUFFER_SIZE, 0.0), mName(name) {}

Spectrum::Spectrum(size_t size, const std::string &name) : std::vector<complexd>(size), mName(name) {}

Spectrum::Spectrum(const std::vector<complexd> &values, const std::string &name) : std::vector<complexd>(values), mName(name) {}

Spectrum::Spectrum(const Spectrum &other) : std::vector<complexd>(other),mName(other.mName) {}

Spectrum &Spectrum::operator=(const Spectrum &other) {
	if (this != &other) {
		std::vector<complexd>::operator=(other);
	}
	return *this;
}

/* ------------------------------- */

const std::string &Spectrum::getName() const {
	return mName;
}

void Spectrum::setName(const std::string &name) {
	mName = name;
}

bool Spectrum::hasInfitityValue() const {
	for (size_t i = 0; i < size(); i++) {
		if (((*this)[i].real() == INFINITY) || ((*this)[i].real() == -INFINITY)) {
			return true;
		}
	}
	return false;
}

/* ------------------------------- */

Spectrum &Spectrum::addValue(complexd value) {
	std::vector<complexd>::push_back(value);
	return *this;
}

Spectrum &Spectrum::fill(complexd value)
{
	for (size_t i = 0; i < size(); i++) {
		(*this)[i] = value;
	}
	return *this;
}

/* ------------------------------- */

Spectrum Spectrum::operator+(const Spectrum &other) const {
	Spectrum output(other.size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = (*this)[i] + other[i];
	}
	return output;
}

Spectrum Spectrum::operator-(const Spectrum &other) const {
	Spectrum output(other.size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = (*this)[i] - other[i];
	}
	return output;
}

Spectrum Spectrum::operator*(const Spectrum &other) const {
	Spectrum output(other.size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = (*this)[i] * other[i];
	}
	return output;
}

Spectrum Spectrum::operator/(const Spectrum &other) const {
	Spectrum output(other.size());
	for (size_t i = 0; i < size(); i++) {
		if (std::abs(other[i]) == 0) output[i] = {INFINITY, INFINITY};
		else output[i] = (*this)[i] / other[i];
	}
	return output;
}

Spectrum &Spectrum::operator+=(const Spectrum &other) {
	for (size_t i = 0; i < size(); i++) {
		(*this)[i] += other[i];
	}
	return *this;
}

Spectrum &Spectrum::operator-=(const Spectrum &other) {
	for (size_t i = 0; i < size(); i++) {
		(*this)[i] -= other[i];
	}
	return *this;
}

Spectrum &Spectrum::operator*=(const Spectrum &other) {
	for (size_t i = 0; i < size(); i++) {
		(*this)[i] *= other[i];
	}
	return *this;
}

Spectrum &Spectrum::operator/=(const Spectrum &other) {
	for (size_t i = 0; i < size(); i++) {
		if (std::abs(other[i]) == 0) (*this)[i] = {INFINITY, INFINITY};
		else (*this)[i] /= other[i];
	}
	return *this;
}

/* ------------------------------- */

Spectrum Spectrum::operator+(complexd value) const {
	Spectrum output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = (*this)[i] + value;
	}
	return output;
}

Spectrum Spectrum::operator-(complexd value) const {
	Spectrum output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = (*this)[i] - value;
	}
	return output;
}

Spectrum Spectrum::operator*(complexd value) const {
	Spectrum output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = (*this)[i] * value;
	}
	return output;
}

Spectrum Spectrum::operator/(complexd value) const {
	Spectrum output(size());
	for (size_t i = 0; i < size(); i++) {
		if (std::abs(value) == 0) output[i] = {INFINITY, INFINITY};
		else output[i] = (*this)[i] / value;
	}
	return output;
}

Spectrum &Spectrum::operator+=(complexd value) {
	for (size_t i = 0; i < size(); i++) {
		(*this)[i] += value;
	}
	return *this;
}

Spectrum &Spectrum::operator-=(complexd value) {
	for (size_t i = 0; i < size(); i++) {
		(*this)[i] -= value;
	}
	return *this;
}

Spectrum &Spectrum::operator*=(complexd value) {
	for (size_t i = 0; i < size(); i++) {
		(*this)[i] *= value;
	}
	return *this;
}

Spectrum &Spectrum::operator/=(complexd value) {
	for (size_t i = 0; i < size(); i++) {
		if (std::abs(value) == 0) (*this)[i] = {INFINITY, INFINITY};
		else (*this)[i] /= value;
	}
	return *this;
}

/* ------------------------------- */

Spectrum operator+(complexd value, const Spectrum &input) {
	Spectrum output(input.size());
	for (size_t i = 0; i < input.size(); i++) {
		output[i] = input[i] + value;
	}
	return output;
}

Spectrum operator-(complexd value, const Spectrum &input) {
	Spectrum output(input.size());
	for (size_t i = 0; i < input.size(); i++) {
		output[i] = input[i] - value;
	}
	return output;
}

Spectrum operator*(complexd value, const Spectrum &input) {
	Spectrum output(input.size());
	for (size_t i = 0; i < input.size(); i++) {
		output[i] = input[i] * value;
	}
	return output;
}

Spectrum operator/(complexd value, const Spectrum &input) {
	Spectrum output(input.size());
	for (size_t i = 0; i < input.size(); i++) {
		if (std::abs(value) == 0) output[i] = {INFINITY, INFINITY};
		else output[i] = input[i] / value;
	}
	return output;
}

/* ------------------------------- */

Spectrum Spectrum::cos() const {
	Spectrum output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::cos((*this)[i]);
	}
	return output;
}

Spectrum Spectrum::sin() const {
	Spectrum output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::sin((*this)[i]);
	}
	return output;
}

Spectrum Spectrum::tan() const {
	Spectrum output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::tan((*this)[i]);
	}
	return output;
}

Spectrum Spectrum::cosh() const {
	Spectrum output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::cosh((*this)[i]);
	}
	return output;
}

Spectrum Spectrum::sinh() const {
	Spectrum output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::sinh((*this)[i]);
	}
	return output;
}

Spectrum Spectrum::tanh() const {
	Spectrum output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::tanh((*this)[i]);
	}
	return output;
}

Spectrum Spectrum::acos() const {
	Spectrum output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::acos((*this)[i]);
	}
	return output;
}

Spectrum Spectrum::asin() const {
	Spectrum output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::asin((*this)[i]);
	}
	return output;
}

Spectrum Spectrum::atan() const {
	Spectrum output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::atan((*this)[i]);
	}
	return output;
}

Spectrum Spectrum::acosh() const {
	Spectrum output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::acosh((*this)[i]);
	}
	return output;
}

Spectrum Spectrum::asinh() const {
	Spectrum output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::asinh((*this)[i]);
	}
	return output;
}

Spectrum Spectrum::atanh() const {
	Spectrum output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::atanh((*this)[i]);
	}
	return output;
}

/* ------------------------------- */

Spectrum Spectrum::square() const {
	Spectrum output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = (*this)[i] * (*this)[i];
	}
	return output;
}

Spectrum Spectrum::pow(complexd exponent) const {
	Spectrum output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::pow((*this)[i], exponent);
	}
	return output;
}

Spectrum Spectrum::sqrt() const {
	Spectrum output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::sqrt((*this)[i]);
	}
	return output;
}

Spectrum Spectrum::log() const {
	Spectrum output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::log((*this)[i]);
	}
	return output;
}

Spectrum Spectrum::log10() const {
	Spectrum output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::log10((*this)[i]);
	}
	return output;
}

Spectrum Spectrum::exp() const {
	Spectrum output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::exp((*this)[i]);
	}
	return output;
}

/* ------------------------------- */

Signal Spectrum::abs() const {
	Signal output(size());
	for (size_t i = 0; i < size(); i++) {
		output[i] = std::abs((*this)[i]);
	}
	return output;
}

complexd Spectrum::max() const {
	if (this->empty()) {
		return NAN;
	}
	complexd maxElem = (*this)[0];
	for (size_t i = 0; i<this->size(); i++) {
		if (std::abs(maxElem) < std::abs((*this)[i])) maxElem = (*this)[i];
	}
	return maxElem;
}

complexd Spectrum::min() const {
	if (this->empty()) {
		return NAN;
	}
	complexd minElem = (*this)[0];
	for (size_t i = 0; i<this->size(); i++) {
		if (std::abs(minElem) > std::abs((*this)[i])) minElem = (*this)[i];
	}
	return minElem;
}

complexd Spectrum::mean() const {
	if (this->empty()) {
		return NAN;
	}
	complexd sum = 0;
	for (size_t i = 0; i<this->size(); i++) {
		sum += (*this)[i];
	}
	return sum / static_cast<complexd>(this->size());
}

/* ------------------------------- */

bool Spectrum::operator < (const Spectrum &input) const {
	for (size_t i = 0; i < size(); i++) {
		if (std::abs((*this)[i]) >= std::abs(input[i])) return false;
	}
	return true;
}

bool Spectrum::operator <= (const Spectrum &input) const {
	for (size_t i = 0; i < size(); i++) {
		if (std::abs((*this)[i]) > std::abs(input[i])) return false;
	}
	return true;
}

bool Spectrum::operator > (const Spectrum &input) const {
	for (size_t i = 0; i < size(); i++) {
		if (std::abs((*this)[i]) <= std::abs(input[i])) return false;
	}
	return true;
}

bool Spectrum::operator >= (const Spectrum &input) const {
	for (size_t i = 0; i < size(); i++) {
		if (std::abs((*this)[i]) < std::abs(input[i])) return false;
	}
	return true;
}

bool Spectrum::operator == (const Spectrum &input) const {
	for (size_t i = 0; i < size(); i++) {
		if ((*this)[i] != input[i]) return false;
	}
	return true;
}

bool Spectrum::operator != (const Spectrum &input) const {
	for (size_t i = 0; i < size(); i++) {
		if ((*this)[i] == input[i]) return false;
	}
	return true;
}

Signal Spectrum::calculateMagnitude() const {
	const size_t N = this->size();
	Signal output(N);
	for (size_t i = 0; i < N; i++) {
		output[i] = std::abs((*this)[i])/N;
	}
	return output;
}

Signal Spectrum::calculatePhase() const {
	const size_t N = this->size();
	Signal output(N);
	for (size_t i = 0; i < N; i++) {
		output[i] = std::arg((*this)[i]);
	}
	return output;
}

/* ------------------------------- */

unsigned int reverseBits(unsigned int n, unsigned int bits) {
	unsigned int reversed = 0;
	for (unsigned int i = 0; i < bits; ++i) {
		reversed = (reversed << 1) | (n & 1);
		n >>= 1;
	}
	return reversed;
}

void Spectrum::IFFT(Signal &out_signal) const {
	size_t p = BITS_PER_SAMPLE;
	size_t N = 1 << BITS_PER_SAMPLE;
	while (N > this->size()) {
		N >>= 1;
		p--;
	}

	std::vector<complexd> A(N), B(N);

	// Inverser la séquence d'entrée
	for (size_t k = 0; k < N; ++k) {
		A[k] = std::conj((*this)[k]);
	}

	for (unsigned int q = 1; q <= p; ++q) {
		size_t taille = 1 << q;
		size_t taille_precedente = 1 << (q - 1);

		complexd phi(0, 2 * M_PI / taille); // Notez le signe positif ici pour l'IFFT
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

	// Inverser la séquence de sortie et normaliser
	out_signal.resize(N);
	for (size_t k = 0; k < N; ++k) {
		out_signal[k] = std::real(std::conj(A[k])) / static_cast<double>(N);
	}
}

/* ------------------------------- */

std::ostream &operator<<(std::ostream &out, const Spectrum &spectrum) {
	out << spectrum.getName() << "{";
	for (size_t i = 0; i < spectrum.size(); i++) {
		out << spectrum[i].real() << (spectrum[i].imag() >= 0 ? "+" : "") << spectrum[i].imag() << "j";
		if (i < spectrum.size()-1) out << ",";
	}
	out << "}";
	return out;
}