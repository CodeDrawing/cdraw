

#ifndef CDRAW_CDRAWTIME_H
#define CDRAW_CDRAWTIME_H

#include <iostream>
#include <chrono>

#include "module_glog.h"


class CdrawTime {
private:
    std::chrono::time_point<std::chrono::system_clock> current_time_;
    std::chrono::time_point<std::chrono::system_clock> last_time_;
public:
    CdrawTime() {
        current_time_ = std::chrono::system_clock::now();
    }
    ~CdrawTime() = default;
    long long getCurrentTime(const bool print = true, const bool updateNow = true, const bool updateLast = true);
    long long getTimeDiff(const bool print = true, const bool updateNow = true, const bool updateLast = true);
};


#endif //CDRAW_CDRAWTIME_H
