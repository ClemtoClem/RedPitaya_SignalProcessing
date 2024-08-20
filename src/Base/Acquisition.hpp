#ifndef ACQUISITION_HPP
#define ACQUISITION_HPP

#include "Utils.hpp"

class Acquisition {
public:

    Acquisition();
    ~Acquisition();

    int Init();
    int Release();

    void SetArmSetup(bool enable);
    
    void SetGain(RPChannel channel, RpChannelGain gain)
    RpChannelGain GetGain(RPChannel channel) const;
    float GetGainVoltage(RPChannel channel) const;

    void SetDecimation(RPDecimation decimation);
    RPDecimation GetDecimation() const;
    void SetDecimationFactor(uint32_t decimFactor);
    uint32_t GetDecimationFactor() const;

    void AxiSetDecimation(RPDecimation decimation);
    void AxiSetDecimationFactor(uint32_t decimFactor);

    
};

#endif /* ACQUISITION_HPP */