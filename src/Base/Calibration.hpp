#ifndef CALIBRATION_HPP
#define CALIBRATION_HPP

#include "Common.hpp"
#include "rp.h"

class Calibration : public Common {
public:
    Calibration();
    ~Calibration();

    int Init();
    int Release();

    rp_calib_params_t GetParams() const;
    int WriteParams(const rp_calib_params_t& calib_params);
    void SetToZero();
    uint32_t GetFrontEndScale(rp_channel_t channel, rp_pinState_t gain) const;
    int SetFrontEndOffset(rp_channel_t channel, rp_pinState_t gain, rp_calib_params_t* out_params);
    int SetFrontEndScaleLV(rp_channel_t channel, float referentialVoltage, rp_calib_params_t* out_params);
    int SetFrontEndScaleHV(rp_channel_t channel, float referentialVoltage, rp_calib_params_t* out_params);
    int SetBackEndOffset(rp_channel_t channel);
    int SetBackEndScale(rp_channel_t channel);
    int CalibrateBackEnd(rp_channel_t channel, rp_calib_params_t* out_params);
    int Reset();
    int32_t GetDataMedian(rp_channel_t channel, rp_pinState_t gain) const;
    float GetDataMedianFloat(rp_channel_t channel, rp_pinState_t gain) const;
    int GetDataMinMaxFloat(rp_channel_t channel, rp_pinState_t gain, float* min, float* max) const;
    int SetCachedParams();

private:
    int ReadParams(rp_calib_params_t* calib_params);
    
    rp_calib_params_t calib;
    rp_calib_params_t failsafe_params;
};

#endif /* CALIBRATION_HPP */
