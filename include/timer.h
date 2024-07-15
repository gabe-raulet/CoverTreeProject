#ifndef TIMER_H_
#define TIMER_H_

#include <chrono>

using namespace std;

struct LocalTimer
{
    using clock = chrono::high_resolution_clock;
    using duration = chrono::duration<double>;
    using time_point = chrono::time_point<clock, duration>;

    time_point start, end;

    void start_timer() { start = clock::now(); }
    void stop_timer()  { end = clock::now(); }
    double get_elapsed() { return duration(end - start).count(); }
};

#endif
