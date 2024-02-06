
/********************************************************************************
* @author: CodeDrawing
* @email: cs845391508@gmail.com
* @date: 2024/2/1 23:49
* @version: 1.0
* @description: 
********************************************************************************/


#include "CdrawUart.h"
#define __MODULE_NAME__ "cdrawUart"
int
CdrawUart::openUart() {
    uart_fd_ = open(uart_name_.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (uart_fd_ < 0) {
        LOG_WITH_MODULE(ERROR) << "error " << errno << " opening " << uart_name_ << ": " << strerror(errno)
                               << std::endl;
        return -1;
    }
    if (tcgetattr(uart_fd_, &tty_) < 0) {
        std::cout << "error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
        return -1;
    }

    tty_.c_cflag = (tty_.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // 禁用IGNBRK以获取断开的数据，不执行其他输入处理
    tty_.c_iflag &= ~IGNBRK;
    tty_.c_lflag = 0;        // 不使用信号字符，不回显
    tty_.c_oflag = 0;        // 不执行输出处理
    tty_.c_cc[VMIN] = 0;    // 非阻塞读 // 0表示立即返回 1表示等待一个字符
    tty_.c_cc[VTIME] = 5;    // 0.5秒读超时 // 十分之n

    tty_.c_cflag &= ~PARENB;  // Clear parity bit, disabling parity (most common)
    tty_.c_cflag &= ~CSTOPB;  // Clear stop field, only one stop bit used in communication (most common)
    tty_.c_cflag &= ~CSIZE;  // Clear all the size bits, then use one of the statements below
    tty_.c_cflag |= CS8;  // 8 bits per byte (most common)
    tty_.c_cflag &= ~CRTSCTS;  // Disable RTS/CTS hardware flow control (most common) // 关闭硬件流控制
    tty_.c_cflag |= CREAD | CLOCAL;  // Turn on READ & ignore ctrl lines (CLOCAL = 1) // 忽略调制解调器控制线，启用接收器
    tty_.c_lflag &= ~ICANON;
    tty_.c_lflag &= ~ECHO;  // Disable echo
    tty_.c_lflag &= ~ECHOE;  // Disable erasure
    tty_.c_lflag &= ~ECHONL;  // Disable new-line echo
    tty_.c_lflag &= ~ISIG;  // Disable interpretation of INTR, QUIT and SUSP
    tty_.c_iflag &= ~(IXON | IXOFF | IXANY);  // Turn off s/w flow ctrl // 关闭XON/XOFF流控制
    tty_.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR |
                      ICRNL);  // Disable any special handling of received bytes
    return 0;
}
int
CdrawUart::uart_read(uint8_t * buf, int size){
    if(uart_fd_ < 0){
        LOG_WITH_MODULE(ERROR) << "uart_fd_ is invalid";
        return -1;
    }
    return read(uart_fd_, buf, size);
}

int
CdrawUart::uart_write(uint8_t * buf, int size){
    if(uart_fd_ < 0){
        LOG_WITH_MODULE(ERROR) << "uart_fd_ is invalid";
        return -1;
    }
    return write(uart_fd_, buf, size);
}