#include "PID.hpp"
#include "globals.hpp"

PID::PID() : kp(0.0), ki(0.0), kd(0.0), outMin(-1.0), outMax(1.0), alpha(0.0), beta(0.0), gamma(0.0), is_set(false) {
	deltaTime = 1/SAMPLING_FREQUENCY;
}

PID::~PID()
{
}

bool PID::set(double kp, double ki, double kd, double outMin, double outMax, double alpha, double beta, double gamma) {
	if (kp<0 || ki<0|| kd<0) {
		std::cerr << "kp, ki and kd must be greater than zero" << std::endl;
		return false;
	}

	// Set values
	this->kp = kp;
	this->ki = ki;
	this->kd = kd;

	this->outMax = outMax;
	this->outMin = outMin;

	this->alpha = alpha;
	this->beta = beta;
	this->gamma = gamma;

	// Reset the PID
	reset();

	// Set the PID as set
	is_set = true;
	return true;
}

bool PID::setGains(double kp, double ki, double kd) {
	if (kp<0 || ki<0|| kd<0) {
		std::cerr << "kp, ki and kd must be greater than zero" << std::endl;
		return false;
	}
	this->kp = kp;
	this->ki = ki;
	this->kd = kd;

	// Set the PID as set
	is_set = true;
	return true;
}

bool PID::setOutputLimits(double outMin, double outMax) {
	if (outMin >= outMax) {
		std::cerr << "outMin must be less than outMax" << std::endl;
		return false;
	}
	this->outMin = outMin;
	this->outMax = outMax;
	return true;
}

double PID::apply(double desired_value, double input) {
	if (!is_set) {
		std::cerr << "PID regulator is not set" << std::endl;
		return 0.0;
	}

	// The error is the difference between the desired value and the input value
	double error = desired_value - input;
	// The integral is the sum of the error over time
	integral += error * deltaTime;
	// The derivative is the rate of change of the error
	double derivative = (error - last_error) / deltaTime;
	// The output is the sum of the proportional, integral, and derivative
	double output = kp * error + ki * integral + kd * derivative;

	// If the output is less than the minimum output, we need to increase it to
	// be within the limits and prevent the system from overshooting the desired value
	if (output < outMin) {
		output = outMin;
		if (error > 0) {
			integral -= error * deltaTime;
		}
	} else if (output > outMax) {
		output = outMax;
		if (error < 0) {
			integral -= error * deltaTime;
		}
	}

	return output;
}

void PID::reset() {
	integral = 0.0;
	last_error = 0.0;
}