#include "tests.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>
#include <numeric>
#include <complex>
#include <string>
#include <cstring>
#include <chrono>
#include <algorithm>
#include <sys/time.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include "rp.h"
#include "rp_hw-calib.h"
#include "rp_hw-profiles.h"

#include "Signal.hpp"
#include "Spectrum.hpp"
#include "Filter.hpp"
#include "Demodulator.hpp"
#include "PID.hpp"
#include "CSVFile.hpp"
#include "Noise.hpp"
#include "Window.hpp"
#include "globals.hpp"
#include "utils.hpp"
#include "acquisition.hpp"
#include <stdexcept>

int test_acquire(const std::vector<std::string> &args) {
	int result;
	
	try {

		WindowType window_type = WindowType::Rectangular;
		rp_waveform_t waveform = RP_WAVEFORM_SINE;
		int    frequency = 10000;
		float  amplitude = 1.0;
		float  offset = 0;
		float  phase  = 0;
		float  duty_cycle = 0.5;
		bool   hasSetDecimation = false;
		int    points_per_period = 100;
		bool   acquire_on_channel1 = true;
		bool   acquire_on_channel2 = false;
		float  trigger_level = 0.01f;
		int32_t trigger_delay = BUFFER_SIZE / 2;

		if (args.size() >= 1) {
			for (auto param : args) {

				// Vérifier si l'argument est "help"
				if (param == "help") {
					std::cerr << "\033[4;0mHelp message\033[0m" << std::endl;
					std::cerr << "Details:" << std::endl;
					std::cerr << "  This application acquires on analog signal in channel 1 or channel 2, a signal" << std::endl;
					std::cerr << "  generated on analog output 1, in order to perform fft calculation." << std::endl;
					std::cerr << "  The time and frequency signals are written to a csv file." << std::endl;
					std::cerr << "Possibilities of utilisation : " << std::endl;
					std::cerr << "  waveform=<string>; wf=<string>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << rpWaveformToString(waveform) << "." << std::endl;
					std::cerr << "  amplitude=<value_>; a=<value>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << amplitude << "." << std::endl;
					std::cerr << "  frequency=<integer>; f=<integer>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << frequency << "." << std::endl;
					std::cerr << "  offset=<valeur>; off=<valeur>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << offset << "." << std::endl;
					std::cerr << "  phase=<value>; ph=<value>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << phase << "." << std::endl;
					std::cerr << "  duty_cycle=<value>; dtc=<value>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << duty_cycle << "." << std::endl;
					std::cerr << "  window=<string>; win=<string>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << windowTypeToString(window_type) << "." << std::endl;
					std::cerr << "  points_per_period=<integer>; ppp=<integer>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is "<< points_per_period << "." << std::endl;
					std::cerr << "  decimation=<integer>; dec=<integer>" << std::endl;
					std::cerr << "      note: The 'points_per_period' and 'decimation' arguments allow decimation to be defined differently." << std::endl;
					std::cerr << "            The 'points_per_period' argument defines the number of points per period, and the 'decimation' argument defines the decimation factor." << std::endl;
					std::cerr << "  buffsize=<integer>; bs=<integer>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << BUFFER_SIZE << "." << std::endl;
					std::cerr << "  acquire_on_channel1=<boolean>; acq1=<boolean>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << acquire_on_channel1 << "." << std::endl;
					std::cerr << "  acquire_on_channel2=<boolean>; acq2=<boolean>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << acquire_on_channel2 << "." << std::endl;
					std::cerr << "  trigger_level=<float>; trl=<float>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << trigger_level << "." << std::endl;
					std::cerr << "  trigger_delay=<integer>; trd=<integer>" << std::endl;
					std::cerr << "      note: Enter the trigger delay in sample index." << std::endl;
					std::cerr << "            This argument is optional, and if not entered, the default value is " << trigger_delay << "." << std::endl;
					return 0;
				} else {
					// Parse other arguments
					size_t pos = param.find('=');
					if (pos != std::string::npos) {
						std::string name = param.substr(0, pos);
						std::string value = param.substr(pos + 1);

						for (size_t i = 0; i < name.size(); ++i) {
							if (name[i] <= 'Z' && name[i] >= 'A') {
								name[i] = name[i] + 32;
							}
						}

						if (name == "waveform" || name == "wf") {
							waveform = stringToRpWaveform(value);
						} else if (name == "amplitude" || name == "a") {
							amplitude = std::stof(value);
						} else if (name == "frequency" || name == "f") {
							frequency = convertToInteger(value);
						} else if (name == "offset" || name == "off") {
							offset = std::stof(value);
						} else if (name == "phase" || name == "ph") {
							phase = std::stof(value);
						} else if (name == "duty_cycle" || name == "dtc") {
							duty_cycle = std::stof(value);
						} else if (name == "window" || name == "win") {
							window_type = stringToWindowType(value);
						} else if (name == "points_per_period" || name == "ppp") {
							points_per_period = std::stoi(value);
						} else if (name == "decimation" || name == "dec") {
							hasSetDecimation = true;
							SetDecimation(std::stoi(value));
						} else if (name == "buffsize" || name == "bs") {
							SetBufferSize(std::stoi(value));
						} else if (name == "acquire_on_channel1" || name == "acq1") {
							acquire_on_channel1 = stringToBool(value);
						} else if (name == "acquire_on_channel2" || name == "acq2") {
							acquire_on_channel2 = stringToBool(value);
						} else if (name == "trigger_level" || name == "trl") {
							trigger_level = std::stof(value);
						} else if (name == "trigger_delay" || name == "trd") {
							trigger_delay = std::stoi(value);
						} else {
							std::cerr << "Invalid argument format: " << param << std::endl;
							return 1;
						}
					} else {
						std::cerr << "Invalid argument format: " << param << std::endl;
						return 1;
					}
				}
			}
		}

		/* - - - - - - - - - - - - - - - - - - - - - - - */
		
		if (rp_InitReset(true) != RP_OK) {
			throw std::runtime_error("Rp api init failed!");
		}
		
		if (!hasSetDecimation) {
			SetDecimation(calculateDecimation(frequency, points_per_period));
		}

		initAcquisition(trigger_level, trigger_delay);
		
		std::cerr << "+----------- START -------------+" << std::endl;

		/* - - - - - - - - - - - - - - - - - - - - - - - */

		Signal output("output1(t)");
		Signal input1("input1(t)");
		Signal input2("input2(t)");
		Signal windowedSignal1;
		Signal windowedSignal2;
		Spectrum spectrum1("INPUT1(f)");
		Spectrum spectrum2("INPUT2(f)");

		/* - - - - - - - - - - - - - - - - - - - - - - - */
		/* Initialisation de la fenêtre */
		Window window;
		if (!window.set(window_type, BUFFER_SIZE)) {
			std::cerr << "Error: Unable to set the window function." << std::endl;
			return 1;
		}
		window.setup();

		/* - - - - - - - - - - - - - - - - - - - - - - - */

		output.generateWaveform(waveform, amplitude, frequency, phase, offset, 0, duty_cycle*100);
		
		rp_GenReset();
		rp_GenWaveform(RP_CH_1, waveform);

		rp_GenFreq(RP_CH_1, frequency);
		rp_GenAmp(RP_CH_1, amplitude);
		rp_GenOffset(RP_CH_1, offset);
		rp_GenPhase(RP_CH_1, phase);
		rp_GenDutyCycle(RP_CH_1, duty_cycle);

		rp_GenOutEnable(RP_CH_1);
		rp_GenTriggerOnly(RP_CH_1);

		/* - - - - - - - - - - - - - - - - - - - - - - - */

		if (acquire_on_channel1 && !acquire_on_channel2)
			acquisitionChannel1(input1);
		else if (acquire_on_channel2 && !acquire_on_channel1)
			acquisitionChannel2(input2);
		else if (acquire_on_channel1 && acquire_on_channel2)
			acquisitionChannels1_2(input1, input2, RP_T_CH_1);

		std::cerr << "Acquisition successful" << std::endl;

		/* - - - - - - - - - - - - - - - - - - - - - - - */
		
		if (acquire_on_channel1) {
			windowedSignal1 = window.apply(input1);
			windowedSignal1.FFT(spectrum1);
		}
		if (acquire_on_channel2) {
			windowedSignal2 = window.apply(input2);
			windowedSignal2.FFT(spectrum2);
		}
		std::cerr << "FFT successful" << std::endl;

		/* - - - - - - - - - - - - - - - - - - - - - - - */

		std::cerr << "Write results in csv files" << std::endl;

		CSVFile::setTimeToFilepath();
		std::string filename1 = "signals.csv";
		std::string filename2 = "spectrums.csv";
		
		CSVFile outFile1(filename1);
		std::vector<Signal> outSig{output};
		if (acquire_on_channel1) outSig.emplace_back(input1);
		if (acquire_on_channel2) outSig.emplace_back(input2);
		outFile1.writeSignals(outSig, true);

		CSVFile outFile2(filename2);
		std::vector<Spectrum> outSpectrums;
		if (acquire_on_channel1) outSpectrums.emplace_back(spectrum1);
		if (acquire_on_channel2) outSpectrums.emplace_back(spectrum2);
		outFile2.writeSpectrums(outSpectrums, true);

		/* - - - - - - - - - - - - - - - - - - - - - - - */

		std::cerr << "Write descriptions file" << std::endl;

		std::ostringstream oss;
	
		oss << "[program]" << std::endl;
		oss << "type = test" << std::endl;
		oss << "name = acquire" << std::endl;
		oss << "[parameters]"			<< std::endl;
		oss << "# Acquisition parameters" << std::endl;
		oss << "acquire_on_channel1 = " << acquire_on_channel1 << std::endl;
		oss << "acquire_on_channel2 = " << acquire_on_channel2 << std::endl;
		oss << "trigger_level = "		<< trigger_level << std::endl;
		oss << "trigger_delay = "		<< trigger_delay << std::endl;
		double ppp = SAMPLING_FREQUENCY / static_cast<double>(frequency);
		oss << "points_per_period = "	<< ppp << std::endl;
		oss << "decimation = " 			<< DECIMATION << std::endl;
		oss << "buffsize = "			<< BUFFER_SIZE << std::endl;
		oss << " # Generation parameters" << std::endl;
		oss << "waveform = "			<< rpWaveformToString(waveform) << std::endl;
		oss << "amplitude = "			<< amplitude << std::endl;
		oss << "frequency = "			<< frequency << std::endl;
		oss << "offset = "				<< offset << std::endl;
		oss << "phase = "				<< phase << std::endl;
		oss << "duty_cycle = "			<< duty_cycle << std::endl;
		oss << "window = "				<< windowTypeToString(window_type) << std::endl;
		oss << "[files]" << std::endl;
		oss << "file1 = " << filename1 << std::endl;
		oss << "file2 = " << filename2 << std::endl;
		CSVFile::writeDescriptions(oss.str());
		
		releaseAcquisition();
		rp_Release();
		
		std::cerr << "+------------ END --------------+" << std::endl;

		result = 0;
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		result = 1;
	}

	return result;
}

