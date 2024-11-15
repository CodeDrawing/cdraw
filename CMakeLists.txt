cmake_minimum_required(VERSION 3.16)
project(cdraw)

set(CMAKE_CXX_STANDARD 17)

# 设置sysroot路径
set(CMAKE_SYSROOT /home/cdraw/work-dir/sdk/rd-linux/rk-linux5.10/buildroot/output/rockchip_rk3588/host/aarch64-buildroot-linux-gnu/sysroot/)
#
##设置编译器和连接器
set(CMAKE_C_COMPILER /home/cdraw/work-dir/sdk/rd-linux/rk-linux5.10/buildroot/output/rockchip_rk3588/host/bin/aarch64-buildroot-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /home/cdraw/work-dir/sdk/rd-linux/rk-linux5.10/buildroot/output/rockchip_rk3588/host/bin/aarch64-buildroot-linux-gnu-g++)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_FIND_ROOT_PATH /home/cdraw/work-dir/sdk/rd-linux/rk-linux5.10/buildroot/output/rockchip_rk3588/host/aarch64-buildroot-linux-gnu/sysroot/)

set(CMAKE_INSTALL_PREFIX /home/cdraw/work-dir/cDrawEnv)

add_definitions("-g")
set (cxz_json_path		${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cxz_json)
set (pcie_common        ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/rk_pcie/common_dev_sdk)
set (pcie_rc_dev        ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/rk_pcie/rc_dev_sdk)
set (pcie_ep_dev        ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/rk_pcie/ep_dev_sdk)
set (glog_path        ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glog)


# 递归查找指定目录下的所有源文件
include_directories(
#        /usr/include/drm
        ${cxz_json_path}/include
        ${pcie_common}/
        ${pcie_ep_dev}/
        ${pcie_ep_dev}/rk_mem
        ${pcie_ep_dev}/rk_mem/hugepage
        ${pcie_ep_dev}/rk_mem/rkdrm/drm/include
        ${pcie_ep_dev}/rk_pcie

        ${pcie_rc_dev}/
        ${pcie_rc_dev}/libpci
        ${pcie_rc_dev}/libpci/compat
        ${pcie_rc_dev}/libpci/lib
        ${pcie_rc_dev}/rk_mem
        ${pcie_rc_dev}/rk_mem/hugepage
        ${pcie_rc_dev}/rk_mem/rkep
        ${pcie_rc_dev}/rk_pcie

        ${glog_path}/include
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
        ${CMAKE_CURRENT_SOURCE_DIR}/cdrawTools/OSD
        ${CMAKE_CURRENT_SOURCE_DIR}/cdrawTools/PCIe-rc
        ${CMAKE_CURRENT_SOURCE_DIR}/cdrawTools/PCIe-ep
)
include_directories(/home/cdraw/work-dir/sdk/rd-linux/rk-linux5.10/buildroot/output/rockchip_rk3588/host/aarch64-buildroot-linux-gnu/sysroot/usr/include/freetype2/)
include_directories(/home/cdraw/work-dir/cDrawEnv/include)
include_directories(/home/cdraw/work-dir/sdk/rd-linux/rk-linux5.10/buildroot/output/rockchip_rk3588/host/aarch64-buildroot-linux-gnu/sysroot/usr/include/drm/)

link_directories(
        ${glog_path}/lib
        ${cxz_json_path}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cxz_json/lib
        ${pcie_common}
        ${pcie_rc_dev}
        ${pcie_ep_dev}
)

#link_directories(/home/codedrawing/work-dir/cDrawEnv/lib)

set(lib
        glog
        cJSON

)
set(drm_lib
        glog
        cJSON
        freetype
        drm
        cDrawS
        mali

)

set(pcie_rc_lib
        glog
        rkpcie-rc
        pthread
)

set(pcie_ep_lib
        glog
        rkpcie-ep
        pthread
)



FILE (GLOB SRC_MAIN . main.cc)
FILE (GLOB SRC_SFUD_TEST . test_sfud_c.c)

FILE (GLOB SRC_DRM_TEST . test-drm-full.cc)
FILE (GLOB SRC_PCIE_RC . test_pcie_rc.cc)
FILE (GLOB SRC_PCIE_EP . test_pcie_ep.cc)

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
aux_source_directory(cdrawTools/OSD     CDRAW_OSD_SRC)
aux_source_directory(cdrawTools/PCIe-rc     CDRAW_PCIE_RC_SRC)
aux_source_directory(cdrawTools/PCIe-ep     CDRAW_PCIE_EP_SRC)
#aux_source_directory(${SRC_LIST}     CDRAW_PCIE_RC_SRC)


#aux_source_directory(${pcie_ep_dev}/ A)
#aux_source_directory(${pcie_ep_dev}/rk_mem B)
#aux_source_directory(${pcie_ep_dev}/rk_mem/hugepage C)
#aux_source_directory(${pcie_ep_dev}/rk_pcie D)
#aux_source_directory(${pcie_rc_dev}/ E)
#aux_source_directory(${pcie_rc_dev}/libpci F)
#aux_source_directory(${pcie_rc_dev}/libpci/compat G)
#aux_source_directory(${pcie_rc_dev}/libpci/lib H)
#aux_source_directory(${pcie_rc_dev}/rk_mem I)
#aux_source_directory(${pcie_rc_dev}/rk_mem/hugepage J)
#aux_source_directory(${pcie_rc_dev}/rk_mem/rkep K)
#aux_source_directory(${pcie_rc_dev}/rk_pcie L)
#aux_source_directory(${pcie_common}/ M)


set(SOURCE_FILES
        ${A}
        ${B}
        ${C}
        ${D}
        ${E}
        ${F}
        ${G}
        ${H}
        ${I}
        ${J}
        ${K}
        ${L}
)

add_compile_options(-fpermissive)

add_executable(cdraw_main
                ${SRC_MAIN} ${GLOG_SRC} ${CDRAW_QUEUE_SRC}
                ${CDRAW_TIME_SRC} ${CDRAW_NET_SRC} ${CDRAW_UART_SRC}
                ${CDRAW_JSON_SRC} ${SFUD_INC} ${SFUD_SRC} ${SFUD_PORT}
                ${CDRAW_SPI_SRC} ${CDRAW_CMT_SRC}
)

#add_executable(cdraw_sfud_test
#        ${SRC_SFUD_TEST} ${GLOG_SRC} ${CDRAW_QUEUE_SRC}
#        ${CDRAW_TIME_SRC} ${CDRAW_NET_SRC} ${CDRAW_UART_SRC}
#        ${CDRAW_JSON_SRC} ${SFUD_INC} ${SFUD_SRC} ${SFUD_PORT}
#        ${CDRAW_SPI_SRC} ${CDRAW_CMT_SRC}
#)

add_executable(test_drm_full
        ${SRC_DRM_TEST} ${GLOG_SRC}
        ${CDRAW_TIME_SRC} ${CDRAW_NET_SRC}
 ${CDRAW_OSD_SRC}
)
#add_executable(test_pcie_rc
#        ${SRC_PCIE_RC} ${GLOG_SRC} ${CDRAW_QUEUE_SRC}
#        ${CDRAW_TIME_SRC} ${CDRAW_CMT_SRC}
#        ${CDRAW_PCIE_RC_SRC} ${SOURCE_FILES}
##        ${pcie_common}  ${pcie_rc_dev}   ${pcie_ep_dev}
#)
#add_executable(test_pcie_ep
#        ${SRC_PCIE_EP} ${GLOG_SRC} ${CDRAW_QUEUE_SRC}
#        ${CDRAW_TIME_SRC} ${CDRAW_CMT_SRC}
#        ${CDRAW_PCIE_EP_SRC} ${SOURCE_FILES}
##        ${pcie_common}  ${pcie_rc_dev}   ${pcie_ep_dev}
#)
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
#target_link_libraries(cdraw_sfud_test ${lib})
target_link_libraries(test_drm_full ${drm_lib})
#target_link_libraries(test_pcie_rc ${pcie_rc_lib})
#target_link_libraries(test_pcie_ep ${pcie_ep_lib})

# 添加cdrawTools的头文件
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cdrawTools/
        DESTINATION include/cdrawTools
        FILES_MATCHING PATTERN "*.h")

# 添加cdrawStruct的头文件
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cdrawStruct/
        DESTINATION include/cdrawStruct
        FILES_MATCHING PATTERN "*.h")

# 安装可执行文件、库文件和头文件
install(TARGETS test_drm_full cDrawD cDrawS
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

# 添加3rdparty的头文件
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/
        DESTINATION include/3rdparty
        FILES_MATCHING PATTERN "*.h")

