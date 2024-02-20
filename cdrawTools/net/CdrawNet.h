//
// Created by 84539 on 2024/1/20.
//

#ifndef CDRAW_CDRAWNET_H
#define CDRAW_CDRAWNET_H


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "CdrawQueue.h"
#include "module_glog.h"
#include "CdrawJSON.h"

typedef uint32_t SEND_SIZE;
typedef uint32_t REV_SIZE;



class CdrawNet {
 private:

    bool is_work_;
    bool use_json_;
    uint8_t id_;
    std::string local_ip_;
    std::string remote_ip_;
    int net_fd_;
    uint32_t local_port_;
    uint32_t remote_port_;
    struct sockaddr_in local_addr_;
    struct sockaddr_in remote_addr_;
    CdrawJSON json_;
    netJSON net_json_;

 public:

    /**
     * @author: codeDrawing
     * @description: this class has two ways to init, one is localIp and remoteIp, another is localIp
     *               if only has localIp, than indicate the class is used to receive data, another is used to send data and receive date
    */
    CdrawNet();
    CdrawNet(const uint8_t id, const std::string localIp, const std::string remoteIp, const uint32_t localPort,
             const uint32_t remotePort, const bool isWork = false, const bool use_json_ = false,const std::string json_path = "/etc/cdhf/net_param.json",
             const std::string json_name = "net_param");
    CdrawNet(const uint8_t id, const std::string localIp, const uint32_t localPort, const bool isWork = false, const bool use_json_ = false,
             const std::string json_path = "/etc/cdhf/net_json.json", const std::string json_name = "net_param");

    ~CdrawNet();

    /**
     * @author: codeDrawing
     * @description: send data by udp , once
    */
    SEND_SIZE sendDateByUdp(const uint8_t *send_data, const uint32_t send_size);

    /**
     * @author: codeDrawing
     * @description: receive data by udp , once
    */
    REV_SIZE revDataByUdp(uint8_t *rev_data, const uint32_t rcv_size);

    /**
     * @author: codeDrawing
     * @description: the function will send all data in queue from localAddr to remoteAddr
     *               if package_size is zero, send all data in queue, although it is 1 :)
     *               if package_size is not zero, this function will wait and until the getQueueSize() >= package_size
     *               control the function work or not through isWork in class, isWork default value is false,
     *               you can through setIsWork() to change it
     * @Warning: this function will not create a new thread,
     *           e.g. std::thread sendThread(&CdrawNet::sendDatesByUdpFromQueue<int>, queue, 10, true);
    */
    template<typename T>
        void sendDatesByUdpFromQueue(CdrawQueue<T> send_data_queue, const uint32_t package_size = 0, const bool isWork = true);

    /**
     * @author: codeDrawing
     * @description: the function will receive data from remoteAddr to localAddr
     *               control the function work or not through isWork in class, isWork default is false
     *               receive size default value is 1500
    */
    template<typename T>
        void revDatesByUdpToQueue(CdrawQueue<T> rev_data_queue, const uint32_t rev_size = 1500, const bool isWork = true);

    /**
     * @author: codeDrawing
     * @description: set isWork_ value, success return 0, failed return -1
    */
    void setIsWork(const bool isWork);

    /**
     * @author: codeDrawing
     * @description: get isWork_ value;
    */
    bool get_is_work();

    /**
     * @author: codeDrawing
     * @description:    依然不能放在.cc里面
     * @return:
    */
    template<class T>
    uint8_t init_by_json(void func(cJSON *, int, T*, int, int)){
        if(use_json_ == true){
            if(json_.file_exists() != 0){
                //如果不存在，则根据本类的初始化参数，创建一个json文件
                json_.call_init_json<netJSON>(func, net_json_);
            }else{
                net_json_ = json_.get_by_json<netJSON>(func);
                local_ip_ = net_json_.local_ip;
                local_port_ = net_json_.local_port;
                remote_ip_ = net_json_.out_ip;
                remote_port_ = net_json_.out_port;
            }
        }else{
            LOG_WITH_MODULE(WARNING) << "not use json file, please reset the net_json_ value";
            return -1;
        }
        return 0;
    }

    std::string get_local_ip();
    std::string get_remote_ip();
    uint32_t get_local_port();
    uint32_t get_remote_port();

};



#endif //CDRAW_CDRAWNET_H
