#ifndef __UTILS_HPP
#define __UTILS_HPP

#include <string>
#include <vector>
#include <complex>
#include <cmath>
#include "rp.h"

#include "Signal.hpp"
#include "Window.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double modulo(double value, double modulo);

std::string toLower(std::string str);

std::string toUpper(std::string str);

bool stringToBool(const std::string &str);

rp_waveform_t stringToRpWaveform(const std::string &name);

std::string rpWaveformToString(rp_waveform_t waveform);

WindowType stringToWindowType(const std::string &str);

std::string windowTypeToString(WindowType type);

// for string delimiter
std::vector<std::string> split(std::string s, std::string delimiter);

int calculateDecimation(double f, int N_per_period);

int convertToInteger(const std::string& str);

// Fonction pour calculer le PGCD de deux nombres
int gcd(int a, int b);

// Fonction pour calculer le PGCD de plusieurs nombres
int gcd_multiple(const std::vector<int>& numbers);

// Fonction pour générer une période complète du signal
std::vector<float> generate_waveform_with_n_sinus(size_t buffer_size, std::vector<int> frequencies, std::vector<float> amplitudes, int &fundamental_frequency);

std::string format(double precision, char fill, float value);

std::string format(double precision, char fill, double value);

std::string format(int width, char fill, int value);


#endif // __UTILS_HPP