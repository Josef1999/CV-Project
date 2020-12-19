#pragma once
#include "timer.h"

Timer::Timer()
{
    _start_count = _stop_count = 0;
    _timer_state = false;
    unsigned long long frequency;
    QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
    _millisec_percount = (double)1.0 / ((double)frequency / 1000.0);
}
void Timer::StartTimer()
{
    _timer_state = true;
    QueryPerformanceCounter((LARGE_INTEGER*)&_start_count);
}
void Timer::StopTimer()
{
    QueryPerformanceCounter((LARGE_INTEGER*)&_stop_count);
    _timer_state = false;
}
double Timer::GetTimerSec() const
{
    return GetTimerMilliSec() / (double)1000.0;
}
double Timer::GetTimerMilliSec() const
{
    return (float)(_stop_count - _start_count) * _millisec_percount;
}