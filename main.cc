
//#include "CdrawQueue.h"
//#include "CdrawNet.h"
#include "CdrawUart.h"
#include <iostream>
#include <cstring>

//#include "CdrawJSON.h"
#define net_json
//#define uart_json

#define __MODULE_NAME__ "CdrawMain"




int main(int argc, char **argv) {
    log_init(argv[0]);

    CdrawUart cdrawUart("/dev/ttyS0", 20, true);
    cdrawUart.init_by_json(sjb_bind_uartJSON);
//    CdrawNet CdrawNet(20, "192.168.2.242", 49200, false, true);
//    //加载
//    CdrawNet.init_by_json(sjb_bind_netJSON);
//    std::cout << "local_ip :" << CdrawNet.get_local_ip() << std::endl;
//    std::cout << "local_port :" << CdrawNet.get_local_port() << std::endl;
//    CdrawJSON cdrawJson("/etc/cdhf/net_param.json", "net_param", 20);
//    net_param netParam;
//    strcpy(netParam.local_ip, "192.168.2.242");
//    netParam.local_port = 49200;
//    cdrawJson.save_to_json_file(sjb_bind_net_param, netParam);
    return 0;
}


