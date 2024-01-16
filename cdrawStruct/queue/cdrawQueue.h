/**
 * codeDrawing
 * 2024/1/15 17:47
 *
 */


#ifndef CDRAW_CDRAWQUEUE_H
#define CDRAW_CDRAWQUEUE_H


#include <iostream>
#include "../../3rdparty/glog/moudle_glog.h"
#include <mutex>
#define __MODULE_NAME__  "cdrawQueue"
template <typename T>
class cdrawQueue {
private:
    T *data;
    int head, tail, capacity, count;
    uint8_t id;
    std::mutex mtx;
public:
    cdrawQueue(int size, int id) : capacity(size), head(0), tail(0), count(0), id(id){
        LOG_WITH_MODULE(INFO) << "cdrawQueue init, id :" << id;
        data = new T[size];
    }
    ~cdrawQueue(){
        LOG_WITH_MODULE(INFO) << "cdrawQueue destroy, id :" << id;
        delete[] data;
    }

    /**
    * @author: codeDrawing
    * @description: 判断该队列是否为空
    */
    bool isEmpty() {
        return count == 0;
    }

    /**
    * @author: codeDrawing
    * @description: 判断该队列是否已满
    */
    bool isFull(){
        return count == capacity;
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
        std::lock_guard<std::mutex> lock(mtx);
        if(isEmpty() == true){
            LOG_WITH_MODULE(WARNING) << "the queue is empty, id :" << id;
            return 0;
        }
        T item = data[head];
        head = (head + 1) % capacity;
        count--;
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
        std::lock_guard<std::mutex> lock(mtx);
        head = 0;
        tail = 0;
        count = 0;
    }
};


#endif //CDRAW_CDRAWQUEUE_H
