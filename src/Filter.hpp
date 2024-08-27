#ifndef __FILTER_HPP
#define __FILTER_HPP

#include <vector>
#include <complex>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include "Signal.hpp"
#include "Spectrum.hpp"

enum class FilterGabarit {
	LOW_PASS,
	HIGH_PASS,
	BAND_PASS,
	BAND_STOP,
};

enum class AnalogFilter {
	BESSEL,	  // IIR Bessel /Thomsonfilter design
	BUTTERWORTH, // IIR Butterworth filter design
	CHEBYSHEV1,  // IIR Chebyshev Type I filter design
	CHEBYSHEV2,  // IIR Chebyshev Type II filter design
	ELLIPTIC,	// IIR Cauer/Elliptic filter design
};

class IIRFilter {
public:
	IIRFilter();
	~IIRFilter();

	// paramétrage du filtre
	bool set(int order, double fc1, double fc2, FilterGabarit gabarit, AnalogFilter analogFilter, double rp = 0, double rs = 0);
	
	/**
	 * @brief Check if the filter is setup
	 * @return True if the filter is setup
	 */
	bool isSetup() const { return _isSetup; }

	// réinitialisation du filtre
	void reset();

	// Méthode pour afficher les coefficients du filtre (à des fins de débogage)
	void printCoefficients();

	/* ----- Méthode 1 ----- */

	// calcul des coefficients
	void setup();
	
	Signal apply(const Signal &input);

	// calcul de y(n) par application de l‘équation aux différences
	double apply(double y);

	Spectrum frequency_response(size_t num_points);

	/* ----- Méthode 2 ----- */

	// calcul des coefficients
	//void setup2();

private:
	void ButterworthCoefficients(); /* < Méthode 1 */

	bool _isSetup;

	int _order;
	double _fc1, _fc2;
	double _rp; // passband ripple
	double _rs; // stopband attenuation
	FilterGabarit _gabarit;
	AnalogFilter _analogFilter;

	std::vector<double> _a, _b;
	std::vector<std::complex<double>> _z, _p;
	double _k; // gain
	std::vector<double> _xMem, _yMem;
};


/* ------------------------------------------------------------------------ */
/* Méthode 2 */

// Return (z,p,k) for analog prototype of Nth-order Butterworth filter
/*void ButterworthAnalogPrototype(int N, std::vector<std::complex<double>> &z, std::vector<std::complex<double>> &p, double &k);

void TransformLowpassToLowpass(std::vector<std::complex<double>> &z, std::vector<std::complex<double>> &p, double &k, double warped);

void TransformLowpassToHighpass(std::vector<std::complex<double>> &z, std::vector<std::complex<double>> &p, double &k, double warped);

void TransformLowpassToBandpass(std::vector<std::complex<double>> &z, std::vector<std::complex<double>> &p, double &k, double warped1, double warped2);

void TransformLowpassToBandStop(std::vector<std::complex<double>> &z, std::vector<std::complex<double>> &p, double &k, double warped1, double warped2);

void IIRBilinearTransformation(std::vector<std::complex<double>> &z, std::vector<std::complex<double>> &p, double &k, double fs);

void PolynomialTransfer(const std::vector<std::complex<double>> &z, const std::vector<std::complex<double>> &p, double k, std::vector<double> &a, std::vector<double> &b);
*/





/// @brief Classe de filtre de moyennage
class AveragingFilter {
public:
	/// @brief Constructeur avec spécification de l'ordre du filtre
    AveragingFilter();
	
	/// @brief Destructeur
    ~AveragingFilter() {}

    /// @param order Ordre du filtre (nombre d'échantillons à moyenner)
	bool set(int order);

	/// @brief Réinitialiser le filtre
	void reset();

	/// @brief Appliquer le filtre de moyennage à un signal complet
    /// @param input Signal d'entrée
    /// @return Signal filtré
    Signal apply(const Signal &input);

	/// @brief Appliquer le filtre de moyennage à un échantillon unique
    /// @param x Échantillon d'entrée
    /// @return Échantillon filtré
    double apply(double x);

private:
    bool   _isSetup;
    int    _order;
    size_t _index;
    double _sum;
	double _memory_is_full; // indique si toutes les données ont été accumulé en fonction de l'ordre
    std::vector<double> _memory;
};



#endif // __FILTER_HPP