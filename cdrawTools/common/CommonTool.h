
/********************************************************************************
* @author: CodeDrawing
* @email: cs845391508@gmail.com
* @date: 2024/2/20 14:57
* @version: 1.0
* @description: 
********************************************************************************/


#ifndef CDRAW_COMMONTOOL_H
#define CDRAW_COMMONTOOL_H

#include "module_glog.h"

#define _MODULAR_NAME_ "commonTool"

#define CDRAW_ASSERT(EXPR)                                                                               \
if (!(EXPR))                                                                                             \
{                                                                                                        \
    LOG_WITH_MODULE(ERROR) << "(" << #EXPR << ")" << "has assert failed at %s." << __FUNCTION__;         \
    while (1);                                                                                           \
}

/**
 * @author: codeDrawing
 * @description:
 * @return:
*/
inline uint8_t
get_check_sum_uint8(const uint8_t *data, const uint32_t start, const uint32_t end){
    uint8_t sum = 0;
    for (uint32_t i = start; i < end; ++i) {
        sum += data[i];
    }
    return sum;
}

/**
 * @author: codeDrawing
 * @description:    给出原始校验和，给出需要计算的校验和数据已经起始位和结束位
 * @return:         相等返回true，不相等返回false
*/
template<typename T>
inline bool
cmp_check_sum(const T original_sum, const uint8_t *data, const uint32_t start, const uint32_t end){
    T t = 0;
    t = get_check_sum_uint8(data, start, end);
    printf("t = %d\n", t);
    printf("original_sum = %d\n", original_sum);
    return original_sum == t;
}

/**
 * @author: codeDrawing
 * @description:    进度条,传入0-1的数值，某些终端可能不支持
 * @return:
*/
void printProgress(float progress) {
    int barWidth = 70; // 进度条的宽度

    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) {
            std::cout << "=";
        } else if (i == pos) {
            std::cout << ">";
        } else {
            std::cout << " ";
        }
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush(); // 清空输出缓冲区，立即显示进度条
}
class commonTool {

};


#endif //CDRAW_COMMONTOOL_H
