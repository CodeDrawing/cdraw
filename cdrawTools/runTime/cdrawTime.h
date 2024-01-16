//
// Created by 84539 on 2024/1/15.
//

#ifndef CDRAW_CDRAWTIME_H
#define CDRAW_CDRAWTIME_H

#include <iostream>
#include <chrono>
#include "../../3rdparty/glog/moudle_glog.h"
#define __MODULE_NAME__ "cdrawTime"
class cdrawTime {
private:
    std::chrono::time_point<std::chrono::system_clock> currentTime;
    std::chrono::time_point<std::chrono::system_clock> lastTime;
public:
    cdrawTime() {
        currentTime = std::chrono::system_clock::now();
    }
    long long getCurrentTime(const bool print = true, const bool updateNow = true, const bool updateLast = true) {
        if(updateLast){
            lastTime = currentTime;
        }
        if(updateNow){
            currentTime = std::chrono::system_clock::now();
        }
        if(print){
            LOG_WITH_MODULE(INFO) << "currentTime: " << std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count();
        }
        return std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count();
    }
    long long getTimeDiff(const bool print = true, const bool updateNow = true, const bool updateLast = true){
        if(updateNow){
            currentTime = std::chrono::system_clock::now();
        }
        auto tmp_last = lastTime;
        if(updateLast){
            lastTime = currentTime;
        }
        if(print){
            LOG_WITH_MODULE(INFO) << "timeDiff: " << std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch() - tmp_last.time_since_epoch()).count();
        }
        return std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch() - tmp_last.time_since_epoch()).count();
    }
};


#endif //CDRAW_CDRAWTIME_H
