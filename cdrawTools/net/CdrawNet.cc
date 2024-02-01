
#include <thread>
#include "CdrawNet.h"

CdrawNet::CdrawNet  (const uint8_t id, const std::string localIp, const std::string remoteIp, const uint32_t localPort,
                    const uint32_t remotePort, const bool isWork): id_(id), local_port_(localPort), remote_port_(remotePort),
                    local_ip_(localIp), remote_ip_(remoteIp), is_work_(isWork){

    net_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&local_addr_, 0, sizeof(struct sockaddr_in));
    memset(&remote_addr_, 0, sizeof(struct sockaddr_in));

    local_addr_.sin_addr.s_addr = inet_addr(local_ip_.c_str());
    local_addr_.sin_port = htons(local_port_);
    local_addr_.sin_family = AF_INET;

    remote_addr_.sin_addr.s_addr = inet_addr(remote_ip_.c_str());
    remote_addr_.sin_port = htons(remote_port_);
    remote_addr_.sin_family = AF_INET;

    if(bind(net_fd_, (const struct sockaddr *)&local_addr_, sizeof(local_addr_)) != 0){
        LOG_WITH_MODULE(ERROR) << "bind local ip failed local ip:" << local_ip_ << "local port :" << local_port_;
        return;
    }
    LOG_WITH_MODULE(INFO) << "bind local ip success local ip:" << local_ip_ << "local port :" << local_port_;
}

CdrawNet::CdrawNet  (const uint8_t id, const std::string localIp, const uint32_t localPort, const bool isWork)
                    :id_(id), local_ip_(localIp), local_port_(localPort), is_work_(isWork) {

    net_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&local_addr_, 0, sizeof(struct sockaddr_in));

    local_addr_.sin_addr.s_addr = inet_addr(local_ip_.c_str());
    local_addr_.sin_port = htons(local_port_);
    local_addr_.sin_family = AF_INET;


    if(bind(net_fd_, (const struct sockaddr *)&local_addr_, sizeof(local_addr_)) != 0){
        LOG_WITH_MODULE(ERROR) << "bind local ip failed local ip:" << local_ip_ << "local port :" << local_port_;
        return;
    }
    LOG_WITH_MODULE(INFO) << "bind local ip success local ip:" << local_ip_ << "local port :" << local_port_;
}

CdrawNet::~CdrawNet() {
    close(net_fd_);
}

SEND_SIZE
CdrawNet::sendDateByUdp(const uint8_t *send_data, const uint32_t send_size){
    if(send_data != NULL && send_size != 0){
        return sendto(net_fd_, send_data, send_size, 0, (const struct sockaddr *)&remote_addr_, sizeof(remote_addr_));
    }else{
        LOG_WITH_MODULE(ERROR) << "send data is NULL or send size is zero";
        return -1;
    }
}

REV_SIZE
CdrawNet::revDataByUdp(uint8_t *rev_data, const uint32_t rcv_size){
    if(rev_data != NULL && rcv_size != 0){
        return recvfrom(net_fd_, static_cast<void *>(rev_data), rcv_size, 0, (struct sockaddr *)&remote_addr_,
                        reinterpret_cast<socklen_t *>(sizeof(remote_addr_)));
    }else{
        LOG_WITH_MODULE(ERROR) << "recevie data is NULL or send size is zero";
        return -1;
    }
}

template<typename T>
void
CdrawNet::sendDatesByUdpFromQueue(CdrawQueue<T> send_data_queue, const uint32_t package_size, const bool isWork){
    T send_data;
    uint8_t get_count = 0;
    this->setIsWork(isWork);
    while (true){
        send_data = send_data_queue.dequeueBlock();
        get_count++;
        if(is_work_ == false){
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            continue;
        }
        if(get_count >= package_size){
            sendto(net_fd_, &send_data, sizeof(send_data), 0, (const struct sockaddr *)&remote_addr_, sizeof(remote_addr_));
            get_count = 0;
        }else{
            continue;
        }
    }
}

template<typename T>
void
CdrawNet::revDatesByUdpToQueue(CdrawQueue<T> rev_data_queue, const uint32_t rev_size, const bool isWork){
    T rev_data;
    this->setIsWork(isWork);
    while (true){
        if(is_work_ == false){
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            continue;
        }
        recvfrom(net_fd_, &rev_data, rev_size, 0, (struct sockaddr *)&remote_addr_, sizeof(remote_addr_));
        rev_data_queue.enqueue(rev_data);
    }
}

inline
void CdrawNet::setIsWork(const bool isWork){
    is_work_ = isWork;
    LOG_WITH_MODULE(INFO) << "set is work success, current is work :" << is_work_;
}

inline bool CdrawNet::getIsWork() {
    return is_work_;
}