#ifndef CSVFILE_HPP
#define CSVFILE_HPP

#include <fstream>
#include <vector>
#include <complex>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include "Signal.hpp"
#include "Spectrum.hpp"



// Method to generate a frequency axis
std::vector<double> frequency_axis(size_t n, double d = 1.0);

class CSVFile {
public:
	static std::string DEFAULT_FILEPATH;
	static std::string FILEPATH;

	static void setFilePath(const std::string &path) {
		FILEPATH = path;
		std::filesystem::create_directories(FILEPATH);
	}

	static void setTimeToFilepath(bool withDate = true, bool withTime = true);

	/**
	 * @brief Write a descriptions on the arguments used and files created
	 * @param[in] descriptions Descriptions of the arguments used and files created
	 */
	static void writeDescriptions(const std::string &descriptions) {
		std::ofstream file(FILEPATH + "descriptions.cfg", std::ios::out | std::ios::trunc);
		file << descriptions;
		file.close();
	}

	CSVFile(const std::string &filename);

	// Method to read signals from a CSV file
	std::vector<Signal> readSignals();

	/**
	 * @brief Method to write signals to a CSV file
	 * @param[in] signals Signals to write
	 * @param[in] axis If true, the axis will be written
	*/
	void writeSignals(const std::vector<Signal> &signals, bool axis = true);

	/**
	 * @brief Method to write signal to a CSV file
	 * @param[in] signal Signal to write
	 * @param[in] axis If true, the axis will be written
	 */
	void writeSignal(const Signal &signal, bool axis = true);

	/**
	 * @brief Method to append signals to a CSV file
	 * @param[in] signals Signals to write
	 */
	void writeSignalsToEnd(const std::vector<Signal> &signals);
	
	/**
	 * @brief Method to add a signal to a CSV file
	 * @param[in] signal Signal to write
	 */
	void writeSignalToEnd(const Signal &signal);

	// Method to read spectrums from a CSV file
	std::vector<Spectrum> readSpectrums();

	/**
	 * @brief Method to write spectrums to a CSV file
	 * @param[in] spectrums Spectrums to write
	 * @param[in] axis If true, the axis will be written
	 * @param[in] withNegativeFrequencies If true, the negative frequencies will be written
	 */
	void writeSpectrums(const std::vector<Spectrum> &spectrums, bool axis = true, bool withNegativeFrequencies = false);

	/**
	 * @biref Method to append spectrums to a CSV file
	 * @param[in] spectrums Spectrums to write
	 * @param[in] withNegativeFrequencies If true, the negative frequencies will be written
	 */
	void writeSpectrumsToEnds(const std::vector<Spectrum> &spectrums, bool withNegativeFrequencies = false);

	/**
	 * @brief Method to add a spectrum to a CSV file
	 * @param[in] spectrum Spectrum to write
	 * @param[in] withNegativeFrequencies If true, the negative frequencies will be written
	 */
	void writeSpectrumToEnd(const Spectrum &spectrum, bool withNegativeFrequencies = false);

private:
	// Utility function to parse a complex number from a string
	complexd parseComplex(const std::string &str);

	void ensureSameSize(const std::vector<Signal>& signals);
    void ensureSameSize(const std::vector<Spectrum>& spectrums);

	// Overload for writing complex numbers to a stream
	friend std::ostream &operator<<(std::ostream &os, const complexd &c) {
		os << c.real() << (c.imag() >= 0 ? "+" : "") << c.imag() << "j";
		return os;
	}

	std::string _filename;
    std::fstream _fileStream;
};

#endif // CSVFILE_HPP