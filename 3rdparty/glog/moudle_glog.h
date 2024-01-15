//
// Created by 84539 on 2024/1/15.
//

#ifndef CDRAW_MOUDLE_GLOG_H
#define CDRAW_MOUDLE_GLOG_H

#include <glog/logging.h>
#define LOG_WITH_MODULE(INFO_LEVEL) LOG(INFO_LEVEL) << "[" << __MODULE_NAME__ << "]"
#endif //CDRAW_MOUDLE_GLOG_H
