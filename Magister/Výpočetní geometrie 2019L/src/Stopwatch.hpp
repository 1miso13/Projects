//
//  Stopwatch.hpp
//  Libraries
//
//  Created by Marcel Kiss on 07/02/2019.
//

#ifndef Stopwatch_hpp
#define Stopwatch_hpp

#include <chrono>
#include <string>
#include <map>

using namespace std;
using namespace chrono;

class Stopwatch {
    struct StopwatchData { nanoseconds begin, time; };
    map<string, StopwatchData> _stopwatchs;
public:
    /** Begin stopwatch with name */
    void begin(string name);
    /** Ends stopwatch with name */
    void end(string name);
    /** Returns time elapsed for given stopwatch name */
    double time_in_sec(string name);
    double time_in_ms(string name);
    double time_in_us(string name);
    long long time_in_ns(string name);
};

#endif /* Stopwatch_hpp */
