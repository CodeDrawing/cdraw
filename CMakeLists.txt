cmake_minimum_required(VERSION 3.16)
project(cdraw)

set(CMAKE_CXX_STANDARD 17)

set(system_config_path /etc/cdhf/lib-export)

set (glog_path		${systp_path}/glog)
include_directories(
        ${glog_path}/include
)
link_directories(
        ${glog_path}/lib
)
set(glog)
add_executable(cdraw main.cpp
        cdrawStruct/queue/cdrawQueue.cpp
        cdrawStruct/queue/cdrawQueue.h
        3rdparty/glog/moudle_glog.h
        cdrawTools/runTime/cdrawTime.cpp
        cdrawTools/runTime/cdrawTime.h
)
