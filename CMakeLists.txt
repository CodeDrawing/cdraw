cmake_minimum_required(VERSION 3.16)
project(cdraw)

set(CMAKE_CXX_STANDARD 17)
add_definitions("-g")

# 设置sysroot路径
set(CMAKE_SYSROOT /home/codedrawing/work-dir/Tools/buildroot-2023.11/output/host/aarch64-buildroot-linux-gnu/sysroot)

#设置编译器和连接器
set(CMAKE_C_COMPILER /home/codedrawing/work-dir/Tools/buildroot-2023.11/output/host/bin/aarch64-linux-gcc)
set(CMAKE_CXX_COMPILER /home/codedrawing/work-dir/Tools/buildroot-2023.11/output/host/bin/aarch64-linux-g++)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_FIND_ROOT_PATH /home/codedrawing/work-dir/Tools/buildroot-2023.11/output/host/aarch64-buildroot-linux-gnu/sysroot)

set(CMAKE_INSTALL_PREFIX /home/codedrawing/work-dir/cDrawEnv)

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

FILE (GLOB SRC_MAIN . main.cc)
FILE (GLOB SRC_SFUD_TEST . test_sfud_c.c)

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

add_executable(cdraw_main
                ${SRC_MAIN} ${GLOG_SRC} ${CDRAW_QUEUE_SRC}
                ${CDRAW_TIME_SRC} ${CDRAW_NET_SRC} ${CDRAW_UART_SRC}
                ${CDRAW_JSON_SRC} ${SFUD_INC} ${SFUD_SRC} ${SFUD_PORT}
                ${CDRAW_SPI_SRC} ${CDRAW_CMT_SRC}
)

add_executable(cdraw_sfud_test
        ${SRC_SFUD_TEST} ${GLOG_SRC} ${CDRAW_QUEUE_SRC}
        ${CDRAW_TIME_SRC} ${CDRAW_NET_SRC} ${CDRAW_UART_SRC}
        ${CDRAW_JSON_SRC} ${SFUD_INC} ${SFUD_SRC} ${SFUD_PORT}
        ${CDRAW_SPI_SRC} ${CDRAW_CMT_SRC}
)

# 创建动态库
add_library(cDrawD SHARED
        ${GLOG_SRC} ${CDRAW_QUEUE_SRC}
        ${CDRAW_TIME_SRC} ${CDRAW_NET_SRC} ${CDRAW_UART_SRC}
        ${CDRAW_JSON_SRC} ${SFUD_INC} ${SFUD_SRC} ${SFUD_PORT}
        ${CDRAW_SPI_SRC} ${CDRAW_CMT_SRC}
)

# 创建静态库
add_library(cDrawS STATIC
        ${GLOG_SRC} ${CDRAW_QUEUE_SRC}
        ${CDRAW_TIME_SRC} ${CDRAW_NET_SRC} ${CDRAW_UART_SRC}
        ${CDRAW_JSON_SRC} ${SFUD_INC} ${SFUD_SRC} ${SFUD_PORT}
        ${CDRAW_SPI_SRC} ${CDRAW_CMT_SRC}
)


target_link_libraries(cdraw_main ${lib})
target_link_libraries(cdraw_sfud_test ${lib})

# 添加cdrawTools的头文件
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cdrawTools/
        DESTINATION include/cdrawTools
        FILES_MATCHING PATTERN "*.h")

# 添加cdrawStruct的头文件
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cdrawStruct/
        DESTINATION include/cdrawStruct
        FILES_MATCHING PATTERN "*.h")

# 安装可执行文件、库文件和头文件
install(TARGETS cdraw_main cDrawD cDrawS
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/
        DESTINATION include/3rdparty
        FILES_MATCHING PATTERN "*.h")