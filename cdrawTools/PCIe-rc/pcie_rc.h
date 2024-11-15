//
// Created by cdhf on 2024/8/16.
//

#ifndef CDRAW_PCIE_H
#define CDRAW_PCIE_H

#include <iostream>
#include <unistd.h>

#include "module_glog.h"

extern "C" {
    #include "rk_pcie_rc.h"
}
#define VENDOR_ID 0x1d87
#define DEVICE_ID 0x356a

#define THREADS_MAX 4

struct rc_context_st {
    RK_PCIE_HANDLES rk_handle;
    struct pcie_dev_st dev;
    struct user_cmd_st *user_cmd;
    pthread_t thr_recv_ep[THREADS_MAX];
    pthread_t thr_send_ep[THREADS_MAX];
    pthread_t thr_stream_speed;
    pthread_mutex_t mutex_stream_speed;
    long buff_size;
    unsigned int stream_speed_index;
    char recv_ep_exit;
    char send_ep_exit;
    char stream_speed_exit;
    struct pcie_dev_mode devmode;
    struct pcie_dev_bus dev_bus;
};

class pcie_rc {
public:
    pcie_rc(long buf_size);
    ~pcie_rc();
private:

    struct rc_context_st ctx;
    struct pcie_buff_node *node;
};


#endif //CDRAW_PCIE_H
