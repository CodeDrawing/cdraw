

#ifndef CDRAW_CDRAWQUEUE_H
#define CDRAW_CDRAWQUEUE_H

#include <thread>
#include <iostream>
#include <mutex>
#include <vector>

#include "module_glog.h"

#define __MODULE_NAME__  "cdrawQueue"

template<typename T>
class CdrawQueue {
private:
    T *data_;
    int head_, tail_, capacity_, count_;
    uint8_t id_;
    std::mutex mtx_;
public:
    CdrawQueue(int size, int id) : capacity_(size), head_(0), tail_(0), count_(0), id_(id) {
        LOG_WITH_MODULE(INFO) << "cdrawQueue init, id :" << id_;
        data_ = new T[size];
    }

    ~CdrawQueue() {
        LOG_WITH_MODULE(INFO) << "cdrawQueue destroy, id :" << id_;
        delete[] data_;
    }

    /**
    * @author: codeDrawing
    * @description: 判断该队列是否为空
    */
    bool isEmpty() {
        return count_ == 0;
    }

    /**
    * @author: codeDrawing
    * @description: 判断该队列是否已满
    */
    bool isFull() {
        return count_ == capacity_;
    }

    /**
    * @author: codeDrawing
    * @description: 入队
    */
    uint8_t enqueue(const T item) {
        std::lock_guard <std::mutex> lock(mtx_);
        if (isFull()) {
            LOG_WITH_MODULE(WARNING) << "the queue is full, id :" << id_;
            return 0;
        }
        data_[tail_] = item;
        tail_ = (tail_ + 1) % capacity_;
        count_++;
        return 1;
    }

    /**
    * @author: codeDrawing
    * @description: 出队, 非阻塞式
    */
    T dequeueNotBlock() {
        std::lock_guard <std::mutex> lock(mtx_);
        if (isEmpty() == true) {
            LOG_WITH_MODULE(WARNING) << "the queue is empty, id :" << id_;
            return 0;
        }
        T item = data_[head_];
        head_ = (head_ + 1) % capacity_;
        count_--;
        return item;
    }

    /**
     * @author: codeDrawing
     * @description: 出队, 阻塞式
    */
    T dequeueBlock() {
        std::lock_guard <std::mutex> lock(mtx_);
        while (isEmpty() == true) {
            //LOG_WITH_MODULE(WARNING) << "the queue is empty, id :" << id_;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        T item = data_[head_];
        head_ = (head_ + 1) % capacity_;
        count_--;
        return item;
    }

    /**
     * @author: codeDrawing
     * @description: 使所有元素出队，非阻塞式
    */
    std::vector <T> dequeueAllNotBlock() {
        std::lock_guard <std::mutex> lock(mtx_);
        std::vector <T> allItem;
        if (isEmpty() == true) {
            LOG(WARNING) << "the queue is empty, id :" << id_;
            return allItem;
        }
        while (isEmpty() == false) {
            T item = data_[head_];
            head_ = (head_ + 1) % capacity_;
            count_--;
            allItem.push_back(item);
        }
        return allItem;
    }

    /**
     * @author: codeDrawing
     * @description: 使所有元素出队，阻塞式
    */
    std::vector <T> dequeueAllBlock() {
        std::lock_guard <std::mutex> lock(mtx_);
        while (isEmpty() == true) {
            //LOG_WITH_MODULE(WARNING) << "the queue is empty, id :" << id_;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        std::vector <T> allItem;
        if (isEmpty() == true) {
            LOG(WARNING) << "the queue is empty, id :" << id_;
            return allItem;
        }
        while (isEmpty() == false) {
            T item = data_[head_];
            head_ = (head_ + 1) % capacity_;
            count_--;
            allItem.push_back(item);
        }
        return allItem;
    }

    /**
     * @author: codeDrawing
     * @description: 清空队列
    */
    void clear() {
        std::lock_guard <std::mutex> lock(mtx_);
        head_ = 0;
        tail_ = 0;
        count_ = 0;
    }
};


#endif //CDRAW_CDRAWQUEUE_H
