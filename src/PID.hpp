#ifndef __PID_HPP
#define __PID_HPP

#include <iostream>

/**
 * @brief Implement a PID controller
 */
class PID {
private:
	double kp;
	double ki;
	double kd;
	double outMin;
	double outMax;
	double integral;
	double last_error;
	double deltaTime;

	// Additional parameters for self-tuning
	double alpha; // Adaptation rate for kp
	double beta;  // Adaptation rate for ki
	double gamma; // Adaptation rate for kd

	bool is_set;

public:
	
	PID();

	~PID();

	/**
	 * Set the PID controller parameters
	 * @param[in] kp Proportional gain
	 * @param[in] ki Integral gain
	 * @param[in] kd Derivative gain
	 * @param[in] outMin Minimum value of the manipulated variable
	 * @param[in] outMax Maximum value of the manipulated variable
	 * @param[in] alpha Adaptation rate for kp
	 * @param[in] beta Adaptation rate for ki
	 * @param[in] gamma Adaptation rate for kd
	 */
	bool set(double kp, double ki, double kd, double outMin = -1, double outMax = 1, double alpha = 0.0, double beta = 0.0, double gamma = 0.0);

	/**
	 * Set the PID controller parameters
	 * @param[in] kp Proportional gain
	 * @param[in] ki Integral gain
	 * @param[in] kd Derivative gain
	 */
	bool setGains(double kp, double ki, double kd);

	/**
	 * Set the PID controller output limits
	 * @param[in] outMin Minimum value of the manipulated variable
	 * @param[in] outMax Maximum value of the manipulated variable
	 */
	bool setOutputLimits(double outMin, double outMax);

	/**
	 * @brief Returns et the proportional gain
	 * @return Proportional gain
	 */
	double getKp() { return kp; }

	/**
	 * @brief Returns et the integral gain
	 * @return Integral gain
	 */
	double getKi() { return ki; }

	/**
	 * @brief Returns et the derivative gain
	 * @return Derivative gain
	 */
	double getKd() { return kd; }

	/**
	 * @brief Returns et the maximum value of the manipulated variable
	 * @return Maximum value of the manipulated variable
	 */
	double getOutMax() { return outMax; }

	/**
	 * @brief Returns et the minimum value of the manipulated variable
	 * @return Minimum value of the manipulated variable
	 */
	double getOutMin() { return outMin; }

	/**
	 * @brief Apply the PID controller on the input value and the desired value
	 * @param[in] desired_value Desired value
	 * @param[in] input Input value
	 * @return Output value
	 */
	double apply(double desired_value, double input);

	/**
	 * @brief Reset the PID controller
	 */
	void reset();
};


#endif // __PID_HPP