#include "utils.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>
#include <numeric>
#include <complex>
#include <cstring>
#include <algorithm>
#include <sys/time.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include "rp_hw-calib.h"
#include "rp_hw-profiles.h"

double modulo(double value, double modulo) {
    // Utiliser fmod pour trouver le reste de value / modulo
    double result = fmod(value, modulo);
    
    // Assurer que le résultat est dans [0, modulo]
    /*if (result < 0) {
        result += modulo;
    }*/
    return result;
}

std::string toLower(std::string str)
{
    std::string &result = str;
	for (size_t i = 0; i < str.length(); i++) {
		result[i] = tolower(result[i]);
	}
	return result;
}

std::string toUpper(std::string str) {
    std::string &result = str;
    for (size_t i = 0; i < str.length(); i++) {
        result[i] = toupper(result[i]);
    }
	return result;
}

bool stringToBool(const std::string &str) {
	std::string _str = toUpper(str);
    if (_str == "TRUE" || str == "1") {
        return true;
    } else if (_str == "FALSE" || str == "0") {
        return false;
    } else {
        throw std::invalid_argument("Invalid boolean value: " + str);
    }
}

rp_waveform_t stringToRpWaveform(const std::string &name) {
	std::string _name = toUpper(name);
    if (_name == "SINE" || _name == "SINUS" ||_name == "RP_WAVEFORM_SINE") {
		return RP_WAVEFORM_SINE;
	} else if (_name == "SQUARE" || _name == "RP_WAVEFORM_SQUARE") {
		return RP_WAVEFORM_SQUARE;
	} else if (_name == "PWM" || _name == "RP_WAVEFORM_PWM") {
		return RP_WAVEFORM_PWM;
	} else if (_name == "TRIANGLE" || _name == "RP_WAVEFORM_TRIANGLE") {
		return RP_WAVEFORM_TRIANGLE;
	} else if (_name == "RAMP_UP" || _name == "RP_WAVEFORM_RAMP_UP") {
		return RP_WAVEFORM_RAMP_UP;
	} else if (_name == "RAMP_DOWN" || _name == "RP_WAVEFORM_RAMP_DOWN") {
		return RP_WAVEFORM_RAMP_DOWN;
	} else if (_name == "DC" || _name == "RP_WAVEFORM_DC") {
		return RP_WAVEFORM_DC;
	} else if (_name == "DC_NEG" || _name == "RP_WAVEFORM_DC_NEG") {
		return RP_WAVEFORM_DC;
	} else {
		std::cerr << "Unknown waveform: " << _name << std::endl;
		return RP_WAVEFORM_SINE;
	}
}

std::string rpWaveformToString(rp_waveform_t waveform) {
    switch (waveform) {
        case RP_WAVEFORM_SINE:
            return "RP_WAVEFORM_SINE";
        case RP_WAVEFORM_SQUARE:
            return "RP_WAVEFORM_SQUARE";
        case RP_WAVEFORM_PWM:
            return "RP_WAVEFORM_PWM";
        case RP_WAVEFORM_TRIANGLE:
            return "RP_WAVEFORM_TRIANGLE";
        case RP_WAVEFORM_RAMP_UP:
            return "RP_WAVEFORM_RAMP_UP";
        case RP_WAVEFORM_RAMP_DOWN:
            return "RP_WAVEFORM_RAMP_DOWN";
        case RP_WAVEFORM_DC:
            return "RP_WAVEFORM_DC";
		case RP_WAVEFORM_DC_NEG:
			return "RP_WAVEFORM_DC_NEG";
        default:
            return "RP_WAVEFORM_SINE";
    }
}

WindowType stringToWindowType(const std::string &str) {
	WindowType type = WindowType::Rectangular;
	if (str.size() == 0) {
		return type;
	}
	std::string _str = toLower(str);
	if (_str == "rectangular") {
		type = WindowType::Rectangular;
	} else if (_str == "triangular") {
		type = WindowType::Triangular;
	} else if (_str == "parzen") {
		type = WindowType::Parzen;
	} else if (_str == "welch") {
		type = WindowType::Welch;
	} else if (_str == "sine") {
		type = WindowType::Sine;
	} else if (_str == "hann") {
		type = WindowType::Hann;
	} else if (_str == "hamming") {
		type = WindowType::Hamming;
	} else if (_str == "blackman") {
		type = WindowType::Blackman;
	} else if (_str == "nuttall") {
		type = WindowType::Nuttall;
	} else if (_str == "blackman-nuttall" || _str == "blackmannuttall") {
		type = WindowType::BlackmanNuttall;
	} else if (_str == "blackman-harris" || _str == "blackmanharris") {
		type = WindowType::BlackmanHarris;
	} else if (_str == "tukey") {
		type = WindowType::Tukey;
	} else if (_str == "planck-taper" || _str == "plancktaper") {
		type = WindowType::PlanckTaper;
	}
	return type;
}

std::string windowTypeToString(WindowType type) {
	switch (type) {
		case WindowType::Rectangular:
			return "Rectangular";
		case WindowType::Triangular:
			return "Triangular";
		case WindowType::Parzen:
			return "Parzen";
		case WindowType::Welch:
			return "Welch";
		case WindowType::Sine:
			return "Sine";
		case WindowType::Hann:
			return "Hann";
		case WindowType::Hamming:
			return "Hamming";
		case WindowType::Blackman:
			return "Blackman";
		case WindowType::Nuttall:
			return "Nuttall";
		case WindowType::BlackmanNuttall:
			return "Blackman-Nuttall";
		case WindowType::BlackmanHarris:
			return "Blackman-Harris";
		case WindowType::Tukey:
			return "Tukey";
		case WindowType::PlanckTaper:
			return "Planck-Taper";
		default:
			return "Unknown";
	}
}