int test_spectrum(const std::vector<std::string> &args)
{
	int result;
	
	try {
		SetBufferSize(ADC_BUFFER_SIZE);
		SetDecimation(16);

		std::vector<int> frequencies  = {10000, 100000};
		std::vector<float> amplitudes = { 0.8, 0.2};
		float offset = 0;
		float phase  = 0;
		double noise = 0.0;
		WindowType window_type  = WindowType::Hamming;
		bool hasSetDecimation = false;
		int points_per_period = 100;
		float  trigger_level = 0.01f;
		int32_t trigger_delay = BUFFER_SIZE/2;

		if (args.size() >= 1) {
			for (auto param : args) {

				// Vérifier si l'argument est "help"
				if (param == "help") {
					std::cerr << "\033[4;0mHelp message\033[0m" << std::endl;
					std::cerr << "Details:" << std::endl;
					std::cerr << "  This application acquires on analog input 1, a signal" << std::endl;
					std::cerr << "  generated on analog output 1, in order to perform" << std::endl;
					std::cerr << "  fft calculation." << std::endl;
					std::cerr << "  The time and frequency signals are written to a csv file." << std::endl;
					std::cerr << "Possibilities of utilisation : " << std::endl;
					std::cerr << "  amplitudes=<value_1,value_2,...,value_n>; a=<value_1,value_2,...,value_n>" << std::endl;
					std::cerr << "  frequencies=<integer_1,integer_2,...,integer_n>; f=<integer_1,integer_2,...,integer_n>" << std::endl;
					std::cerr << "      note: The number of frequencies and amplitudes entered must be the same." << std::endl;
					std::cerr << "            Values ​​must be separated by a comma." << std::endl;
					std::cerr << "  offset=<valeur>; off=<valeur>" << std::endl;
					std::cerr << "      note: This argument is optional, and if not entered, the default value is " << offset << "." << std::endl;
					std::cerr << "  phase=<value>; ph=<value>" << std::endl;
					std::cerr << "      note: This argument is optional, and if not entered, the default value is " << phase << "." << std::endl;
					std::cerr << "  noise=<value>; ns=<value>" << std::endl;
					std::cerr << "      note: This argument is optional, and if not entered, the default value is " << noise << "." << std::endl;
					std::cerr << "  window=<string>; win=<string>" << std::endl;
					std::cerr << "      note: This argument is optional, and if not entered, the default value is " << windowTypeToString(window_type) << "." << std::endl;
					std::cerr << "  points_per_period=<integer>; ppp=<integer>" << std::endl;
					std::cerr << "      note: This argument is optional, and if not entered, the default value is " << points_per_period << "." << std::endl;
					std::cerr << "  decimation=<integer>; dec=<integer>" << std::endl;
					std::cerr << "      note: The 'points_per_period' and 'decimation' arguments allow decimation to be defined differently." << std::endl;
					std::cerr << "            The 'points_per_period' argument defines the number of points per period, and the 'decimation' argument defines the decimation factor." << std::endl;
					std::cerr << "  buffsize=<integer>; bs=<integer>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << ADC_BUFFER_SIZE << std::endl;
					std::cerr << "  trigger_level=<float>; trl=<float>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << trigger_level << "." << std::endl;
					std::cerr << "  trigger_delay=<integer>; trd=<integer>" << std::endl;
					std::cerr << "      note: Enter the trigger delay in sample index." << std::endl;
					std::cerr << "            This argument is optional, and if not entered, the default value is " << trigger_delay << "." << std::endl;
					return 0;
				} else {
					// Parse other arguments
					size_t pos = param.find('=');
					if (pos != std::string::npos) {
						std::string name = param.substr(0, pos);
						std::string value = param.substr(pos + 1);

						for (size_t i = 0; i < name.size(); ++i) {
							if (name[i] <= 'Z' && name[i] >= 'A') {
								name[i] = name[i] + 32;
							}
						}

						if (name == "amplitudes" || name == "a") {
							std::vector<std::string> values = split(value, ",");
							amplitudes.clear();
							for (const std::string& v : values) {
								amplitudes.push_back(std::stof(v));
							}
						} else if (name == "frequencies" || name == "f") {
							std::vector<std::string> values = split(value, ",");
							frequencies.clear();
							for (const std::string& v : values) {
								frequencies.push_back(convertToInteger(v));
							}
						} else if (name == "offset" || name == "off") {
							offset = std::stof(value);
						} else if (name == "phase" || name == "ph") {
							phase = std::stof(value);
						} else if (name == "noise" || name == "ns") {
							noise = std::stof(value);
						} else if (name == "window" || name == "win") {
							window_type = stringToWindowType(value);
						} else if (name == "points_per_period" || name == "ppp") {
							points_per_period = convertToInteger(value);
						} else if (name == "decimation" || name == "dec") {
							hasSetDecimation = true;
							SetDecimation(std::stoi(value));
						} else if (name == "buffsize" || name == "bs") {
							SetBufferSize(convertToInteger(value));
						} else if (name == "trigger_level" || name == "trl") {
							trigger_level = std::stof(value);
						} else if (name == "trigger_delay" || name == "trd") {
							trigger_delay = std::stoi(value);
						} else {
							std::cerr << "Invalid argument format: " << param << std::endl;
							return 1;
						}
					} else {
						std::cerr << "Invalid argument format: " << param << std::endl;
						return 1;
					}
				}
			}
		}
		if (amplitudes.size() != frequencies.size()) {
			std::cerr << "Error: Amplitudes and frequencies must be specified and have the same size." << std::endl;
			return 1;
		}
		
		int max_frequency = 0;
		for (double f : frequencies) {
			if (f > max_frequency) {
				max_frequency = f;
			}
		}

		/* Print error, if rp_Init() function failed */
		if (rp_InitReset(true) != RP_OK) {
			throw std::runtime_error("Rp api init failed!");
		}
		initAcquisition(trigger_level, trigger_delay);
		
		std::cerr << "+----------- START -------------+" << std::endl;
		
		if (!hasSetDecimation) {
			SetDecimation(calculateDecimation(max_frequency, points_per_period));
		}

		/* - - - - - - - - - - - - - - - - - - - - - - - */
		/* Génération du signal arbitraire */
		int fundamental_frequency;
		std::vector<float> waveform = generate_waveform_with_n_sinus(BUFFER_SIZE, frequencies, amplitudes, fundamental_frequency);

		/* - - - - - - - - - - - - - - - - - - - - - - - */

		Signal signal("signal(t)");
		Signal windowedSignal;
		Spectrum spectrum("spectrum(f)");

		/* - - - - - - - - - - - - - - - - - - - - - - - */

		/* Initialisation de la fenêtre */
		Window window;
		if (!window.set(window_type, BUFFER_SIZE)) {
			std::cerr << "Error: Unable to set the window function." << std::endl;
			return 1;
		}
		window.setup();

		/* - - - - - - - - - - - - - - - - - - - - - - - */
		/* Configuration du générateur de tension arbitraire */

		rp_GenReset();
		rp_GenWaveform(RP_CH_1, RP_WAVEFORM_ARBITRARY);
		rp_GenArbWaveform(RP_CH_1, waveform.data(), BUFFER_SIZE);
		rp_GenFreq(RP_CH_1, fundamental_frequency);
		rp_GenAmp(RP_CH_1, 1.0f);
		rp_GenOffset(RP_CH_1, offset);
		rp_GenPhase(RP_CH_1, phase);
		
		rp_GenOutEnable(RP_CH_1);
		rp_GenTriggerOnly(RP_CH_1);
		
		/* - - - - - - - - - - - - - - - - - - - - - - - */
		/* Echantillonnage du signal */
		acquisitionChannel1(signal);
		std::cerr << "Acquisition successful" << std::endl;
		windowedSignal = window.apply(signal);

		/* Calcul des transformées de fourier discrètes des signaux avec BUFFER_SIZE zero padding */
		
		windowedSignal.FFT(spectrum);
		std::cerr << "FFT calculation successful" << std::endl;

		/* - - - - - - - - - - - - - - - - - - - - - - - */
		/* Sauvgarde des résultats */

		std::cerr << "Write results in csv files" << std::endl;

		CSVFile::setTimeToFilepath();
		std::string filename1 = "signals.csv";
		std::string filename2 = "spectrums.csv";
		
		CSVFile outFile1(filename1);
		std::vector<Signal> outSpSig = {signal};
		outFile1.writeSignals(outSpSig, true);
		std::cerr << "File: " << filename1 << std::endl;

		CSVFile outFile2(filename2);
		std::vector<Spectrum> outSpectrum = {spectrum};
		outFile2.writeSpectrums(outSpectrum, true);
		std::cerr << "File: " << filename2 << std::endl;

		/* - - - - - - - - - - - - - - - - - - - - - - - */

		std::cerr << "Write descriptions file" << std::endl;

		std::ostringstream oss;
	
		oss << "[program]" << std::endl;
		oss << "type = test" << std::endl;
		oss << "name = spectrum" << std::endl;	
		oss << "[parameters]" << std::endl;	
		oss << "# Acquisition parameters" << std::endl;
		oss << "smpling_frequency = "		<< SAMPLING_FREQUENCY << std::endl;
		oss << "decimation = "				<< DECIMATION << std::endl;
		oss << "buffsize = "				<< BUFFER_SIZE << std::endl;
		double T_per_period = 1.0 / max_frequency;
		double ppp = T_per_period * SAMPLING_FREQUENCY;
		oss << "points_per_period = "		<< ppp << std::endl;
		oss << "trigger_level = "			<< trigger_level << std::endl;
		oss << "trigger_delay = "			<< trigger_delay << std::endl;
		oss << "# Signal parameters" << std::endl;
		oss << "amplitudes = { ";
		for (double a : amplitudes) { oss << a << " "; }
		oss << "}" << std::endl;
		
		oss << "frequencies = { ";
		for (int f : frequencies) { oss  << f << " "; }
		oss << "}" << std::endl;
		
		oss << "fundamental = "				<< fundamental_frequency << std::endl;
		oss << "noise = "					<< noise << std::endl;
		oss << "offset = "					<< offset << std::endl;
		oss << "phase = "					<< phase << std::endl;
		oss << "window = "					<< windowTypeToString(window_type) << std::endl;
		oss << "[files]" << std::endl;
		oss << "file1 = " << filename1 << std::endl;
		oss << "file2 = " << filename2 << std::endl;
		CSVFile::writeDescriptions(oss.str());

		releaseAcquisition();
		rp_Release();
		
		std::cerr << "+------------ END --------------+" << std::endl;

		result = 0;
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		result = 1;
	}

	return result;
}

