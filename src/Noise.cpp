#include "Noise.hpp"
#include "Signal.hpp"


WhiteNoise::WhiteNoise() : Noise(), _uniformDistribution(-1.0, 1.0) {}

void WhiteNoise::setup() {
	// Pas de coefficients à calculer
}

Signal WhiteNoise::apply(const Signal &input) {
	Signal output = input;
	for (auto &sample : output) {
		sample += _gain * _uniformDistribution(_generator);
	}
	return output;
}

double WhiteNoise::apply(double input) {
	return input + _gain * _uniformDistribution(_generator);
}




PinkNoise::PinkNoise() : Noise(), b(7, 0.0), a(7, 0.0), x_hist(7, 0.0), y_hist(7, 0.0) {}

void PinkNoise::setup() {
	// Coefficients based on Voss-McCartney algorithm for pink noise
	// You can adjust these coefficients as needed
	b = {0.99886, 0.99332, 0.96900, 0.86650, 0.55000, -0.7616, 0.115926};
	a = {1.0, -1.6915, 0.8928, -0.3092, 0.1990, -0.1606, 0.1065};
}

Signal PinkNoise::apply(const Signal &input) {
	Signal output = input;
	for (size_t i = 0; i < input.size(); ++i) {
		output[i] = apply(input[i]);
	}
	return output;
}

double PinkNoise::apply(double input) {
	x_hist.insert(x_hist.begin(), input); // Add current input to history
	x_hist.pop_back(); // Remove oldest input

	double yn = 0.0;
	for (size_t i = 0; i < b.size(); ++i) {
		yn += b[i] * x_hist[i];
	}
	for (size_t i = 1; i < a.size(); ++i) {
		yn -= a[i] * y_hist[i - 1];
	}
	yn /= a[0];

	y_hist.insert(y_hist.begin(), yn); // Add current output to history
	y_hist.pop_back(); // Remove oldest output

	return yn * _gain;
}




BrownNoise::BrownNoise() : Noise(), previous(0.0), distribution(-1.0, 1.0) {}

void BrownNoise::setup() {
	// Pas de coefficients spécifiques à calculer pour le bruit marron
}

Signal BrownNoise::apply(const Signal &input) {
	Signal output = input;
	for (auto &sample : output) {
		double white = distribution(generator);
		previous = (previous + (_gain * white)) / 1.02;
		sample += previous * 0.1; // Ajuster le facteur si nécessaire
	}
	return output;
}

double BrownNoise::apply(double input) {
	double white = distribution(generator);
	previous = (previous + (_gain * white)) / 1.02;
	return input + previous * 0.1; // Ajuster le facteur si nécessaire
}
