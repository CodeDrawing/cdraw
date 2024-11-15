//
// Created by cdhf on 2024/8/16.
//

#include "pcie_rc.h"

pcie_rc::pcie_rc(long buf_size) {
    this->ctx.buff_size = buf_size;
    int ret = rk_pcie_get_pci_bus_addresses(VENDOR_ID, DEVICE_ID, &this->ctx.dev_bus);
    if(ret < 0) {
        LOG(ERROR) << "Failed to get pci bus addresses for device";
    }else {
        LOG(INFO) << "Get pci bus addresses for device success";
    }


    if(this->ctx.dev_bus.dev_max_num == 0) {
        LOG(ERROR) << "No device found";
    }else{
        LOG(INFO) << "Found " << this->ctx.dev_bus.dev_max_num << " devices";
    }
// 初始化第一个
    ctx.rk_handle = rk_pcie_device_init(VENDOR_ID, DEVICE_ID, ctx.dev_bus.pci_bus_address[0]);
    if(ctx.rk_handle == NULL) {
        LOG(ERROR) << "Failed to init device";
    }
    rk_pcie_get_ep_mode(ctx.rk_handle, &ctx.devmode);
    LOG(INFO) << "ep device init, dev mode: " << ctx.devmode.mode << ", submode: " << ctx.devmode.submode;

    ctx.dev.send_buff_num = 10;
    ctx.dev.send_buff_size = ctx.buff_size;
    ctx.dev.recv_buff_num = 10;
    ctx.dev.recv_buff_size = ctx.buff_size;
    ctx.dev.wait_timeout_ms = -1;
    ctx.dev.is_show_speed = 1;
    ctx.dev.using_rc_dma = 1;
    ctx.dev.rc_dma_chn = 0;
    pthread_mutex_init(&ctx.mutex_stream_speed, NULL);
    ret = rk_pcie_ap_init(ctx.rk_handle, &ctx.dev);
    if (ret != 0) {
        LOG(ERROR) << "Failed to init ap, ret: " << ret;
    }
    
    rk_pcie_get_ep_mode(ctx.rk_handle, &ctx.devmode);
    LOG(INFO) << "ep device init, dev mode: " << ctx.devmode.mode << ", submode: " << ctx.devmode.submode;


}

pcie_rc::~pcie_rc() {
    rk_pcie_device_deinit(ctx.rk_handle, 1);
}