/* - - - - - - - - - - - - - - - - - - - - - - - */

int test_demodulation(const std::vector<std::string> &args) {
	int result;
	
	try {
		srand( time( NULL ) );

		SetBufferSize(ADC_BUFFER_SIZE);
		SetDecimation(16);

		std::vector<int> frequencies  = {10000, 100000};
		std::vector<float> amplitudes = { 0.8, 0.2};
		float offset        = 0;
		float phase         = 0;
		double noise        = 0.0;
		WindowType window_type  = WindowType::Hamming;
		double dem_filter_freq = 3e3;
		bool hasSetDecimation = false;
		int points_per_period = 100;
		bool acquire_on_channel1 = true;
		bool acquire_on_channel2 = false;
		float  trigger_level = 0.01f;
		int32_t trigger_delay = BUFFER_SIZE/2;
		
		if (args.size() >= 1) {
			for (auto param : args) {

				// Vérifier si l'argument est "help"
				if (param == "help") {
					std::cerr << "\033[4;0mHelp message\033[0m" << std::endl;
					std::cerr << "Details:" << std::endl;
					std::cerr << "  This application acquires on analog input 1, a signal" << std::endl;
					std::cerr << "  generated on analog output 1, in order to perform" << std::endl;
					std::cerr << "  demodulation and fft calculation." << std::endl;
					std::cerr << "  The time and frequency signals are written to a csv file." << std::endl;
					std::cerr << "Possibilities of utilisation : " << std::endl;
					std::cerr << "  amplitudes=<value_1,value_2,...,value_n>; a=<value_1,value_2,...,value_n>" << std::endl;
					std::cerr << "  frequencies=<integer_1,integer_2,...,integer_n>; f=<integer_1,integer_2,...,integer_n>" << std::endl;
					std::cerr << "      note: The number of frequencies and amplitudes entered must be the same." << std::endl;
					std::cerr << "            Values ​​must be separated by a comma." << std::endl;
					std::cerr << "  offset=<valeur>; off=<valeur>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << offset << "." << std::endl;
					std::cerr << "  phase=<value>; ph=<value>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << phase << "." << std::endl;
					std::cerr << "  noise=<value>; ns=<value>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is 0.0" << std::endl;
					std::cerr << "  window=<string>; win=<string>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is Hamming." << std::endl;
					std::cerr << "  dem_filter_freq=<value>; demff=<value>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is 3000." << std::endl;
					std::cerr << "  points_per_period=<integer>; ppp=<integer>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is 100." << std::endl;
					std::cerr << "  decimation=<integer>; dec=<integer>" << std::endl;
					std::cerr << "      note: the 'points_per_period' and 'decimation' arguments allow decimation to be defined differently." << std::endl;
					std::cerr << "            The 'points_per_period' argument defines the number of points per period, and the 'decimation' argument defines the decimation factor." << std::endl;
					std::cerr << "  buffsize=<integer>; bs=<integer>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is 1024." << std::endl;
					std::cerr << "  acquire_on_channel1=<boolean>; acq1=<boolean>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is true." << std::endl;
					std::cerr << "  acquire_on_channel2=<boolean>; acq2=<boolean>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is false." << std::endl;
					std::cerr << "  trigger_level=<float>; trl=<float>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << trigger_level << "." << std::endl;
					std::cerr << "  trigger_delay=<integer>; trd=<integer>" << std::endl;
					std::cerr << "      note: Enter the trigger delay in sample index." << std::endl;
					std::cerr << "            This argument is optional, and if not entered, the default value is " << trigger_delay << "." << std::endl;
					return 0;
				} else {
					// Parse other arguments
					size_t pos = param.find('=');
					if (pos != std::string::npos) {
						std::string name = param.substr(0, pos);
						std::string value = param.substr(pos + 1);

						for (size_t i = 0; i < name.size(); ++i) {
							if (name[i] <= 'Z' && name[i] >= 'A') {
								name[i] = name[i] + 32;
							}
						}

						if (name == "amplitudes" || name == "a") {
							std::vector<std::string> values = split(value, ",");
							amplitudes.clear();
							for (const std::string& v : values) {
								amplitudes.push_back(std::stof(v));
							}
						} else if (name == "frequencies" || name == "f") {
							std::vector<std::string> values = split(value, ",");
							frequencies.clear();
							for (const std::string& v : values) {
								frequencies.push_back(convertToInteger(v));
							}
						} else if (name == "offset" || name == "off") {
							offset = std::stof(value);
						} else if (name == "phase" || name == "ph") {
							phase = std::stof(value);
						} else if (name == "noise" || name == "ns") {
							noise = std::stof(value);
						} else if (name == "window" || name == "win") {
							window_type = stringToWindowType(value);
						} else if (name == "dem_filter_freq" || name == "demff") {
							dem_filter_freq = convertToInteger(value);
						} else if (name == "points_per_period" || name == "ppp") {
							points_per_period = convertToInteger(value);
						} else if (name == "decimation" || name == "dec") {
							hasSetDecimation = true;
							SetDecimation(std::stoi(value));
						} else if (name == "buffsize" || name == "bs") {
							SetBufferSize(convertToInteger(value));
						} else if (name == "acquire_on_channel1" || name == "acq1") {
							acquire_on_channel1 = stringToBool(value);
						} else if (name == "acquire_on_channel2" || name == "acq2") {
							acquire_on_channel2 = stringToBool(value);
						} else if (name == "trigger_level" || name == "trl") {
							trigger_level = std::stof(value);
						} else if (name == "trigger_delay" || name == "trd") {
							trigger_delay = std::stoi(value);
						} else {
							std::cerr << "Invalid argument: " << param << std::endl;
							return 1;
						}
					} else {
						std::cerr << "Invalid argument format: " << param << std::endl;
						return 1;
					}
				}
			}
		}
		if (amplitudes.size() != frequencies.size()) {
			std::cerr << "Error: Amplitudes and frequencies must be specified and have the same size." << std::endl;
			return 1;
		}
		if (acquire_on_channel1 == false && acquire_on_channel2 == false) {
			std::cerr << "Error: At least one channel must be acquired." << std::endl;
			return 1;
		}

		/* Print error, if rp_Init() function failed */
		if (rp_InitReset(true) != RP_OK) {
			throw std::runtime_error("Rp api init failed!");
		}
		initAcquisition(trigger_level, trigger_delay);
		
		std::cerr << "+----------- START -------------+" << std::endl;
		
		int max_frequency = frequencies[0];
		int min_frequency = frequencies[0];
		for (double f : frequencies) {
			if (f > max_frequency) {
				max_frequency = f;
			}
			if (f < min_frequency) {
				min_frequency = f;
			}
		}
		
		if (!hasSetDecimation) {
			SetDecimation(calculateDecimation(max_frequency, points_per_period));
		}

		/* - - - - - - - - - - - - - - - - - - - - - - - */
		/* génération du signal arbitraire */
		int fundamental_frequency;
		std::vector<float> waveform = generate_waveform_with_n_sinus(BUFFER_SIZE, frequencies, amplitudes, fundamental_frequency);

		Signal signal1("signal1(t)");
		Signal signal2("signal2(t)");
		Signal windowedSignal1;
		Signal windowedSignal2;
		Signal signal_demAmpli1("amplitude1(t)");
		Signal signal_demAmpli2("amplitude2(t)");
		Signal signal_demPhase1("phase1(t)");
		Signal signal_demPhase2("phase2(t)");

		Spectrum spectrum1("SIGNAL1(f)");
		Spectrum spectrum2("SIGNAL2(f)");
		Spectrum spectrum_demAmpli1("AMPLITUDE1(f)");
		Spectrum spectrum_demAmpli2("AMPLITUDE2(f)");
		
		/* - - - - - - - - - - - - - - - - - - - - - - - */
		/* Initialisation de la démodulation */
		double freq_oscillator = static_cast<double>(min_frequency); // fréquence de l'oscillateur
		Demodulator dem(dem_filter_freq, freq_oscillator);
		dem.setup();

		/* Initialisation de la fenêtre */
		Window window;
		if (!window.set(window_type, BUFFER_SIZE)) {
			std::cerr << "Error: Unable to set the window function." << std::endl;
			return 1;
		}
		window.setup();

		/* - - - - - - - - - - - - - - - - - - - - - - - */
		/* Configuration du générateur de tension arbitraire */

		rp_GenReset();
		rp_GenWaveform(RP_CH_1, RP_WAVEFORM_ARBITRARY);
		rp_GenArbWaveform(RP_CH_1, waveform.data(), BUFFER_SIZE);
		rp_GenFreq(RP_CH_1, fundamental_frequency);
		rp_GenAmp(RP_CH_1, 1.0f);
		rp_GenOffset(RP_CH_1, offset);
		rp_GenPhase(RP_CH_1, phase);
		
		rp_GenOutEnable(RP_CH_1);
		rp_GenTriggerOnly(RP_CH_1);
		
		/* - - - - - - - - - - - - - - - - - - - - - - - */
		/* Echantillonnage du signal */
		
		if (acquire_on_channel1 && !acquire_on_channel2) acquisitionChannel1(signal1);
		else if (acquire_on_channel2 && !acquire_on_channel1) acquisitionChannel2(signal2);
		else acquisitionChannels1_2(signal1, signal2, RP_T_CH_2);
		std::cerr << "Acqusition successful" << std::endl;
		
		if (acquire_on_channel1) windowedSignal1 = window.apply(signal1);
		if (acquire_on_channel2) windowedSignal2 = window.apply(signal2);
		
		/* - - - - - - - - - - - - - - - - - - - - - - - */
		/* Démodulation du signal */
		
		if (acquire_on_channel1) dem.apply(signal1, signal_demAmpli1, signal_demPhase1, true);
		if (acquire_on_channel2) dem.apply(signal2, signal_demAmpli2, signal_demPhase2, true);
		std::cerr << "Demodulation successful" << std::endl;

		/* - - - - - - - - - - - - - - - - - - - - - - - */
		/* Calcul des transformées de fourier discrètes des signaux avec BUFFER_SIZE zero padding */
		
		if (acquire_on_channel1) windowedSignal1.FFT(spectrum1);
		if (acquire_on_channel2) windowedSignal2.FFT(spectrum2);

		std::cerr << "Calculation fft signals successful" << std::endl;
		
		float rising_time = 3/dem_filter_freq;
		size_t index = rising_time * SAMPLING_FREQUENCY;

		std::cerr << "Rising time: " << std::setw(11) << rising_time << " s " << std::endl;
		if (acquire_on_channel1) signal1.FFT(spectrum_demAmpli1, index);
		if (acquire_on_channel2) signal2.FFT(spectrum_demAmpli2, index);

		std::cerr << "Calculation fft amplitudes successful" << std::endl;

		/* - - - - - - - - - - - - - - - - - - - - - - - */
		/* Sauvgarde des signaux */

		std::cerr << "Write results in csv files" << std::endl;

		CSVFile::setTimeToFilepath();
		std::string filename1 = "signals.csv";
		std::string filename2 = "spectrums.csv";
		std::string filename3 = "spectrums_amplitudes.csv";

		CSVFile outFile1(filename1);
		std::vector<Signal> outSpSig;
		if (acquire_on_channel1) {
			outSpSig.emplace_back(signal1);
			outSpSig.emplace_back(signal_demAmpli1);
			outSpSig.emplace_back(signal_demPhase1);
		}
		if (acquire_on_channel2) {
			outSpSig.emplace_back(signal2);
			outSpSig.emplace_back(signal_demAmpli2);
			outSpSig.emplace_back(signal_demPhase2);
		}
		outFile1.writeSignals(outSpSig, true);
		std::cerr << "File: " << filename1  << std::endl;

		CSVFile outFile2(filename2);
		std::vector<Spectrum> outSpectrum;
		if (acquire_on_channel1) outSpectrum.emplace_back(spectrum1);
		if (acquire_on_channel2) outSpectrum.emplace_back(spectrum2);
		outFile2.writeSpectrums(outSpectrum, true, false);
		std::cerr << "File: " << filename2  << std::endl;

		CSVFile outFile3(filename3);
		std::vector<Spectrum> outSpectrum2;
		if (acquire_on_channel1) outSpectrum2.emplace_back(spectrum_demAmpli1);
		if (acquire_on_channel2) outSpectrum2.emplace_back(spectrum_demAmpli2);
		outFile3.writeSpectrums(outSpectrum2, true, false);
		std::cerr << "File: " << filename3  << std::endl;

		/* - - - - - - - - - - - - - - - - - - - - - - - */

		std::cerr << "Write descriptions file" << std::endl;

		std::ostringstream oss;
	
		oss << "[program]" << std::endl;
		oss << "type = test" << std::endl;
		oss << "name = demodulation" << std::endl;	
		oss << "[parameters]" << std::endl;
		oss << "# Acquisition Parameters" << std::endl;
		double T_per_period = 1.0 / max_frequency;
		double ppp = T_per_period * SAMPLING_FREQUENCY;
		oss << "points_per_period = "		<< ppp << std::endl;
		oss << "smpling_frequency = "		<< SAMPLING_FREQUENCY << std::endl;
		oss << "decimation = "				<< DECIMATION << std::endl;
		oss << "buffsize = "				<< BUFFER_SIZE << std::endl;
		oss << "trigger_level = "			<< trigger_level << std::endl;
		oss << "trigger_delay = "			<< trigger_delay << std::endl;
		oss << "# Signal Parameters" << std::endl;
		oss << "amplitudes = { ";
		for (double a : amplitudes) { oss << a << " "; }
		oss << "}" << std::endl;
		
		oss << "frequencies = { ";
		for (int f : frequencies) { oss  << f << " "; }
		oss << "}" << std::endl;
		oss << "fundamental = "				<< fundamental_frequency  << std::endl;
		oss << "offset = "					<< offset << std::endl;
		oss << "phase = "					<< phase << std::endl;
		oss << "noise = "					<< noise << std::endl;
		oss << "window = "					<< windowTypeToString(window_type) << std::endl;
		oss << "demodulation_filter_frequency = " << dem_filter_freq << std::endl;
		oss << "[files]" << std::endl;
		oss << "file1 = " << filename1 << std::endl;
		oss << "file2 = " << filename2 << std::endl;
		oss << "file3 = " << filename3 << std::endl;
		CSVFile::writeDescriptions(oss.str());

		std::cerr << "+------------ END --------------+" << std::endl;

		result = 0;
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		result = 1;
	}
	releaseAcquisition();
	rp_Release();
	return result;
}

