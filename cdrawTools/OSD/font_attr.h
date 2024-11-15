//
// Created by cdhf on 2024/5/7.
//

#ifndef CDRAW_FONT_ATTR_H
#define CDRAW_FONT_ATTR_H
#include "iostream"
#pragma pack(1)
struct sFontAttr{
    int id_ = 0 ;
    bool used_ = false;
    uint32_t font_select_ = 20; 	// 外部设置
    int r_position_ = 500; 	//外部设置 距离顶部的像素
    int c_position_ = 500;  	//外部设置 距离左边的像素
    unsigned char *bitmap_buffer_; // 自动得到
    size_t bitmap_size_; 	//自动得到
    unsigned int font_width_; 	// 自动得到
    unsigned int font_rows_;  	// 自动得到
    uint8_t font_size_ = 30; 		// 外部设置
};
#pragma pack()
#endif //CDRAW_FONT_ATTR_H
