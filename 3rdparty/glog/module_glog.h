//
// Created by 84539 on 2024/1/15.
//

#ifndef CDRAW_MODULE_GLOG_H
#define CDRAW_MODULE_GLOG_H

#include <glog/logging.h>
#define __MODULE_NAME__ "cdrawLog"
#define LOG_WITH_MODULE(INFO_LEVEL) LOG(INFO_LEVEL) << "[" << __MODULE_NAME__ << "]"

inline int log_init(char *argv, std::string log_dir = "/etc/cdhf/logs/")
{

// Most flags work immediately after updating values.
    FLAGS_alsologtostderr = true; //output to file and stdout
    FLAGS_log_dir = log_dir;
    FLAGS_colorlogtostderr = true;
    FLAGS_max_log_size = 20;  //set .log file size max is 20 (MB)
    FLAGS_stop_logging_if_full_disk = true; //start stop storing log file when disk is full
    google::InstallFailureSignalHandler();
    google::InitGoogleLogging(argv);

    LOG(INFO) << "initialization logger success";

    srand((unsigned)time(NULL));

    return 0;
}

#endif //CDRAW_MODULE_GLOG_H