/* - - - - - - - - - - - - - - - - - - - - - - - */

int test_demodulation2(const std::vector<std::string> &args) {
	int result;
	
	try {
		SetBufferSize(ADC_BUFFER_SIZE);

		int   frequency1        = 10000;
		int   frequency_min     = 100;
		int   frequency_max     = 10000;
		int   number_of_frequencies_in_the_interval = 21;
		float amplitude1        = 0.8;
		float amplitude2        = 0.2;
		float offset            = 0;
		float phase             = 0;
		double noise            = 0.0;
		WindowType window_type  = WindowType::Hamming;
		double dem_filter_freq  = 1e3;
		bool hasSetDecimation   = false;
		int points_per_period   = 100;
		
		if (args.size() >= 1) {
			for (auto param : args) {

				// Vérifier si l'argument est "help"
				if (param == "help") {
					std::cerr << "\033[4;0mHelp message\033[0m" << std::endl;
					std::cerr << "Details:" << std::endl;
					std::cerr << "  " << std::endl;
					std::cerr << "Possibilities of utilisation : " << std::endl;
					std::cerr << "  frequency1=<integer>; f1=<integer>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << frequency1 << "." << std::endl;
					std::cerr << "  frequency_min=<integer>; fmin=<integer>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << frequency_min << "." << std::endl;
					std::cerr << "  frequency_max=<integer>; fmax=<integer>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << frequency_max << "." << std::endl;
					std::cerr << "  number_of_frequencies_in_the_interval=<integer>; nfi=<integer>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << number_of_frequencies_in_the_interval << "." << std::endl;
					std::cerr << "  amplitude1=<valeur>; a1=<valeur>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << amplitude1 << "." << std::endl;
					std::cerr << "  amplitude2=<valeur>; a2=<valeur>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << amplitude2 << "." << std::endl;
					std::cerr << "  offset=<valeur>; off=<valeur>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << offset << "." << std::endl;
					std::cerr << "  phase=<value>; ph=<value>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << phase << "." << std::endl;
					std::cerr << "  noise=<value>; n=<value>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << noise << "." << std::endl;
					std::cerr << "  window_type=<string>; wt=<string>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << windowTypeToString(window_type) << "." << std::endl;
					std::cerr << "  dem_filter_freq=<integer>; demff=<integer>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << dem_filter_freq << "." << std::endl;
					std::cerr << "  points_per_period=<integer>; ppp=<integer>" << std::endl;
					std::cerr << "      details: Number of points per period of the sampled signal" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << points_per_period << "." << std::endl;
					std::cerr << "  decimation=<integer>; dec=<integer>" << std::endl;
					std::cerr << "      details : 125MHz quartz frequency decimation factor" << std::endl;
					std::cerr << "      note: decimation must be a power of 2" << std::endl;
					std::cerr << "  buffsize=<integer>; bs=<integer>" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << BUFFER_SIZE << "." << std::endl;
					return 0;
				} else {
					// Parse other arguments
					size_t pos = param.find('=');
					if (pos != std::string::npos) {
						std::string name = param.substr(0, pos);
						std::string value = param.substr(pos + 1);
						
						// Compare the parameter name to retrieve the value
						if (name == "frequency1" || name == "f1") {
							frequency1 = convertToInteger(value);
						} else if (name == "frequency_min" || name == "fmin") {
							frequency_min = convertToInteger(value);
						} else if (name == "frequency_max" || name == "fmax") {
							frequency_max = convertToInteger(value);
						} else if (name == "number_of_frequencies_in_the_interval" || name == "nfi") {
							number_of_frequencies_in_the_interval = std::stod(value);
						} else if (name == "amplitude1" || name == "a1") {
							amplitude1 = std::stod(value);
						} else if (name == "amplitude2" || name == "a2") {
							amplitude2 = std::stod(value);
						} else if (name == "offset" || name == "off") {
							offset = std::stod(value);
						} else if (name == "phase" || name == "ph") {
							phase = std::stod(value);
						} else if (name == "noise" || name == "n") {
							noise = std::stod(value);
						} else if (name == "window_type" || name == "wt") {
							window_type = stringToWindowType(value);
						} else if (name == "decimation" || name == "d") {
							hasSetDecimation = true;
							SetDecimation(std::stoi(value));
						} else if (name == "points_per_period" || name == "ppp") {
							points_per_period = convertToInteger(value);
						} else if (name == "dem_filter_freq" || name == "demff") {
							dem_filter_freq = convertToInteger(value);
						} else if (name == "buffsize" || name == "bs") {
							SetBufferSize(convertToInteger(value));
						} else {
							std::cerr << "Invalid argument format: " << param << std::endl;
							return 1;
						}
					} else {
						std::cerr << "Invalid argument format: " << param << std::endl;
						return 1;
					}
				}
			}
		}

		/* Print error, if rp_Init() function failed */
		if (rp_InitReset(true) != RP_OK) {
			std::runtime_error("Rp api init failed!");
		}
		initAcquisition();

		std::cerr << "+----------- START -------------+" << std::endl;

		// on calcul la décimation si elle n'a pas été définie
		if (!hasSetDecimation)
			SetDecimation(calculateDecimation(frequency1, points_per_period));

		/* - - - - - - - - - - - - - - - - - - - - - - - */

		std::vector<Signal> outSig;
		std::vector<Signal> outAmp;
		std::vector<Signal> outPha;
		std::vector<Spectrum> outSpAmp;
		std::vector<Spectrum> outSpPha, outSpSig;

		Signal signal;
		Signal signal_ampli;
		Signal signal_phase;

		Spectrum FFT_sig;
		Spectrum FFT_ampli;

		/* - - - - - - - - - - - - - - - - - - - - - - - */
		/* Initialisation de la démodulation */
		Demodulator dem(dem_filter_freq, frequency1);
		dem.setup();
		std::cerr << "Start of process" << std::endl;

		std::vector<int> sequence_frequencies;
		int freq_in_interval = frequency_max - frequency_min;
		double step = freq_in_interval / (number_of_frequencies_in_the_interval-1);
		for (double freq = static_cast<double>(frequency_min); freq < static_cast<double>(frequency_max); freq += step) {
			sequence_frequencies.push_back(static_cast<int>(std::round(freq)));
		}

		int frequency2;
		for (int freq : sequence_frequencies) {
			frequency2 = frequency1 + freq;

			std::cerr << "\rfrequency2 : " << frequency2 << " Hz " << std::flush;

			std::string fstr = std::to_string(frequency2);
			signal.setName("signal_" + fstr + "(t)");
			signal.resize(BUFFER_SIZE, 0.0);
			signal_ampli.setName("amplitude_" + fstr + "(t)");
			signal_ampli.resize(BUFFER_SIZE, 0.0);
			signal_phase.setName("phase_" + fstr + "(t)");
			signal_phase.resize(BUFFER_SIZE, 0.0);

			/* - - - - - - - - - - - - - - - - - - - - - - - */
			/* Configuration du générateur de tension */        
			std::vector<int> freqs = {frequency1, frequency2};
			std::vector<float> amps = {amplitude1, amplitude2};
			int fundamental_frequency;
			std::vector<float> waveform = generate_waveform_with_n_sinus(BUFFER_SIZE, freqs, amps, fundamental_frequency);
			
			rp_GenReset();
			rp_GenWaveform(RP_CH_1, RP_WAVEFORM_ARBITRARY);
			rp_GenArbWaveform(RP_CH_1, waveform.data(), BUFFER_SIZE);
			rp_GenFreq(RP_CH_1, fundamental_frequency);
			rp_GenAmp(RP_CH_1, 1.0f);
			rp_GenOffset(RP_CH_1, offset);
			rp_GenPhase(RP_CH_1, phase);
			
			rp_GenOutEnable(RP_CH_1);
			rp_GenTriggerOnly(RP_CH_1);
			
			/* - - - - - - - - - - - - - - - - - - - - - - - */
			/* Echantillonnage du signal */
			acquisitionChannel1(signal);

			/* - - - - - - - - - - - - - - - - - - - - - - - */
			/* Démodulation du signal */
			//std::cerr << "---- Demodulate ----" << std::endl;
			dem.apply(signal, signal_ampli, signal_phase);

			outSig.push_back(signal);
			outAmp.push_back(signal_ampli);
			outPha.push_back(signal_phase);
		}
		std::cerr << "\nEnd of process" << std::endl;

		/* - - - - - - - - - - - - - - - - - - - - - - - */
		/* Calcul de la durée de rising time max */
		double T_per_period = 1.0 / frequency1;
		size_t max_high_index = 0;
		for (size_t idSig = 0; idSig < outSig.size(); idSig++) {
			size_t low_index, high_index;
			// on prend une marge en multipliant par 2 le temps en régime transitoire pour s'assurer que la dft est effectuée seulement sur le régime stable
			float rising_time = outSig[idSig].getRisingTime(low_index, high_index);
			high_index = low_index + (rising_time/T_per_period) * 5; // régime transitoire à 5 tau
			if (high_index > max_high_index) {
				max_high_index = high_index;
			}
			
		}
		std::cerr << "Rising Time : " << max_high_index*SAMPLING_FREQUENCY << " s " << std::endl;

		/* - - - - - - - - - - - - - - - - - - - - - - - */
		/* Calcul des transformées de fourier discrètes des signaux avec BUFFER_SIZE zero padding */
		std::cerr << "Start fft calculation" << std::endl;
		for (size_t idSig = 0; idSig < outSig.size(); idSig++) {
			std::cerr << "\rFrequency : " << sequence_frequencies[idSig] << " Hz     " << std::flush;
			FFT_sig.setName("SIGNAL_" + std::to_string(sequence_frequencies[idSig]) + "(f)");
			FFT_ampli.setName("AMPLITUDE_" + std::to_string(sequence_frequencies[idSig]) + "(f)");
			outAmp[idSig].FFT(FFT_ampli, max_high_index);
			outSig[idSig].FFT(FFT_sig);
			outSpAmp.push_back(FFT_ampli);
			outSpSig.push_back(FFT_sig);
		}
		std::cerr << "\nEnd fft calculation" << std::endl;

		/* - - - - - - - - - - - - - - - - - - - - - - - */
		/* Sauvgarde des signaux */

		std::cerr << "Write results in csv files" << std::endl;

		CSVFile::setTimeToFilepath();
		std::string filename1 = "signals.csv";
		std::string filename2 = "signals_amplitudes.csv";
		std::string filename3 = "signals_phases.csv";
		std::string filename4 = "spectrums.csv";
		std::string filename5 = "spectrums_amplitudes.csv";

		CSVFile outFile1(filename1);
		outFile1.writeSignals(outSig, true);
		std::cerr << "File: " << filename1  << std::endl;

		CSVFile outFile2(filename2);
		outFile2.writeSignals(outAmp, true);
		std::cerr << "File: " << filename2  << std::endl;

		CSVFile outFile3(filename3);
		outFile3.writeSignals(outPha, true);
		std::cerr << "File: " << filename3  << std::endl;

		CSVFile outFile4(filename4);
		outFile4.writeSpectrums(outSpSig, true);
		std::cerr << "File: " << filename4  << std::endl;

		CSVFile outFile5(filename5);
		outFile5.writeSpectrums(outSpAmp, true);
		std::cerr << "File: " << filename5  << std::endl;

		/* - - - - - - - - - - - - - - - - - - - - - - - */

		std::cerr << "Write descriptions file" << std::endl;

		std::ostringstream oss;
	
		oss << "[program]" << std::endl;
		oss << "type = test" << std::endl;
		oss << "name = demodulation2" << std::endl;	
		oss << "[parameters]"				<< std::endl;
		oss << "frequency1 = "				<< frequency1 << std::endl;
		oss << "frequency_min = "			<< frequency_min << std::endl;
		oss << "frequency_max = " 			<< frequency_max << std::endl;
		oss << "number_of_frequencies_in_the_interval = " << number_of_frequencies_in_the_interval << std::endl;
		oss << "amplitude1 = " 				<< amplitude1 << std::endl;
		oss << "amplitude2 = " 				<< amplitude2 << std::endl;
		oss << "offset = "					<< offset << std::endl;
		oss << "phase = "					<< phase << std::endl;
		oss << "noise = "					<< noise << std::endl;
		oss << "window = "					<< windowTypeToString(window_type) << std::endl;
		oss << "demodulation_filter_frequency = " << dem_filter_freq << std::endl;
		double ppp = T_per_period * SAMPLING_FREQUENCY;
		oss << "points_per_period = "		<< ppp << std::endl;
		oss << "smpling_frequency = "		<< SAMPLING_FREQUENCY << std::endl;
		oss << "decimation = "				<< DECIMATION << std::endl;
		oss << "buffsize = "				<< BUFFER_SIZE << std::endl;
		oss << "[files]" << std::endl;
		oss << "file1 = " << filename1 << std::endl;
		oss << "file2 = " << filename2 << std::endl;
		oss << "file3 = " << filename3 << std::endl;
		oss << "file4 = " << filename4 << std::endl;
		oss << "file5 = " << filename5 << std::endl;
		CSVFile::writeDescriptions(oss.str());

		/* - - - - - - - - - - - - - - - - - - - - - - - */

		std::cerr << "+------------ END --------------+" << std::endl;

		result = 0;
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		result = 1;
	}

	releaseAcquisition();
	rp_Release();
	return result;
}


