#ifndef __SIGNAL_HPP
#define __SIGNAL_HPP

#include <iostream>
#include <string.h>
#include <vector>
#include "globals.hpp"
#include "rp.h"

class Spectrum;

class Signal : public std::vector<double> {
private:
	std::string mName; // Nom du signal (Optionnel)
public:

	// Constructeur par defaut
	Signal(const std::string &name = "");
	
	// Constructeur
	Signal(size_t size, const std::string &name = "");

	// Constucteur 
	Signal(const std::vector<double> &values, const std::string &name = "");

	// Constructeur par recopie
	Signal(const Signal& other);

	Signal& operator=(const Signal& other);

	/* ------------------------------- */

	// Méthode pour obtenir le nom du signal (retourne une chaîne vide si aucun nom n'est défini)
	const std::string &getName() const;

	// Méthode pour définir le nom du signal
	void setName(const std::string &name);

	bool hasInfitityValue() const;

	/* ------------------------------- */

	Signal &addValue(double value);

	Signal &fill(double value = 0.0);

	/* ------------------------------- */

	// Surcharge de l'opérateur d'addition
	Signal operator+(const Signal& other) const;

	// Surcharge de l'opérateur de soustraction
	Signal operator-(const Signal& other) const;

	// Surcharge de l'opérateur de multiplication
	Signal operator*(const Signal& other) const;

	// Surcharge de l'opérateur de division
	Signal operator/(const Signal& other) const;

	// Surcharge de l'opérateur +=
	Signal& operator+=(const Signal& other);

	// Surcharge de l'opérateur -=
	Signal& operator-=(const Signal& other);

	// Surcharge de l'opérateur *=
	Signal& operator*=(const Signal& other);

	// Surcharge de l'opérateur /=
	Signal& operator/=(const Signal& other);

	/* ------------------------------- */

	// Surcharge de l'opérateur d'addition avec un complexe
	Signal operator+(double value) const;

	// Surcharge de l'opérateur de soustraction avec un complexe
	Signal operator-(double value) const;

	// Surcharge de l'opérateur de multiplication avec un complexe
	Signal operator*(double value) const;

	// Surcharge de l'opérateur de division avec un complexe
	Signal operator/(double value) const;

	// Surcharge de l'opérateur += avec un complexe
	Signal& operator+=(double value);

	// Surcharge de l'opérateur -= avec un complexe
	Signal& operator-=(double value);

	// Surcharge de l'opérateur *= avec un complexe
	Signal& operator*=(double value);

	// Surcharge de l'opérateur /= avec un complexe
	Signal& operator/=(double value);

	/* ------------------------------- */

	// Fonction amie pour l'addition d'un complexe et d'un signal
	friend Signal operator+(double value, const Signal &input);

	// Fonction amie pour la soustraction d'un complexe et d'un signal
	friend Signal operator-(double value, const Signal &input);

	// Fonction amie pour la multiplication d'un complexe et d'un signal
	friend Signal operator*(double value, const Signal &input);

	// Fonction amie pour la division d'un complexe et d'un signal
	friend Signal operator/(double value, const Signal &input);

	/* ------------------------------- */
	/* FONCTIONS TRIGONOMETRIQUES */

	Signal cos() const;

	Signal sin() const;

	Signal tan() const;

	Signal cosh() const;

	Signal sinh() const;

	Signal tanh() const;

	Signal acos() const;

	Signal asin() const;

	Signal atan() const;

	Signal acosh() const;

	Signal asinh() const;

	Signal atanh() const;

	friend Signal cos(const Signal &input) { return input.cos(); }

	friend Signal sin(const Signal &input) { return input.sin(); }

	friend Signal tan(const Signal &input) { return input.tan(); }

	friend Signal cosh(const Signal &input) { return input.cosh(); }

	friend Signal sinh(const Signal &input) { return input.sinh(); }

	friend Signal tanh(const Signal &input) { return input.tanh(); }

	friend Signal acos(const Signal &input) { return input.acos(); }

	friend Signal asin(const Signal &input) { return input.asin(); }

	friend Signal atan(const Signal &input) { return input.atan(); }

	friend Signal acosh(const Signal &input) { return input.acosh(); }

	friend Signal asinh(const Signal &input) { return input.asinh(); }

	friend Signal atanh(const Signal &input) { return input.atanh(); }

	/* ------------------------------- */

	// Fonction pour calculer la valeur absolue de chaque élément du signal
	Signal abs() const;

	// Fonction pour mettre au carré chaque élément du signal
	Signal square() const;

	// Fonction pour élever chaque élément du signal à une puissance donnée
	Signal pow(double exponent) const;

	// Fonction pour calculer la racine carrée de chaque élément du signal
	Signal sqrt() const;

	// Fonction pour calculer le logarithme naturel de chaque élément du signal
	Signal log() const;

	// Fonction pour calculer le logarithme binaire de chaque élément du signal
	Signal log2() const;

	// Fonction pour calculer le logarithme décimal de chaque élément du signal
	Signal log10() const;

	// Fonction pour calculer l'exponentielle de chaque élément du signal
	Signal exp() const;
	
	friend Signal abs(const Signal &input) { return input.abs(); }

	friend Signal square(const Signal &input) { return input.square(); }

	friend Signal pow(const Signal &input, double exponent) { return input.pow(exponent); }

	friend Signal sqrt(const Signal &input) { return input.sqrt(); }

	friend Signal log(const Signal &input) { return input.log(); }

	friend Signal log2(const Signal &input) { return input.log2(); }

	friend Signal log10(const Signal &input) { return input.log10(); }

	friend Signal exp(const Signal &input) { return input.exp(); }

	/* ------------------------------- */

	// Fonction pour calculer le maximum du signal
	double max(size_t index = 0) const;

	// Fonction pour calculer le minimum du signal
	double min(size_t index = 0) const;

	// Fonction pour calculer la moyenne du signal
	double mean(size_t index = 0) const;

	friend double max(const Signal &input) { return input.max(); }

	friend double min(const Signal &input) { return input.min(); }

	friend double mean(const Signal &input) { return input.mean(); }

	/* ------------------------------- */

	bool operator < (const Signal &input) const;

	bool operator <= (const Signal &input) const;

	bool operator > (const Signal &input) const;

	bool operator >= (const Signal &input) const;

	bool operator == (const Signal &input) const;

	bool operator != (const Signal &input) const;

	/* ------------------------------- */

	void generateWaveform(rp_waveform_t type = RP_WAVEFORM_SINE, double amplitude = 1.0, double frequency = 1e3, double phase = 0.0, double offset = 0.0, double delay = 0.0, double duty_cycle = 50.0);

	/* ------------------------------- */

	/**
	 * Fonction pour effectuer la transformée de Fourier discrète rapide(FFT) et générer le spectre
	 * @param[out] output_spectrum Spectre de la transformée de Fourier discrète rapide(FFT)
	 */
	void FFT(Spectrum &output_spectrum, size_t sample_offset = 0) const;

	/* ------------------------------- */

	// Calculer le niveau RMS du bruit
	double calculateNoiseRMS() const;

	double getRisingTime(size_t &low_index, size_t &high_index) const;

	/* ------------------------------- */

	friend std::ostream& operator << (std::ostream &out, const Signal &signal);
};

#endif // __SIGNAL_HPP