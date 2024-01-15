/**
 * codeDrawing
 * 2024/1/15 17:47
 *
 */


#ifndef CDRAW_CDRAWQUEUE_H
#define CDRAW_CDRAWQUEUE_H


#include <iostream>
#include "../../3rdparty/glog/moudle_glog.h"
template <typename T>
class cdrawQueue {
private:
    T *data;
    int head, tail, capacity, count;
    uint8_t id;
public:
    cdrawQueue(int size, int id) : capacity(size), head(0), tail(0), count(0), id(id){
        LOG(INFO) << "cdrawQueue init, id :" << id;
        data = new int[size];
    }
    ~cdrawQueue(){
        LOG(INFO) << "cdrawQueue destroy, id :" << id;
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
    uint8_t enqueue(T item) {
        if (isFull()) {
            LOG(WARNING) << "the queue is full, id :" << id;
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
        if(isEmpty() == true){
            LOG(WARNING) << "the queue is empty, id :" << id;
            //return 0;
        }
        T item = data[head];
        head = (head + 1) % capacity;
        count--;
        return item;
    }

};


#endif //CDRAW_CDRAWQUEUE_H
