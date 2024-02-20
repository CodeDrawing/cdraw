cmake_minimum_required(VERSION 3.16)
project(cdraw)

set(CMAKE_CXX_STANDARD 17)
add_definitions("-g")

#set (cxz_json_path		${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cxz_json)
include_directories(
        #${glog_path}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glog
        ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cxz_json/include
        ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/sfud/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/cdrawStruct/queue
        ${CMAKE_CURRENT_SOURCE_DIR}/cdrawTools/runTime
        ${CMAKE_CURRENT_SOURCE_DIR}/cdrawTools/net
        ${CMAKE_CURRENT_SOURCE_DIR}/cdrawTools/uart
        ${CMAKE_CURRENT_SOURCE_DIR}/cdrawTools/json
        ${CMAKE_CURRENT_SOURCE_DIR}/cdrawTools/spi
        ${CMAKE_CURRENT_SOURCE_DIR}/cdrawTools/common
)

link_directories(
        #${glog_path}/lib
        #${cxz_json_path}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cxz_json/lib
)

set(lib
    glog
    cJSON
)

aux_source_directory(. SRC)
aux_source_directory(3rdparty/glog GLOG_SRC)
aux_source_directory(3rdparty/cxz_json/include CXZ_JSON_SRC)
aux_source_directory(3rdparty/sfud/inc  SFUD_INC)
aux_source_directory(3rdparty/sfud/src  SFUD_SRC)
aux_source_directory(3rdparty/sfud/port SFUD_PORT)
aux_source_directory(cdrawStruct/queue  CDRAW_QUEUE_SRC)
aux_source_directory(cdrawTools/runTime CDRAW_TIME_SRC)
aux_source_directory(cdrawTools/net     CDRAW_NET_SRC)
aux_source_directory(cdrawTools/uart    CDRAW_UART_SRC)
aux_source_directory(cdrawTools/json    CDRAW_JSON_SRC)
aux_source_directory(cdrawTools/spi     CDRAW_SPI_SRC)
aux_source_directory(cdrawTools/common     CDRAW_CMT_SRC)

add_compile_options(-fpermissive)

add_executable(cdraw
                ${SRC} ${GLOG_SRC} ${CDRAW_QUEUE_SRC}
                ${CDRAW_TIME_SRC} ${CDRAW_NET_SRC} ${CDRAW_UART_SRC}
                ${CDRAW_JSON_SRC} ${SFUD_INC} ${SFUD_SRC} ${SFUD_PORT}
                ${CDRAW_SPI_SRC} ${CDRAW_CMT_SRC}
)
target_link_libraries(cdraw ${lib})