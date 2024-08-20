#include "CSVFile.hpp"
#include "globals.hpp"
#include "utils.hpp"

std::string CSVFile::DEFAULT_FILEPATH = "./data/";
std::string CSVFile::FILEPATH = CSVFile::DEFAULT_FILEPATH;

std::vector<double> calculate_frequency_axis(size_t n, double d) {
    if (n == 0) {
        throw std::invalid_argument("The number of points must be greater than 0");
    }
    if (d <= 0.0) {
        throw std::invalid_argument("The frequency step must be greater than 0");
    }
    std::vector<double> freqs(n);
    double val = 1.0 / (n * d); // Frequency step
    size_t N = (n - 1) / 2 + 1;

    for (size_t i = 0; i < N; ++i) {
        freqs[i] = i * val;
    }

    for (size_t i = N; i < n; ++i) {
        freqs[i] = (static_cast<double>(i) - static_cast<double>(n)) * val;
    }

    return freqs;
}

void CSVFile::setTimeToFilepath(bool withDate, bool withTime) {
    // Get the current date and time
    time_t now = time(0);
    tm *ltm = localtime(&now);
    std::stringstream ss;
    ss << DEFAULT_FILEPATH;
    if (withDate) {
        ss << (1900 + ltm->tm_year) << "-";
        ss << format(2, '0', (1 + ltm->tm_mon)) << "-" << format(2, '0', ltm->tm_mday);
    }
    if (withTime) {
        ss << "_" << format(2, '0', ltm->tm_hour) << ":"<< format(2, '0', ltm->tm_min) << ":" << format(2, '0', ltm->tm_sec) << "/";
    } else if (withDate) {
        ss << "/";
    }
    FILEPATH = ss.str();

    // créer le répertoire s'il n'existe pas
    std::filesystem::create_directories(FILEPATH);
}

CSVFile::CSVFile(const std::string &filename) : _filename(filename) {
    if (filename.empty()) {
        throw std::invalid_argument("Filename cannot be empty");
    }
}


std::vector<Signal> CSVFile::readSignals() {
    _fileStream.open(FILEPATH + _filename, std::ios::in);
    if (!_fileStream.is_open()) {
        throw std::ios_base::failure("Failed to open file");
    }

    std::vector<Signal> signals;
    std::string line;

    while (std::getline(_fileStream, line)) {
        std::stringstream lineStream(line);
        std::string name;
        std::getline(lineStream, name, ','); // Read the signal name
        std::string token;
        std::vector<double> values;
        while (std::getline(lineStream, token, ',')) {
            values.push_back(std::stod(token));
        }
        signals.emplace_back(values, name);
    }
    _fileStream.close();

    return signals;
}

void CSVFile::writeSignals(const std::vector<Signal>& signals, bool axis) {
    if (signals.empty()) {
        throw std::invalid_argument("No signals provided");
    }

    ensureSameSize(signals);

    _fileStream.open(FILEPATH + _filename, std::ios::out | std::ios::trunc);
    if (!_fileStream.is_open()) {
        throw std::ios_base::failure("Failed to open file");
    }

	size_t N = signals[0].size();

	if (axis) {
		_fileStream << "time";
		for (size_t i = 0; i < N; i++) {
			_fileStream << "," << static_cast<double>(i) / SAMPLING_FREQUENCY;
		}
        _fileStream << "\n";
	}

    for (const auto& signal : signals) {
        _fileStream << signal.getName();
        for (const auto& value : signal) {
            _fileStream << "," << value;
        }
        _fileStream << "\n";
    }
    _fileStream.close();
}

void CSVFile::writeSignal(const Signal &signal, bool axis)
{
    _fileStream.open(FILEPATH + _filename, std::ios::out | std::ios::trunc);
    if (!_fileStream.is_open()) {
        throw std::ios_base::failure("Failed to open file");
    }
	size_t N = signal.size();

	if (axis) {
		_fileStream << "time";
		for (size_t i = 0; i < N; i++) {
			_fileStream << "," << static_cast<double>(i) / SAMPLING_FREQUENCY;
		}
        _fileStream << "\n";
	}

	_fileStream << signal.getName();
	for (const auto& value : signal) {
		_fileStream << "," << value;
	}
	_fileStream << "\n";

    _fileStream.close();
}

void CSVFile::writeSignalsToEnd(const std::vector<Signal>& signals) {
    if (signals.empty()) {
        throw std::invalid_argument("No signals provided");
    }

    ensureSameSize(signals);

    _fileStream.open(FILEPATH + _filename, std::ios::out | std::ios::app);
    if (!_fileStream.is_open()) {
        throw std::ios_base::failure("Failed to open file");
    }
	_fileStream.seekg (0, std::ios::end);

    for (const auto& signal : signals) {
        _fileStream << signal.getName();
        for (const auto& value : signal) {
            _fileStream << "," << value;
        }
        _fileStream << "\n";
    }
    _fileStream.close();
}

void CSVFile::writeSignalToEnd(const Signal &signal)
{
    _fileStream.open(FILEPATH + _filename, std::ios::out | std::ios::app);
    if (!_fileStream.is_open()) {
        throw std::ios_base::failure("Failed to open file");
    }
	_fileStream.seekg (0, std::ios::end);

    // Add the new signal name to the header
    _fileStream << signal.getName();

    // Add the new signal values to each line
    for (const auto& value : signal) {
        _fileStream << "," << value;
    }

	_fileStream << "\n";
    _fileStream.close();
}

