#ifndef __SPECTRUM_HPP
#define __SPECTRUM_HPP

#include <iostream>
#include <string.h>
#include <vector>
#include "globals.hpp"

class Signal;

class Spectrum : public std::vector<complexd> {
private:
	std::string mName; // Nom du spectre (Optionnel)
public:

	// Constructeur par defaut
	Spectrum(const std::string &name = "");
	
	// Constructeur
	Spectrum(size_t size, const std::string &name = "");

	// Constucteur 
	Spectrum(const std::vector<complexd> &values, const std::string &name = "");

	// Constructeur par recopie
	Spectrum(const Spectrum& other);

	Spectrum& operator=(const Spectrum& other);

	/* ------------------------------- */

	// Méthode pour obtenir le nom du spectre (retourne une chaîne vide si aucun nom n'est défini)
	const std::string &getName() const;

	// Méthode pour définir le nom du spectre
	void setName(const std::string &name);

	bool hasInfitityValue() const;

	/* ------------------------------- */

	Spectrum &addValue(complexd value);

	Spectrum &fill(complexd value = 0.0);

	/* ------------------------------- */

	// Surcharge de l'opérateur d'addition
	Spectrum operator+(const Spectrum& other) const;

	// Surcharge de l'opérateur de soustraction
	Spectrum operator-(const Spectrum& other) const;

	// Surcharge de l'opérateur de multiplication
	Spectrum operator*(const Spectrum& other) const;

	// Surcharge de l'opérateur de division
	Spectrum operator/(const Spectrum& other) const;

	// Surcharge de l'opérateur +=
	Spectrum& operator+=(const Spectrum& other);

	// Surcharge de l'opérateur -=
	Spectrum& operator-=(const Spectrum& other);

	// Surcharge de l'opérateur *=
	Spectrum& operator*=(const Spectrum& other);

	// Surcharge de l'opérateur /=
	Spectrum& operator/=(const Spectrum& other);

	/* ------------------------------- */

	// Surcharge de l'opérateur d'addition avec un complexe
	Spectrum operator+(complexd value) const;

	// Surcharge de l'opérateur de soustraction avec un complexe
	Spectrum operator-(complexd value) const;

	// Surcharge de l'opérateur de multiplication avec un complexe
	Spectrum operator*(complexd value) const;

	// Surcharge de l'opérateur de division avec un complexe
	Spectrum operator/(complexd value) const;

	// Surcharge de l'opérateur += avec un complexe
	Spectrum& operator+=(complexd value);

	// Surcharge de l'opérateur -= avec un complexe
	Spectrum& operator-=(complexd value);

	// Surcharge de l'opérateur *= avec un complexe
	Spectrum& operator*=(complexd value);

	// Surcharge de l'opérateur /= avec un complexe
	Spectrum& operator/=(complexd value);

	/* ------------------------------- */

	// Fonction amie pour l'addition d'un complexe et d'un spectrum
	friend Spectrum operator+(complexd value, const Spectrum &input);

	// Fonction amie pour la soustraction d'un complexe et d'un spectrum
	friend Spectrum operator-(complexd value, const Spectrum &input);

	// Fonction amie pour la multiplication d'un complexe et d'un spectrum
	friend Spectrum operator*(complexd value, const Spectrum &input);

	// Fonction amie pour la division d'un complexe et d'un spectrum
	friend Spectrum operator/(complexd value, const Spectrum &input);

	/* ------------------------------- */
	/* FONCTIONS TRIGONOMETRIQUES */

	Spectrum cos() const;

	Spectrum sin() const;

	Spectrum tan() const;

	Spectrum cosh() const;

	Spectrum sinh() const;

	Spectrum tanh() const;

	Spectrum acos() const;

	Spectrum asin() const;

	Spectrum atan() const;

	Spectrum acosh() const;

	Spectrum asinh() const;

	Spectrum atanh() const;

	friend Spectrum cos(const Spectrum &input) { return input.cos(); }

	friend Spectrum sin(const Spectrum &input) { return input.sin(); }

	friend Spectrum tan(const Spectrum &input) { return input.tan(); }

	friend Spectrum cosh(const Spectrum &input) { return input.cosh(); }

	friend Spectrum sinh(const Spectrum &input) { return input.sinh(); }

	friend Spectrum tanh(const Spectrum &input) { return input.tanh(); }

	friend Spectrum acos(const Spectrum &input) { return input.acos(); }

	friend Spectrum asin(const Spectrum &input) { return input.asin(); }

	friend Spectrum atan(const Spectrum &input) { return input.atan(); }

	friend Spectrum acosh(const Spectrum &input) { return input.acosh(); }

	friend Spectrum asinh(const Spectrum &input) { return input.asinh(); }

	friend Spectrum atanh(const Spectrum &input) { return input.atanh(); }

	/* ------------------------------- */

	// Fonction pour mettre au carré chaque élément du spectrum
	Spectrum square() const;

	// Fonction pour élever chaque élément du spectrum à une puissance donnée
	Spectrum pow(complexd exponent) const;

	// Fonction pour calculer la racine carrée de chaque élément du spectrum
	Spectrum sqrt() const;

	// Fonction pour calculer le logarithme naturel de chaque élément du spectrum
	Spectrum log() const;

	// Fonction pour calculer le logarithme décimal de chaque élément du spectrum
	Spectrum log10() const;

	// Fonction pour calculer l'exponentielle de chaque élément du spectrum
	Spectrum exp() const;

	friend Spectrum square(const Spectrum &input) { return input.square(); }

	friend Spectrum pow(const Spectrum &input, complexd exponent) { return input.pow(exponent); }

	friend Spectrum sqrt(const Spectrum &input) { return input.sqrt(); }

	friend Spectrum log(const Spectrum &input) { return input.log(); }

	friend Spectrum log10(const Spectrum &input) { return input.log10(); }

	friend Spectrum exp(const Spectrum &input) { return input.exp(); }

	/* ------------------------------- */

	Signal abs() const;

	/* ------------------------------- */

	// Fonction pour calculer le maximum du spectrum
	complexd max() const;

	// Fonction pour calculer le minimum du spectrum
	complexd min() const;

	// Fonction pour calculer la moyenne du spectrum
	complexd mean() const;

	friend complexd max(const Spectrum &input) { return input.max(); }

	friend complexd min(const Spectrum &input) { return input.min(); }

	friend complexd mean(const Spectrum &input) { return input.mean(); }

	/* ------------------------------- */

	bool operator < (const Spectrum &input) const;

	bool operator <= (const Spectrum &input) const;

	bool operator > (const Spectrum &input) const;

	bool operator >= (const Spectrum &input) const;

	bool operator == (const Spectrum &input) const;

	bool operator != (const Spectrum &input) const;

	/* ------------------------------- */

	Signal calculateMagnitude() const;

	Signal calculatePhase() const;

	/**
	 * Fonction pour effectuer la transformée de Fourier inverse rapide (IFFT) et reconstruire le signal
	 * @param[out] output_signal : Signal à reconstruire
	 */
	void IFFT(Signal &output_signal) const;

	/* ------------------------------- */

	friend std::ostream& operator << (std::ostream &out, const Spectrum &spectrum);
};

#endif // __SPECTRUM_HPP