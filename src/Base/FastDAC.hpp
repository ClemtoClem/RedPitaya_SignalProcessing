#ifndef FASTDAC_HPP
#define FASTDAC_HPP

#include "Utils.hpp"
#include "../Signal.hpp"

class FastDAC {
public:
    FastDAC();
    ~FastDAC();

    bool Init();
    bool Release();
    bool IsInitialized();

    bool Reset();

    void EnableOutputChannel(RpChannel channel);
    void DisableOutputChannel(RpChannel channel);
    bool IsOutputChannelEnabled(RpChannel channel) const;

    void SetWaveform(RpChannel channel, RpWaveform waveform);
    void SetAmplitude(RpChannel channel, float amplitude);
    void SetFrequency(RpChannel channel, float frequency);
    void SetPhase(RpChannel channel, float phase);
    void SetOffset(RpChannel channel, float offset);
    void SetDutyCycle(RpChannel channel, float duty_cycle);
    void GenerateArbitraryWaveWithMultipleSine(RpChannel channel, std::vector<float> amplitudes, std::vector<float> frequencies);
    void SetArbitraryWaveform(RpChannel channel, std::vector<float> waveform);

    RpWaveform GetWaveform(RpChannel channel) const;
    float GetAmplitude(RpChannel channel) const;
    float GetFrequency(RpChannel channel) const;
    float GetPhase(RpChannel channel) const;
    float GetOffset(RpChannel channel) const;
    float GetDutyCycle(RpChannel channel) const;
    const std::vector<float> &GetArbitraryWaveform(RpChannel channel) const;
};

#endif // FASTDAC_HPP