int test_realTimeAcquisition(const std::vector<std::string> &args) {

	int result;
	
	try {
		std::cerr << " TEST 1 : Real-time acquisition" << std::endl;


		SetBufferSize(ADC_BUFFER_SIZE);
		SetDecimation(8);

		int    nb_loop   = 1000;
		int    frequency = 100e3;
		float amplitude  = 1.0;
		float offset     = 0;
		float phase      = 0;
		
		if (args.size() >= 1) {
			for (auto param : args) {

				// Vérifier si l'argument est "help"
				if (param == "help") {
					std::cerr << "\033[4;0mHelp message\033[0m" << std::endl;
					std::cerr << "Details:" << std::endl;
					std::cerr << "  This application acquires on analog input 1, a signal" << std::endl;
					std::cerr << "  generated on analog output 1, in order to perform" << std::endl;
					std::cerr << "  demodulation and fft calculation." << std::endl;
					std::cerr << "  The time and frequency signals are written to a csv file." << std::endl;
					std::cerr << "Possibilities of utilisation : " << std::endl;
					std::cerr << "  nb_loop=<integer>" << std::endl;
					std::cerr << "      details : number of acquisition tests" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is 1000" << std::endl;
					std::cerr << "  frequency=<integer>; f=<integer>" << std::endl;
					std::cerr << "      details : frequency of generated signal in Hz" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is 100k" << std::endl;
					std::cerr << "  amplitude=<valeur>; a=<valeur>" << std::endl;
					std::cerr << "      details : amplitude of generated signal" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is 1.0" << std::endl;
					std::cerr << "  offset=<valeur>; off=<valeur>" << std::endl;
					std::cerr << "      details : offset of generated signal" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is 0.0" << std::endl;
					std::cerr << "  phase=<value>; ph=<value>" << std::endl;
					std::cerr << "      details : phase of generated signal" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is 0.0" << std::endl;
					std::cerr << "  decimation=<integer>; dec=<integer>" << std::endl;
					std::cerr << "      details : 125MHz quartz frequency decimation factor" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is 8" << std::endl;
					std::cerr << "  buffsize=<integer>; bs=<integer>" << std::endl;
					std::cerr << "      details : buffer size" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << ADC_BUFFER_SIZE << std::endl;
					return 0;
				} else {
					// Parse other arguments
					size_t pos = param.find('=');
					if (pos != std::string::npos) {
						std::string name = param.substr(0, pos);
						std::string value = param.substr(pos + 1);
						
						// Compare the parameter name to retrieve the value
						if (name == "nb_loop" || name == "f2min") {
							nb_loop = convertToInteger(value);
						} else if (name == "frequency" || name == "f") {
							frequency = convertToInteger(value);
						} else if (name == "amplitude" || name == "a") {
							amplitude = std::stod(value);
						} else if (name == "offset" || name == "off") {
							offset = std::stod(value);
						} else if (name == "phase" || name == "ph") {
							phase = std::stod(value);
						} else if (name == "decimation" || name == "dec") {
							SetDecimation(std::stoi(value));
						} else if (name == "buffsize" || name == "bs") {
							SetBufferSize(std::stoull(value));
						} else {
							std::cerr << "Invalid argument format: " << param << std::endl;
							return 1;
						}
					} else {
						std::cerr << "Invalid argument format: " << param << std::endl;
						return 1;
					}
				}
			}
		}
		
		/* Print error, if rp_Init() function failed */
		if (rp_InitReset(true) != RP_OK) {
			throw std::runtime_error("Rp api init failed!");
		}

		std::cerr << "+----------- START -------------+" << std::endl;


		Signal signal("output");
		Signal loop_axis(0, "loop_axis");
		Signal elapsedTimesBuffer(0, "elapsed_time");
		buffers_t *b = rp_createBuffer(2,BUFFER_SIZE,false,false,true);

		/* - - - - - - - - - - - - - - - - - - - - - - - */
		/* Initialisation du signal */
		
		/* Generating frequency */
		rp_GenWaveform(RP_CH_1, RP_WAVEFORM_SINE);

		rp_GenFreq(RP_CH_1, frequency);
		rp_GenAmp(RP_CH_1, amplitude);
		rp_GenOffset(RP_CH_1, offset);
		rp_GenPhase(RP_CH_1, phase);
		rp_GenSetInitGenValue(RP_CH_1, 0.0);

		rp_GenOutEnable(RP_CH_1);
		rp_GenTriggerOnly(RP_CH_1);

		/* - - - - - - - - - - - - - - - - - - - - - - - */
		/* Initialisation de l'acquisition */

		std::cerr << "" << std::endl;

		rp_AcqReset();
		rp_AcqSetDecimationFactor(DECIMATION);
		rp_AcqSetTriggerLevel(RP_T_CH_1, 0.01);
		rp_AcqSetTriggerDelay(ADC_BUFFER_SIZE/2.0);

		int timeDelay = getTimeDelay(DECIMATION);
		uint32_t c;
		int rateCounter = 5; /* smaling rate counter */
		rp_acq_trig_state_t state = RP_TRIG_STATE_TRIGGERED;
		bool fillState = false;


		clock_t cpuclock;
		float elapsedTime;

		for (int loop = 0; loop < nb_loop; loop++) {
			std::cerr << "\rLoop :" << std::setw(17) << loop << "/" << nb_loop  << std::flush;
			/* Echantillonnage du signal */
			fillState = false;

			// get initial time-stamp
			cpuclock = clock();

			rp_AcqStart();
			rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);

			/*length and smaling rate*/
			for (int z = 0; z < rateCounter; z++){
				rp_AcqGetPreTriggerCounter(&c);
				usleep(timeDelay);
				//std::cerr << "Pre counter " << c << std::endl;
			}

			while(1) {
				rp_AcqGetTriggerState(&state);
				if(state == RP_TRIG_STATE_TRIGGERED) {
					break;
				} else {
					usleep(1);
				}
			}
			while(!fillState){
				rp_AcqGetBufferFillState(&fillState);
			}

			rp_AcqStop();

			// get final time-stamp
			cpuclock = clock() - cpuclock;

			uint32_t pos = 0;
			rp_AcqGetWritePointerAtTrig(&pos);

			rp_AcqGetData(pos, b);

			/* Enrgistrement du buffer */
			for (size_t i = 0; i < BUFFER_SIZE; i++) signal[i] = b->ch_f[0][i];
			
			loop_axis.push_back(loop);
			elapsedTime = ((float)cpuclock)/(CLOCKS_PER_SEC / 1000); // calculate the elapsed time in ms
			elapsedTimesBuffer.push_back(elapsedTime);
		}

		/* Releasing resources */
		rp_deleteBuffer(b);
		
		/* - - - - - - - - - - - - - - - - - - - - - - - */

		std::cerr << "Write data to files" << std::endl;

		CSVFile::setTimeToFilepath();
		std::string filename = "elapsedTime.csv";

		std::vector<Signal> outSig = {loop_axis, elapsedTimesBuffer};
		CSVFile outFile(filename);
		outFile.writeSignals(outSig, false); // no time axis
		std::cerr << "File: " << filename  << std::endl;

		/* - - - - - - - - - - - - - - - - - - - - - - - */

		std::cerr << "Write descriptions file" << std::endl;

		std::ostringstream oss;
	
		oss << "[program]" << std::endl;
		oss << "type = test" << std::endl;
		oss << "name = realTimeAcquisition" << std::endl;
		oss << "[parameters]" << std::endl;
		oss << "nb_loop = "				<< nb_loop << std::endl;
		oss << "frequency = "			<< frequency  << std::endl;
		oss << "amplitude = "			<< amplitude << std::endl;
		oss << "offset = "				<< offset << std::endl;
		oss << "phase = "				<< phase << std::endl;
		oss << "smpling_frequency = "	<< SAMPLING_FREQUENCY << std::endl;
		oss << "decimation = "			<< DECIMATION << std::endl;
		oss << "buffsize = "			<< BUFFER_SIZE << std::endl;
		oss << "[files]" << std::endl;
		oss << "file1 = " << filename << std::endl;
		CSVFile::writeDescriptions(oss.str());

		std::cerr << "+------------ END --------------+" << std::endl;

		result = 0;
	} catch (const std::exception &e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		result = 1;
	}
	rp_Release();
	return result;
}

