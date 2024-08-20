#ifndef FASTADC_HPP
#define FASTADC_HPP

#include "Utils.hpp"
#include "../Signal.hpp"

class FastADC {
public:
    FastADC();
    ~FastADC();

    bool Init();
    bool Release();
    bool IsInitialized();

    bool Reset();

    void StartAcquisition();
    void StopAcquisition();

    void SetTriggerLevel(float level);
    void SetTriggerDelay(float delay);

    void GetBufferChannel(RpChannel channel, Signal &signal);

private:
    bool _initialized;
};

#endif // FASTADC_HPP