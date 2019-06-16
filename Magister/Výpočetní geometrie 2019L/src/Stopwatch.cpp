//
//  Stopwatch.cpp
//  Libraries
//
//  Created by Marcel Kiss on 07/02/2019.
//

#include "Stopwatch.hpp"

#define TIME_IN_NANO()  std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch())
#define NANO_TO_SEC(n)  ((n) / 1000000000.0)

inline nanoseconds nano_time_now() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
}

inline double nano_to_sec(long long nano) { return nano / 1000000000.0; }
inline double nano_to_ms(long long nano) { return nano / 1000000.0; }
inline double nano_to_us(long long nano) { return nano / 1000.0; }

void Stopwatch::begin(string name) {
    _stopwatchs[name].begin = nano_time_now();
}

void Stopwatch::end(string name) {
    _stopwatchs[name].time = (nano_time_now() - _stopwatchs[name].begin);
}

double Stopwatch::time_in_sec(string name) {
    return nano_to_sec(time_in_ns(name));
}

double Stopwatch::time_in_ms(string name) {
    return nano_to_ms(time_in_ns(name));
}

double Stopwatch::time_in_us(string name) {
    return nano_to_us(time_in_ns(name));
}

long long Stopwatch::time_in_ns(string name) {
    return _stopwatchs[name].time.count();
}
