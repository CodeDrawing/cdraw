
#include "CdrawTime.h"
#define __MODULE_NAME__ "cdrawTime"

long long
CdrawTime::getCurrentTime(const bool print, const bool updateNow, const bool updateLast) {
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

long long
CdrawTime::getTimeDiff(const bool print, const bool updateNow, const bool updateLast){
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