#pragma once
#include <windows.h>
#include <immintrin.h>
#include <ctime>

class Timer
{
private:
    unsigned long long _start_count;
    unsigned long long _stop_count;
    double _millisec_percount;
    bool _timer_state;
public:
    Timer();
    void StartTimer();
    void StopTimer();
    double GetTimerMilliSec() const;
    double GetTimerSec() const;
};
