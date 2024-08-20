#ifndef __TEST_HPP
#define __TEST_HPP

#include <string>
#include <vector>

/**
 * @brief Test algorithm
 * @param[in] args Arguments
 * @note Write help message if the argument "help" is provided
 */
int test_acquire(const std::vector<std::string> &args);

/**
 * @brief Test the spectrum algorithm
 * @param[in] args Arguments
 * @note Write help message if the argument "help" is provided
 */
int test_spectrum(const std::vector<std::string> &args);

/**
 * @brief Test the demodulation algorithm
 * @param[in] args Arguments
 * @note Write help message if the argument "help" is provided
 */
int test_demodulation(const std::vector<std::string> &args);


/**
 * @brief Test the demodulation algorithm n°2
 * @param[in] args Arguments
 * @note Write help message if the argument "help" is provided
 */
int test_demodulation2(const std::vector<std::string> &args);

/**
 * @brief Test the real time acquisition algorithm
 * @param[in] args Arguments
 */
int test_realTimeAcquisition(const std::vector<std::string> &args);

/**
 * @brief Test the real time acquisition algorithm n°2
 * @param[in] args Arguments
 */
int test_realTimeAcquisition2(const std::vector<std::string> &args);

#endif // __TEST_HPP