// for string delimiter
std::vector<std::string> split(std::string s, std::string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr (pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back (token);
	}

	res.push_back (s.substr (pos_start));
	return res;
}

int calculateDecimation(double f, int N_per_period) {
	// Calcul du nombre d'échantillons par période
	double T_per_period = 1.0 / f;
	int N = static_cast<int>(round(T_per_period * MAX_SAMPLING_FREQUENCY)); // Nombre total d'échantillons par période

	// Trouver la décimation la plus grande puissance de 2 inférieure ou égale à 16384 et ne dépassant pas N
	int decimation = 1;
	while (N > 2*N_per_period && decimation <= 16384) {
		decimation *= 2;
		N = static_cast<int>(round(T_per_period * static_cast<double>(MAX_SAMPLING_FREQUENCY)/static_cast<double>(decimation))); // Nombre total d'échantillons par période
	}

	return decimation;
}

int convertToInteger(const std::string& str) {
    size_t length = str.length();
    if (length == 0) {
        throw std::invalid_argument("Empty string is not a valid number.");
    }

    double number = 0.0;
    int exponent = 0;

    // Identify and separate the numeric part and the suffix
    size_t pos = 0;
    bool hasExponent = false;
    while (pos < length && (isdigit(str[pos]) || str[pos] == '.' || str[pos] == '-' || str[pos] == '+')) {
        if (str[pos] == 'e' || str[pos] == 'E') {
            hasExponent = true;
            break;
        }
        pos++;
    }

    if (pos > 0) {
        number = std::stod(str.substr(0, pos));
    } else {
        throw std::invalid_argument("Invalid numeric part.");
    }

    if (hasExponent) {
        // Handle scientific notation
        size_t exponentPos = pos;
        pos++;
        while (pos < length && (isdigit(str[pos]) || str[pos] == '-' || str[pos] == '+')) {
            pos++;
        }
        exponent = std::stoi(str.substr(exponentPos + 1, pos - exponentPos - 1));
    } else if (pos < length) {
        // Handle suffix notation
        char suffix = str[pos];
        switch (suffix) {
            case 'M': exponent = 6; break;
            case 'k':
            case 'K': exponent = 3; break;
            case 'm': exponent = -3; break;
            case 'u': exponent = -6; break; // micro
            case 'n': exponent = -9; break;
            case 'p': exponent = -12; break;
            default: throw std::invalid_argument("Unknown suffix.");
        }
        pos++;
    }

    if (pos != length) {
        throw std::invalid_argument("Invalid format.");
    }

    // Apply the exponent
    double result = number * std::pow(10, exponent);

    // Convert to integer
    return static_cast<int>(result);
}

// Fonction pour calculer le PGCD de deux nombres
int gcd(int a, int b) {
	while (b != 0) {
		int t = b;
		b = a % b;
		a = t;
	}
	return a;
}

// Fonction pour calculer le PGCD de plusieurs nombres
int gcd_multiple(const std::vector<int>& numbers) {
	// Initialiser le GCD avec le premier élément du vecteur
	int result = numbers[0];
	// Utiliser std::accumulate pour appliquer gcd sur tout le vecteur
	result = std::accumulate(numbers.begin(), numbers.end(), result, gcd);
	return result;
}

// Fonction pour générer une période complète du signal
std::vector<float> generate_waveform_with_n_sinus(size_t buffer_size, std::vector<int> frequencies, std::vector<float> amplitudes, int &fundamental_frequency) {
	// Trouver la période fondamentale
	fundamental_frequency = gcd_multiple(frequencies);
	float period = 1.0 / static_cast<float>(fundamental_frequency);

	// Initialiser le buffer
	std::vector<float> waveform(buffer_size, 0.0f);

	// Générer le signal pour une période complète
	float t, dt = period / BUFFER_SIZE;
	for (size_t i = 0; i < buffer_size; ++i) {
		t = static_cast<float>(i) * dt;
		for (size_t j = 0; j < amplitudes.size(); ++j) {
			waveform[i] += amplitudes[j] * sin(2 * M_PI * frequencies[j] * t);
		}
	}

	// Normaliser le signal si nécessaire
	double max_amplitude = waveform[0];
	for (size_t i = 0; i < buffer_size; ++i) {
		if (std::abs(waveform[i]) > max_amplitude) {
			max_amplitude = std::abs(waveform[i]);
		}
	}
	if (max_amplitude > 1.0) {
		for (size_t i = 0; i < buffer_size; ++i) {
			waveform[i] /= max_amplitude;
		}
	}

	return waveform;
}

std::string format(double precision, char fill, float value) {
    int totalWidth = static_cast<int>(precision);
    int fractionalWidth = static_cast<int>((precision - totalWidth) * 10);

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(fractionalWidth);

    // Définir le caractère de remplissage et la largeur totale
    oss << std::setw(totalWidth) << std::setfill(fill) << value;

    return oss.str();
}

std::string format(double precision, char fill, double value) {
    int totalWidth = static_cast<int>(precision);
    int fractionalWidth = static_cast<int>((precision - totalWidth) * 10);

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(fractionalWidth);

    // Définir le caractère de remplissage et la largeur totale
    oss << std::setw(totalWidth) << std::setfill(fill) << value;

    return oss.str();
}

std::string format(int width, char fill, int value) {
    std::ostringstream oss;
    oss << std::setw(width) << std::setfill(fill) << value;
    return oss.str();
}