std::vector<Spectrum> CSVFile::readSpectrums() {
    _fileStream.open(FILEPATH + _filename, std::ios::in);
    if (!_fileStream.is_open()) {
        throw std::ios_base::failure("Failed to open file");
    }

    std::vector<Spectrum> spectrums;
    std::string line;

    while (std::getline(_fileStream, line)) {
        std::stringstream lineStream(line);
        std::string name;
        std::getline(lineStream, name, ','); // Read the spectrum name
        std::string token;
        std::vector<complexd> values;
        while (std::getline(lineStream, token, ',')) {
            values.push_back(parseComplex(token));
        }
        spectrums.emplace_back(values, name);
    }
    _fileStream.close();

    return spectrums;
}

void CSVFile::writeSpectrums(const std::vector<Spectrum> &spectrums, bool axis, bool withNegativeFrequencies) {
    if (spectrums.empty()) {
        throw std::invalid_argument("No spectrum provided");
    }

    ensureSameSize(spectrums);

    _fileStream.open(FILEPATH + _filename, std::ios::out | std::ios::trunc);
    if (!_fileStream.is_open()) {
        throw std::ios_base::failure("Failed to open file");
    }

    // Calculate frequency axis
	size_t N = spectrums[0].size();
    std::vector<double> frequencies = calculate_frequency_axis(N, 1 / SAMPLING_FREQUENCY);
    size_t n = (withNegativeFrequencies) ? N : (N - 1) / 2 + 1;

    // Write frequecy axis
    if (axis) {
        _fileStream << "frequency";
		for (size_t i = 0; i < n; i++) {
			_fileStream << "," << frequencies[i];
		}
    	_fileStream << "\n";
    }

    // Write spectrums
	for (auto &spectrum : spectrums) {
        _fileStream << spectrum.getName();
		for (size_t k = 0; k < n; k++) {
			const complexd &c = spectrum[k];
			_fileStream << "," << c.real();
			if (c.imag() != 0) {
				_fileStream << (c.imag() >= 0 ? "+" : "") << c.imag() << "j";
			}
        }
		_fileStream << "\n";
	}
    _fileStream.close();
}

void CSVFile::writeSpectrumsToEnds(const std::vector<Spectrum>& spectrums, bool withNegativeFrequencies) {
    if (spectrums.empty()) {
        throw std::invalid_argument("No spectrums provided");
    }

    ensureSameSize(spectrums);

    _fileStream.open(FILEPATH + _filename, std::ios::out | std::ios::app);
    if (!_fileStream.is_open()) {
        throw std::ios_base::failure("Failed to open file");
    }
	_fileStream.seekg (0, std::ios::end);

	size_t N = spectrums[0].size();
    size_t n = withNegativeFrequencies ? N : (N - 1) / 2 + 1;
    for (const auto& spectrum : spectrums) {
        _fileStream << spectrum.getName();
		for (size_t k = 0; k < n; k++) {
            _fileStream << "," << spectrum[k].real();
            if (spectrum[k].imag() != 0) {
                _fileStream << (spectrum[k].imag() >= 0 ? "+" : "") << spectrum[k].imag() << "j";
            }
        }
        _fileStream << "\n";
    }
    _fileStream.close();
}

void CSVFile::writeSpectrumToEnd(const Spectrum &spectrum, bool withNegativeFrequencies) {
    size_t N = spectrum.size();

   _fileStream.open(FILEPATH + _filename, std::ios::out | std::ios::app);
    if (!_fileStream.is_open()) {
        throw std::ios_base::failure("Failed to open file");
    }
	_fileStream.seekg (0, std::ios::end);

    // Add the new spectrum name to the header
    _fileStream << spectrum.getName();

    // Add the new spectrum values to each line
    size_t n = withNegativeFrequencies ? N : (N - 1) / 2 + 1;
    for (size_t k = 0; k < n; k++) {
        _fileStream << "," << spectrum[k].real();
        if (spectrum[k].imag() != 0) {
            _fileStream << (spectrum[k].imag() >= 0 ? "+" : "") << spectrum[k].imag() << "j";
        }
    }
	_fileStream << "\n";
    _fileStream.close();
}

// Vérifie que tous les signaux ont la même taille
void CSVFile::ensureSameSize(const std::vector<Signal>& signals) {
    if (signals.empty()) return;

    size_t size = signals[0].size();
    for (const auto& signal : signals) {
        if (signal.size() != size) {
            throw std::invalid_argument("All signals must have the same size");
        }
    }
}

// Vérifie que tous les spectrums ont la même taille
void CSVFile::ensureSameSize(const std::vector<Spectrum>& spectrums) {
    if (spectrums.empty()) return;

    size_t size = spectrums[0].size();
    for (const auto& spectrum : spectrums) {
        if (spectrum.size() != size) {
            throw std::invalid_argument("All spectrums must have the same size");
        }
    }
}

complexd CSVFile::parseComplex(const std::string &str) {
    double real = 0.0, imag = 0.0;
    size_t pos = str.find_first_of("+-", 1);
    if (pos != std::string::npos) {
        real = std::stod(str.substr(0, pos));
        imag = std::stod(str.substr(pos));
    } else {
        real = std::stod(str);
    }
    return complexd(real, imag);
}
