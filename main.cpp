#include <iostream>

#include "cdrawStruct/queue/cdrawQueue.h"
#include "cdrawTools/runTime/cdrawTime.h"
#define __MODULE_NAME__ "cdraw"
int log_init(char *argv)
{

// Most flags work immediately after updating values.
    FLAGS_alsologtostderr = true; //output to file and stdout
    FLAGS_log_dir = "/etc/cdhf/logs/";
    FLAGS_colorlogtostderr = true;
    FLAGS_max_log_size = 20;  //set .log file size max is 20 (MB)
    FLAGS_stop_logging_if_full_disk = true; //start stop storing log file when disk is full
    google::InstallFailureSignalHandler();
    google::InitGoogleLogging(argv);

    LOG_WITH_MODULE(INFO) << "initialization logger success";

    srand((unsigned)time(NULL));

    return 0;
}
struct my{
    int id;
    char data[1500];
};
#if 0
int main(int argc, char **argv) {
    log_init(argv[0]);

    cdrawTime time;
    cdrawQueue<my> queue(10, 1);
    struct my a;
    a.id = 1;
    strcpy(a.data, "aa");
    struct my b;
    b.id = 2;
    strcpy(b.data, "bb");
    struct my c;
    c.id = 3;
    strcpy(c.data, "cc");
    struct my d;
    d.id = 4;
    strcpy(d.data, "hhh");
    queue.enqueue(a);
    queue.enqueue(b);
    queue.enqueue(c);
    queue.enqueue(d);
    std::vector<struct my> i = queue.dequeueAll();
    std::cout<< time.getCurrentTime() << std::endl;
    int current = 0;

    while(i[current].id != 0){
        std::cout << "id:" << i[current].id << std::endl;
        std::cout << "data:" << i[current].data << std::endl;
        current++;
    }
    std::cout<< time.getTimeDiff() << std::endl;
    return 0;
}
#endif
int
main(void){
    float test[5] = {1.1, 2.2, 3.3, 4.4, 5.5};
    float dist[5] = {0};
    for (int i = 0; i < 5; ++i) {
        std::cout << "test[i]:" << test[i] << std::endl;
    }
    memcpy(dist, test, sizeof(test));
    for (int i = 0; i < 5; ++i) {
        std::cout << "dist[i]:" << dist[i] << std::endl;
    }

}
