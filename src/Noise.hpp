#ifndef __NOISE_HPP
#define __NOISE_HPP

#include <vector>
#include <random>
#include <cmath>

class Signal; // Assurez-vous que la classe Signal est déjà définie

/**
 * @brief Abstract class for noise
 */
class Noise {
protected:
	double _gain;
public:
	Noise() : _gain(1.0) {}

	virtual ~Noise() = default;

	/* Choisir un gain pour le bruit */
	void setGain(double gain) {
		_gain = gain;
	}

	virtual void setup() = 0; // Calculer les coefficients

	/* Processus de filtrage */
	virtual Signal apply(const Signal &input) = 0;

	/* Processus de filtrage */
	virtual double apply(double input) = 0;
};


/**
 * @brief White noise
 * @details White noise is a type of noise with a flat power spectral density.
 */
class WhiteNoise : public Noise {
private:
	std::default_random_engine _generator;
	std::uniform_real_distribution<double> _uniformDistribution;
public:
	WhiteNoise();
	void setup() override;
	Signal apply(const Signal &input) override;
	double apply(double input) override;
};

/**
 * @brief Pink noise
 * @details Pink noise is a random signal whose spectral density is constant per octave band.
 */
class PinkNoise : public Noise {
private:
	std::vector<double> b; // Numerator coefficients
	std::vector<double> a; // Denominator coefficients
	std::vector<double> x_hist; // Input history
	std::vector<double> y_hist; // Output history
public:
	PinkNoise();
	void setup() override;
	Signal apply(const Signal &input) override;
	double apply(double input) override;
};

/**
 * @brief Brown noise
 * @details Brown noise is a random signal whose spectral density is proportional to the inverse of the frequency.
 */
class BrownNoise : public Noise {
private:
	double previous;
	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution;
public:
	BrownNoise();
	void setup() override;
	Signal apply(const Signal &input) override;
	double apply(double input) override;
};

#endif // __NOISE_HPP