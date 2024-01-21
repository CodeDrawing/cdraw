

#ifndef CDRAW_CDRAWQUEUE_H
#define CDRAW_CDRAWQUEUE_H


#include <iostream>
#include "moudle_glog.h"
#include <mutex>
#define __MODULE_NAME__  "cdrawQueue"
template <typename T>
class CdrawQueue {
private:
    T *data_;
    int head_, tail_, capacity_, count_;
    uint8_t id_;
    std::mutex mtx_;
public:
    CdrawQueue(int size, int id) : capacity_(size), head_(0), tail_(0), count_(0), id_(id){
        LOG_WITH_MODULE(INFO) << "cdrawQueue init, id :" << id;
        data_ = new T[size];
    }
    ~CdrawQueue(){
        LOG_WITH_MODULE(INFO) << "cdrawQueue destroy, id :" << id;
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
    bool isFull(){
        return count_ == capacity_;
    }

    /**
    * @author: codeDrawing
    * @description: 入队
    */
    uint8_t enqueue(const T item) {
        std::lock_guard<std::mutex> lock(mtx);
        if (isFull()) {
            LOG_WITH_MODULE(WARNING) << "the queue is full, id :" << id;
            return 0;
        }
        data[tail] = item;
        tail = (tail + 1) % capacity;
        count++;
        return 1;
    }
    /**
    * @author: codeDrawing
    * @description: 出队
    */

    T dequeue(){
        std::lock_guard<std::mutex> lock(mtx_);
        if(isEmpty() == true){
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
     * @description: 使所有元素出队
    */
    std::vector<T> dequeueAll(){
        std::lock_guard<std::mutex> lock(mtx);
        std::vector<T> allItem;
        if(isEmpty() == true){
            LOG(WARNING) << "the queue is empty, id :" << id;
            return allItem;
        }
        while(isEmpty() == false){
            T item = data[head];
            head = (head + 1) % capacity;
            count--;
            allItem.push_back(item);
        }
        return allItem;
    }

    /**
     * @author: codeDrawing
     * @description: 清空队列
    */
    void clear(){
        std::lock_guard<std::mutex> lock(mtx_);
        head_ = 0;
        tail_ = 0;
        count_ = 0;
    }
};


#endif //CDRAW_CDRAWQUEUE_H
