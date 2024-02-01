cmake_minimum_required(VERSION 3.16)
project(cdraw)

set(CMAKE_CXX_STANDARD 17)

#set(system_config_path /etc/cdhf/lib-export)
#
#set (glog_path		${system_config_path}/glog)

include_directories(
        #${glog_path}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glog
        ${CMAKE_CURRENT_SOURCE_DIR}/cdrawStruct/queue
        ${CMAKE_CURRENT_SOURCE_DIR}/cdrawTools/runTime
        ${CMAKE_CURRENT_SOURCE_DIR}/cdrawTools/net
)
link_directories(
        #${glog_path}/lib
)

set(lib
    glog

)

add_executable(cdraw main.cc
        cdrawStruct/queue/CdrawQueue.cc
        cdrawStruct/queue/CdrawQueue.h
        3rdparty/glog/module_glog.h
        cdrawTools/runTime/CdrawTime.cc
        cdrawTools/runTime/CdrawTime.h
        cdrawTools/net/CdrawNet.cc
        cdrawTools/net/CdrawNet.h
)
target_link_libraries(cdraw ${lib})