//
// Created by cdhf on 2024/8/19.
//

#include "pcie_ep.h"
#define MODULE_NAME "pcie_ep"
pcie_ep::pcie_ep(long buf_size) {
    int ret = 0;
    ret = rk_pcie_device_init();
    if (ret != 0) {
        LOG_WITH_MODULE(ERROR) << "pcie init device error....";
    }
    ret = rk_pcie_ap_init();
    if (ret != 0) {
        LOG_WITH_MODULE(ERROR) << "pcie init ap error....";
    }
}

pcie_ep::~pcie_ep() {
    rk_pcie_ap_deinit();
    rk_pcie_device_deinit();
}