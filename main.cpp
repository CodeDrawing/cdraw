#include <iostream>

#include "cdrawStruct/queue/cdrawQueue.h"
int main() {

    cdrawQueue<int> queue(10, 1);
    queue.enqueue(1);
    queue.enqueue(2);
    std::cout<< "first:" << queue.dequeue() << std::endl;
    return 0;
}
