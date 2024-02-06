cmake_minimum_required(VERSION 3.16)
project(cdraw)

set(CMAKE_CXX_STANDARD 17)


#set (cxz_json_path		${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cxz_json)
include_directories(
        #${glog_path}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glog
        ${CMAKE_CURRENT_SOURCE_DIR}/cdrawStruct/queue
        ${CMAKE_CURRENT_SOURCE_DIR}/cdrawTools/runTime
        ${CMAKE_CURRENT_SOURCE_DIR}/cdrawTools/net
        ${CMAKE_CURRENT_SOURCE_DIR}/cdrawTools/uart
        ${CMAKE_CURRENT_SOURCE_DIR}/cdrawTools/json
        ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cxz_json/include
)

link_directories(
        #${glog_path}/lib
#        ${cxz_json_path}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cxz_json/lib
)

set(lib
    glog
    cJSON
)

aux_source_directory(. SRC)
aux_source_directory(3rdparty/glog GLOG_SRC)
aux_source_directory(3rdparty/cxz_json/include CXZ_JSON_SRC)
aux_source_directory(cdrawStruct/queue CDRAW_QUEUE_SRC)
aux_source_directory(cdrawTools/runTime CDRAW_TIME_SRC)
aux_source_directory(cdrawTools/net CDRAW_NET_SRC)
aux_source_directory(cdrawTools/uart CDRAW_UART_SRC)
aux_source_directory(cdrawTools/json CDRAW_JSON_SRC)

add_compile_options(-fpermissive)

add_executable(cdraw
                ${SRC} ${GLOG_SRC} ${CDRAW_QUEUE_SRC}
                ${CDRAW_TIME_SRC} ${CDRAW_NET_SRC} ${CDRAW_UART_SRC}
                ${CDRAW_JSON_SRC}

)
target_link_libraries(cdraw ${lib})