
/********************************************************************************
* @author: CodeDrawing
* @email: cs845391508@gmail.com
* @date: 2024/2/1 23:49
* @version: 1.0
* @description: 
********************************************************************************/


#ifndef CDRAW_UART_H
#define CDRAW_UART_H

#include <iostream>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>

#include "module_glog.h"

#define __MODULE_NAME__ "cdrawUart"
class cDrawUart {
 private:
    /**
     * @author: codeDrawing
     * @description: 串口的超时时间
    */
    uint16_t uart_read_timeout_ = 0;
    /**
     * @author: codeDrawing
     * @description: 串口的文件描述符
    */
    int uart_fd_;
    /**
     * @author: codeDrawing
     * @description:    串口的读取size，默认为0，表示非阻塞，等待超时时间，到了就退出。
     *                  如果设置为1，表示等待一个字符，如果没有字符，就一直等待。
     *                  ...
    */
    int uart_read_size_ = 0; // 非阻塞读

    /**
     * @author: codeDrawing
     * @description:    串口输入的波特率
    */
    unsigned int uart_in_speed_  = B115200;
    /**
     * @author: codeDrawing
     * @description:    串口输入的波特率
    */
    unsigned int uart_out_speed_ = B115200;

    /**
     * @author: codeDrawing
     * @description:    串口的设备名称
     *                  e.g. /dev/ttyS0
    */
    std::string uart_name_;
    struct termios tty_;




 public:
    /**
     * @author: codeDrawing
     * @description:    构造函数，传入串口的设备名称
    */
    cDrawUart(std::string uart_name) : uart_name_(uart_name_) {
        std::cout << "uart init, uart_name: " << uart_name_ << std::endl;
    }
    /**
     * @author: codeDrawing
     * @description:    析构函数
    */
    ~cDrawUart() {
        if(close(uart_fd_) < 0) {
            LOG_WITH_MODULE(ERROR) << "error " << errno << " closing " << uart_name_ << ": " << strerror(errno) << std::endl;
        }else{
            LOG_WITH_MODULE(INFO) << "uart destroy, uart_name: " << uart_name_ << std::endl;
            close(uart_fd_);
        }
    }

    /**
     * @author: codeDrawing
     * @description:    打开串口并设置串口属性，也可以设置通过 set 方法来修改部分参数
     *                  同时也可以通过 updateUartAttr() 函数来重置串口属性
     * @return:         成功返回0， 失败返回-1
    */
    int openUart();

    /**
     * @author: codeDrawing
     * @description:    根据打开的串口文件描述符，来读取数据到传入的buf里面
     * @return:         成功返回读取到的个数，失败返回-1
    */
    int uart_read(uint8_t * buf, int size);

    /**
     * @author: codeDrawing
     * @description:    根据打开的串口文件描述符，把buf里面的数据写入到串口中
     * @return:         成功返回写入的个数，失败返回-1
    */
    int uart_write(uint8_t * buf, int size);

    /**
     * @author: codeDrawing
     * @description:    设置串口属性，立即生效
     * @return:         成功返回0，失败返回负数，具体查看错误码
    */
    int updateUartAttr(){
        cfsetospeed(&tty_, uart_out_speed_); // 设置输出波特率为 9600
        cfsetispeed(&tty_, uart_in_speed_); // 设置输入波特率为 9600
        if (tcsetattr(uart_fd_, TCSANOW, &tty_) != 0) {
            LOG_WITH_MODULE(ERROR) << "error " << errno << " from tcsetattr: " << strerror(errno) << std::endl;
            return -1;
        }
        return 0;
    }

    /**
     * @author: codeDrawing
     * @description:    串口超时的set/get方法
     * @return:
    */
    bool setUartReadTimeout(uint16_t timeout) {
        uart_read_timeout_ = timeout;
        return true;
    }
    uint16_t getUartReadTimeout() {
        return uart_read_timeout_;
    }
    /**
     * @author: codeDrawing
     * @description:    串口读取等待的字符个数的set/get方法
     * @return:
    */
    bool setUartReadSize(int size) {
        uart_read_size_ = size;
        return true;
    }
    int getUartReadSize() {
        return uart_read_size_;
    }
    /**
     * @author: codeDrawing
     * @description:    串口的输入波特率的set/get方法
     * @return:
    */
    bool setUartInSpeed(unsigned int speed) {
        uart_in_speed_ = speed;
        return true;
    }
    unsigned int getUartInSpeed() {
        return uart_in_speed_;
    }
    /**
     * @author: codeDrawing
     * @description:    串口输出的set/get方法
     * @return:
    */
    bool setUartOutSpeed(unsigned int speed) {
        uart_out_speed_ = speed;
        return true;
    }
    unsigned int getUartOutSpeed() {
        return uart_out_speed_;
    }
    /**
     * @author: codeDrawing
     * @description:    返回串口的设备名称
     * @return:
    */
    std::string getUartName() {
        return uart_name_;
    }
    /**
     * @author: codeDrawing
     * @description:    返回串口的文件描述符
     * @return:
    */
    int getUartFd() {
        return uart_fd_;
    }

};


#endif //CDRAW_UART_H
