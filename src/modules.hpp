#ifndef __MODULES_HPP
#define __MODULES_HPP

#include <string>
#include <vector>

/**
 * @biref Module to perform a frequency scanning
 * @param[in] args Arguments
 * @note Write help message if the argument "help" is provided
 */
int module_frequencyScanning(const std::vector<std::string> &args);

/**
 * @brief Module to perform a real-time acquisition
 * @param[in] args Arguments
 * @note Write help message if the argument "help" is provided
 */
int module_lockIn(const std::vector<std::string> &args);

#endif // __MODULES_HPP