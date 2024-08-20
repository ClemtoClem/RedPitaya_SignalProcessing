#ifndef __ACQUISITION_HPP
#define __ACQUISITION_HPP

#include "rp.h"
#include "rp_hw-calib.h"
#include "rp_hw-profiles.h"

#include "Signal.hpp"
#include "Window.hpp"

uint32_t getTimeDelay(int decimation);

/// @brief Initialization of the acquisition driver
/// @param trigger_level Trigger level
/// @param trigger_delay Trigger delay
void initAcquisition(float trigger_level = 0.001f, int32_t trigger_delay = 0);

/// @brief Release of the acquisition driver
void releaseAcquisition();

/// @brief Acquisition of a single channel
/// @param signal Signal to fill
void acquisitionChannel1(Signal &signal);

/// @brief Acquisition of a single channel
/// @param signal Signal to fill
void acquisitionChannel2(Signal &signal);

/// @brief Acquisition of two channels
/// @param signal1 Signal to fill
/// @param signal2 Signal to fill
/// @param trigger Trigger channel
void acquisitionChannels1_2(Signal &signal1, Signal &signal2, rp_channel_trigger_t trigger = RP_T_CH_1);

#endif // __ACQUISITION_HPP