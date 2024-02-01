

#ifndef CDRAW_CDRAWTIME_H
#define CDRAW_CDRAWTIME_H

#include <iostream>
#include <chrono>

#include "module_glog.h"

#define __MODULE_NAME__ "cdrawTime"
class CdrawTime {
private:
    std::chrono::time_point<std::chrono::system_clock> current_time_;
    std::chrono::time_point<std::chrono::system_clock> last_time_;
public:
    CdrawTime() {
        current_time_ = std::chrono::system_clock::now();
    }
    ~CdrawTime() = default;
    long long getCurrentTime(const bool print = true, const bool updateNow = true, const bool updateLast = true) {
        if(updateLast){
            last_time_ = current_time_;
        }
        if(updateNow){
            current_time_ = std::chrono::system_clock::now();
        }
        if(print){
            LOG_WITH_MODULE(INFO) << "currentTime: " << std::chrono::duration_cast<std::chrono::milliseconds>(current_time_.time_since_epoch()).count();
        }
        return std::chrono::duration_cast<std::chrono::milliseconds>(current_time_.time_since_epoch()).count();
    }
    long long getTimeDiff(const bool print = true, const bool updateNow = true, const bool updateLast = true){
        if(updateNow){
            current_time_ = std::chrono::system_clock::now();
        }
        auto tmp_last = last_time_;
        if(updateLast){
            last_time_ = current_time_;
        }
        if(print){
            LOG_WITH_MODULE(INFO) << "timeDiff: " << std::chrono::duration_cast<std::chrono::milliseconds>(current_time_.time_since_epoch() - tmp_last.time_since_epoch()).count();
        }
        return std::chrono::duration_cast<std::chrono::milliseconds>(current_time_.time_since_epoch() - tmp_last.time_since_epoch()).count();
    }
};


#endif //CDRAW_CDRAWTIME_H
