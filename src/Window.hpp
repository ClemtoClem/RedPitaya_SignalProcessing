#ifndef __WINDOW_HPP
#define __WINDOW_HPP

#include <vector>
#include <cmath>
#include <string>
#include "Signal.hpp"

/**
 * @brief Type of the window
 * @details See https://en.wikipedia.org/wiki/Window_function
 */
enum class WindowType {
	Rectangular,
	Triangular,
	Parzen,
	Welch,
	Sine,
	Hann,
	Hamming,
	Blackman,
	Nuttall,
	BlackmanNuttall,
	BlackmanHarris,
	Tukey,
	PlanckTaper
};

/**
 * @brief Window class for applying a window to a signal
 */
class Window {
public:
	Window();

	/**
	 * @brief Set the window parameters
	 * @param[in] type Type of the window
	 * @param[in] size Size of the window
	 * @param[in] sample_offset Offset of the window in the signal, reduce the size of the window by this value in the beginning and in the end
	 * @param[in] alpha Parameter of the window Tukey and PlanckTaper
	 */
	bool set(WindowType type, size_t size, size_t sample_offset = 0, float alpha = 0.1);

	/**
	 * @brief Set the window type
	 * @param[in] Type of the window
	 */
	void setType(WindowType type);

	/**
	 * @brief Set the window size
	 * @param[in] size Size of the window
	 */
	void setSize(size_t size);

	/**
	 * @brief Set the sample offset
	 * @param[in] sample_offset Offset of the window in the signal, reduce the size of the window by this value in the beginning and in the end
	 * @details The default value is 0
	 */
	void setSampleOffset(size_t sample_offset = 0);

	/**
	 * @brief Set the alpha parameter
	 * @param[in] alpha Parameter of the window Tukey and PlanckTaper
	 * @details The default value is 0.5
	 */
	void setAlpha(float alpha = 0.5);

	/**
	 * @brief Get the window type
	 * @return Window type
	 */
	WindowType getType() const { return _type; }

	/**
	 * @brief Get the window size
	 * @return Window size
	 */
	size_t getSize() const { return _size; }

	/**
	 * @brief Get the sample offset
	 * @return Sample offset
	 */
	size_t getSampleOffset() const { return _sample_offset; }

	/**
	 * @brief Get the alpha parameter
	 * @return Alpha parameter
	 */
	float getAlpha() const { return _alpha; }

	/**
	 * @brief Check if the window is setup
	 * @return True if the window is setup
	 */
	bool isSetup() const { return _isSetup; }

	/**
	 * @brief Calculate the window coefficients
	 */
	void setup();

	/**
	 * @brief Apply the window to the input signal
	 * @param[in] input Input signal
	 */
	virtual Signal apply(const Signal &input);

	/**
	 * @brief Apply the window to the input sample
	 * @param[in] input Input sample
	 * @param[in] index Index of the sample in the signal
	 */
	virtual double apply(double input, size_t index);

private:
	std::vector<double> _window;
	WindowType _type;
	size_t _size;
	size_t _sample_offset; // décalage de la fenêtre
	float _alpha; // pour la fenêtre Tukey et PlanckTaper
	bool _isSetup;
};

#endif // __WINDOW_HPP