int test_realTimeAcquisition2(const std::vector<std::string> &args) {
	int result;

	try {
		std::cerr << " TEST 2 : Real-time acquisition" << std::endl;
		SetBufferSize(ADC_BUFFER_SIZE);
		SetDecimation(8);

		int   nb_loop      = 1000;
		int   frequency    = 100e3;
		float amplitude    = 1.0;
		float offset       = 0;
		float phase        = 0;
		
		if (args.size() >= 1) {
			for (auto param : args) {

				// Vérifier si l'argument est "help"
				if (param == "help") {
					std::cerr << "\033[4;0mHelp message\033[0m" << std::endl;
					std::cerr << "Details:" << std::endl;
					std::cerr << "  This application acquires on analog input 1, a signal" << std::endl;
					std::cerr << "  generated on analog output 1, in order to perform" << std::endl;
					std::cerr << "  demodulation and fft calculation." << std::endl;
					std::cerr << "  The time and frequency signals are written to a csv file." << std::endl;
					std::cerr << "Possibilities of utilisation : " << std::endl;
					std::cerr << "  nb_loop=<valeur>" << std::endl;
					std::cerr << "      details: number of acquisition tests" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is 1000" << std::endl;
					std::cerr << "  frequency=<valeur>; f=<valeur>" << std::endl;
					std::cerr << "      details: frequency of the generated signal in Hz" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is 100k" << std::endl;
					std::cerr << "  amplitude=<valeur>; a=<valeur>" << std::endl;
					std::cerr << "      details: amplitude of the generated signal" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is 1.0" << std::endl;
					std::cerr << "  offset=<valeur>; off=<valeur>" << std::endl;
					std::cerr << "      details: offset of the generated signal" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is 0.0" << std::endl;
					std::cerr << "  phase=<value>; dec=<value>" << std::endl;
					std::cerr << "      details: phase of the generated signal" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is 0.0" << std::endl;
					std::cerr << "  decimation=<value>; ph=<value>" << std::endl;
					std::cerr << "      details : 125MHz quartz frequency decimation factor" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is 8" << std::endl;
					std::cerr << "  buffsize=<value>; bs=<value>" << std::endl;
					std::cerr << "      details : buffer size" << std::endl;
					std::cerr << "      note: this argument is optional, and if not entered, the default value is " << ADC_BUFFER_SIZE << std::endl;
					return 0;
				} else {
					// Parse other arguments
					size_t pos = param.find('=');
					if (pos != std::string::npos) {
						std::string name = param.substr(0, pos);
						std::string value = param.substr(pos + 1);
						
						// Compare the parameter name to retrieve the value
						if (name == "nb_loop" || name == "f2min") {
							nb_loop = std::stoi(value);
						} else if (name == "frequency" || name == "f") {
							frequency = convertToInteger(value);
						} else if (name == "amplitude" || name == "a") {
							amplitude = std::stod(value);
						} else if (name == "offset" || name == "off") {
							offset = std::stod(value);
						} else if (name == "phase" || name == "ph") {
							phase = std::stod(value);
						} else if (name == "decimation" || name == "dec") {
							SetDecimation(std::stoi(value));
						} else if (name == "buffsize" || name == "bs") {
							SetBufferSize(std::stoull(value));
						} else {
							std::cerr << "Invalid argument format: " << param << std::endl;
							return 1;
						}
					} else {
						std::cerr << "Invalid argument format: " << param << std::endl;
						return 1;
					}
				}
			}
		}

		std::cerr << "+----------- START -------------+" << std::endl;

		Signal signal("output");
		Signal loop_axis(0, "loop_axis");
		Signal elapsedTimesBuffer(0, "elapsed_time");
		
		for (int loop = 0; loop < nb_loop; loop++) {
			if (rp_InitReset(true) != RP_OK) {
				throw std::runtime_error("Rp api init failed!");
			}

			buffers_t *b = rp_createBuffer(2,BUFFER_SIZE,false,false,true);

			/* - - - - - - - - - - - - - - - - - - - - - - - */
			/* Initialisation du signal */
			
			/* Generating frequency */
			rp_GenWaveform(RP_CH_1, RP_WAVEFORM_SINE);

			rp_GenFreq(RP_CH_1, frequency);
			rp_GenAmp(RP_CH_1, amplitude);
			rp_GenOffset(RP_CH_1, offset);
			rp_GenPhase(RP_CH_1, phase);
			rp_GenSetInitGenValue(RP_CH_1, 0.0);

			rp_GenOutEnable(RP_CH_1);
			rp_GenTriggerOnly(RP_CH_1);

			/* - - - - - - - - - - - - - - - - - - - - - - - */
			/* Initialisation de l'acquisition */

			rp_AcqReset();
			rp_AcqSetDecimationFactor(DECIMATION);
			rp_AcqSetTriggerLevel(RP_T_CH_1, 0.01);
			rp_AcqSetTriggerDelay(ADC_BUFFER_SIZE/2.0);

			int timeDelay = getTimeDelay(DECIMATION);
			uint32_t c;
			int rateCounter = 5; /* smaling rate counter */
			rp_acq_trig_state_t state = RP_TRIG_STATE_TRIGGERED;
			bool fillState = false;


			clock_t cpuclock;
			float elapsedTime;
		
			std::cerr << "\rLoop :" << std::setw(17) << loop << "/" << nb_loop  << std::flush;
			/* Echantillonnage du signal */
			fillState = false;

			// get initial time-stamp
			cpuclock = clock();

			rp_AcqStart();
			rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);

			/*length and smaling rate*/
			for (int z = 0; z < rateCounter; z++){
				rp_AcqGetPreTriggerCounter(&c);
				usleep(timeDelay);
				//std::cerr << "Pre counter " << c << std::endl;
			}

			while(1) {
				rp_AcqGetTriggerState(&state);
				if(state == RP_TRIG_STATE_TRIGGERED) {
					break;
				} else {
					usleep(1);
				}
			}
			while(!fillState){
				rp_AcqGetBufferFillState(&fillState);
			}

			rp_AcqStop();

			// get final time-stamp
			cpuclock = clock() - cpuclock;

			uint32_t pos = 0;
			rp_AcqGetWritePointerAtTrig(&pos);

			rp_AcqGetData(pos, b);
			
			/* Enrgistrement du buffer */
			for (size_t i = 0; i < BUFFER_SIZE; i++) signal[i] = b->ch_f[0][i];
			
			loop_axis.push_back(loop);
			elapsedTime = ((float)cpuclock)/(CLOCKS_PER_SEC / 1000); // calculate the elapsed time in ms
			elapsedTimesBuffer.push_back(elapsedTime);

			/* Releasing resources */
			rp_deleteBuffer(b);

			rp_Release();
		}
		
		/* - - - - - - - - - - - - - - - - - - - - - - - */

		std::cerr << "+----- Write data to files ----+" << std::endl;

		CSVFile::setTimeToFilepath();
		std::string filename = "elapsedTime.csv";
		std::cerr << "File: " << filename  << std::endl;
		CSVFile outFile(filename);
		std::vector<Signal> outSig = {loop_axis, elapsedTimesBuffer};
		outFile.writeSignals(outSig, false); // no time axis

		/* - - - - - - - - - - - - - - - - - - - - - - - */

		std::cerr << "+---- Write descriptions file ---+" << std::endl;

		std::ostringstream oss;
	
		oss << "[program]" << std::endl;
		oss << "type = test" << std::endl;
		oss << "name = realTimeAcquisition2" << std::endl;
		oss << "[parameters]" << std::endl;
		oss << "nb_loop = "				<< nb_loop << std::endl;
		oss << "frequency = "			<< frequency  << std::endl;
		oss << "amplitude = "			<< amplitude << std::endl;
		oss << "offset = "				<< offset << std::endl;
		oss << "phase = "				<< phase << std::endl;
		oss << "smpling_frequency = "	<< SAMPLING_FREQUENCY << std::endl;
		oss << "decimation = "			<< DECIMATION << std::endl;
		oss << "buffsize = "			<< BUFFER_SIZE << std::endl;
		oss << "[files]" << std::endl;
		oss << "file1 = " << filename << std::endl;
		CSVFile::writeDescriptions(oss.str());

		std::cerr << "+------------ END --------------+" << std::endl;

		result = 0;
	} catch (const std::exception &e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		result = 1;
	}

	return result;
}

