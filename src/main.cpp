/* Red Pitaya Cpp API */

#include <string>
#include <algorithm>
#include <iostream>
#include <iomanip>

#include "tests.hpp"
#include "modules.hpp"

int main(int argc, char *argv[]){
	int res = 0;

	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <name/module_name> [optional arguments]" << std::endl;
		std::cerr << "Write 'help' to see available tests or modules." << std::endl;
		return 1;
	}

	// Parser les arguments
	std::vector<std::string> args(argv + 1, argv + argc);
	std::string name = args[0];

	// Supprimer le premier argument (name)
	args.erase(args.begin());

	if (name == "acquire") {
		res |= test_acquire(args);
	} else if (name == "spectrum") {
		res |= test_spectrum(args);
	} else if (name == "demodulation") {
		res |= test_demodulation(args);
	} else if (name == "demodulation2") {
		res |= test_demodulation2(args);
	} else if (name == "realTimeAcquisition") {
		res |= test_realTimeAcquisition(args);
	} else if (name == "realTimeAcquisition2") {
		res |= test_realTimeAcquisition2(args);
	} else if (name == "frequencyScanning") {
		res |= module_frequencyScanning(args);
	} else if (name == "help") {
		std::cout << "Available tests:" << std::endl;
		std::cout << "\tacquire <optional arguments>" << std::endl;
		std::cout << "\tspectrum <optional arguments>" << std::endl;
		std::cout << "\tdemodulation <optional arguments>" << std::endl;
		std::cout << "\tdemodulation2 <optional arguments>" << std::endl;
		std::cout << "\trealTimeAcquisition <optional arguments>" << std::endl;
		std::cout << "\trealTimeAcquisition2 <optional arguments>" << std::endl;
		std::cout << "Available modules:" << std::endl;
		std::cout << "\tfrequencyScanning <optional arguments>" << std::endl;
	} else {
		std::cerr << "Unknown test or module name: " << name << std::endl;
		std::cerr << "Write 'help' to see available tests or modules." << std::endl;
		return 1;
	}

	return res;
}

