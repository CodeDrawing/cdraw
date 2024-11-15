//
// Created by cdhf on 2024/8/19.
//

#ifndef CDRAW_PCIE_EP_H
#define CDRAW_PCIE_EP_H

#include <iostream>
#include <unistd.h>

#include "module_glog.h"


#define THREADS_MAX 4
extern "C" {
    #include "rk_pcie_ep.h"
};

struct ep_context_st{
    struct user_cmd_st *user_cmd;
    pthread_t thr_recv_rc[THREADS_MAX];
    pthread_t thr_send_rc[THREADS_MAX];
    pthread_t thr_stream_speed;
    pthread_mutex_t mutex_stream_speed;
    unsigned int stream_speed_index;
    char recv_rc_exit;
    char send_rc_exit;
    char stream_speed_exit;
};

class pcie_ep {
public:
    pcie_ep(long buf_size);
    ~pcie_ep();
private:

    struct ep_context_st ctx;
};


#endif //CDRAW_PCIE_EP_H
