#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

class Timer {
public:
	Timer() : _start(std::chrono::high_resolution_clock::now()), _end(std::chrono::high_resolution_clock::now()), _duration(0), _min_duration(0), _max_duration(0) {}

    void start() {
        _start = std::chrono::high_resolution_clock::now();
    }

    void stop() {
        _end = std::chrono::high_resolution_clock::now();
        _duration = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(_end - _start).count()) / 1000;
        if (_duration < _min_duration || _min_duration == 0) {
            _min_duration = _duration;
        }
        if (_duration > _max_duration) {
            _max_duration = _duration;
        }
    }

    double getDuration() const {
        return _duration;
    }

    double getMinDuration() const {
        return _min_duration;
    }

    double getMaxDuration() const {
        return _max_duration;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _start;
    std::chrono::time_point<std::chrono::high_resolution_clock> _end;
    double _duration;
    double _min_duration;
    double _max_duration;
};

#endif // TIMER_HPP