#include "Window.hpp"
#include <string>




Window::Window() : _type(WindowType::Rectangular), _size(0), _sample_offset(0), _alpha(0.1), _isSetup(false) {
}

bool Window::set(WindowType type, size_t size, size_t sample_offset, float alpha) {
	if (size < 1) {
		std::cerr << "Window _size must be at least 1" << std::endl;
		return false;
	}
	_size = size;
	_type = type;
	_sample_offset = sample_offset;
	_alpha = alpha;
	_isSetup = false;
	return true;
}

void Window::setType(WindowType type) {
	_type = type;
	_isSetup = false;
}

void Window::setSize(size_t size){
	_size = size;
	_isSetup = false;
}

void Window::setSampleOffset(size_t sample_offset) {
	_sample_offset = sample_offset;
	_isSetup = false;
}

void Window::setAlpha(float alpha) {
	_alpha = alpha;
	_isSetup = false;
}

void Window::setup() {
	if (_isSetup) {
		//std::cerr << "Window already setup" << std::endl;
		return;
	}

	_window.resize(_size);
	if (_sample_offset) {
		for (size_t i = 0; i < _sample_offset; ++i) {
			_window[i] = 0.0;
			_window[_size - 1 - i] = 0.0;
		}
	}
	for (size_t i = _sample_offset; i < _size-_sample_offset; ++i) {
		switch (_type) {
			case WindowType::Rectangular:
				_window[i] = 1.0;
				break;
			case WindowType::Triangular:
				_window[i] = 1.0 - std::abs((i - (_size - 1) / 2.0) / (_size / 2.0));
				break;
			case WindowType::Parzen:
				_window[i] = 1.0 - std::abs((i - (_size - 1) / 2.0) / (_size / 2.0));
				break;
			case WindowType::Welch:
				_window[i] = 1.0 - std::pow((i - (_size - 1) / 2.0) / (_size / 2.0), 2);
				break;
			case WindowType::Sine:
				_window[i] = std::sin(M_PI * i / (_size - 1));
				break;
			case WindowType::Hann:
				_window[i] = 0.5 * (1 - std::cos(2 * M_PI * i / (_size - 1)));
				break;
			case WindowType::Hamming:
				_window[i] = 0.54 - 0.46 * std::cos(2 * M_PI * i / (_size - 1));
				break;
			case WindowType::Blackman:
				_window[i] = 0.42 - 0.5 * std::cos(2 * M_PI * i / (_size - 1)) + 0.08 * std::cos(4 * M_PI * i / (_size - 1));
				break;
			case WindowType::Nuttall:
				_window[i] = 0.355768 - 0.487396 * std::cos(2 * M_PI * i / (_size - 1)) + 0.144232 * std::cos(4 * M_PI * i / (_size - 1)) - 0.012604 * std::cos(6 * M_PI * i / (_size - 1));
				break;
			case WindowType::BlackmanNuttall:
				_window[i] = 0.3635819 - 0.4891775 * std::cos(2 * M_PI * i / (_size - 1)) + 0.1365995 * std::cos(4 * M_PI * i / (_size - 1)) - 0.0106411 * std::cos(6 * M_PI * i / (_size - 1));
				break;
			case WindowType::BlackmanHarris:
				_window[i] = 0.35875 - 0.48829 * std::cos(2 * M_PI * i / (_size - 1)) + 0.14128 * std::cos(4 * M_PI * i / (_size - 1)) - 0.01168 * std::cos(6 * M_PI * i / (_size - 1));
				break;
			case WindowType::Tukey: {
					if (i < _alpha * (_size - 1) / 2) {
						_window[i] = 0.5 * (1 + cos(M_PI * (2.0 * i / (_alpha * (_size - 1)) - 1)));
					} else if (i <= (_size - 1) * (1 - _alpha / 2)) {
						_window[i] = 1.0;
					} else {
						_window[i] = 0.5 * (1 + cos(M_PI * (2.0 * i / (_alpha * (_size - 1)) - 2.0 / _alpha + 1)));
					}
				}
				break;
			case WindowType::PlanckTaper: {
					double t = 2.0 * i / (_size - 1);
					if (t <= _alpha) {
						_window[i] = 1.0 / (exp(_alpha / t - _alpha / (2 - t)) + 1);
					} else if (t >= 2 - _alpha) {
						_window[i] = 1.0 / (exp(_alpha / (2 - t) - _alpha / t) + 1);
					} else {
						_window[i] = 1.0;
					}
				}
				break;
			default:
				std::cerr << "Unknown window type" << std::endl;
				return;
		}
	}
	_isSetup = true;

}

Signal Window::apply(const Signal &input) {
	if (!_isSetup) {
		throw std::runtime_error("Window not setup");
	}

	if (input.size() != _size) {
		std::stringstream ss;
		ss << "Input signal _size does not match window _size. Input _size: " << input.size() << ", window _size: " << _size;
		throw std::runtime_error(ss.str());
	}

	Signal output(_size);
	for (size_t i = 0; i < _size; ++i) {
		output[i] = input[i] * _window[i];
	}

	return output;
}

double Window::apply(double input, size_t index) {
	if (!_isSetup) {
		throw std::runtime_error("Window not setup");
	}

	if (index >= _size) {
		throw std::runtime_error("Index out of bounds");
	}

	return input * _window[index];
};
