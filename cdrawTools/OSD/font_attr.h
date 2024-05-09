//
// Created by cdhf on 2024/5/7.
//

#ifndef CDRAW_FONT_ATTR_H
#define CDRAW_FONT_ATTR_H
#include "iostream"
#pragma pack(1)
struct sFontAttr{
    int id;
    bool used = false;
    uint32_t font_select; 	// 外部设置
    int r_position; 	//外部设置 距离顶部的像素
    int c_position;  	//外部设置 距离左边的像素
    unsigned char *bitmap_buffer; // 自动得到
    size_t bitmap_size; 	//自动得到
    unsigned int font_width; 	// 自动得到
    unsigned int font_rows;  	// 自动得到
    uint8_t font_size; 		// 外部设置
};
#pragma pack()
#endif //CDRAW_FONT_ATTR_H
