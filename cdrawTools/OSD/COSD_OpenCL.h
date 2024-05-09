//
// Created by cdhf on 2024/5/6.
//

#ifndef CDRAW_COSD_OPENCL_H
#define CDRAW_COSD_OPENCL_H

#include <CL/cl.h>
#include <ft2build.h>

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <tiff.h>
#include <stdlib.h>

#include "font_attr.h"
#include FT_FREETYPE_H

#define FONT_ARRAY_SIZE (256)

//struct sFontAttr{
//    int id;
//    bool used = false;
//    uint32_t font_select; 	// 外部设置
//    int r_position; 	//外部设置 距离顶部的像素
//    int c_position;  	//外部设置 距离左边的像素
//    unsigned char *bitmap_buffer; // 自动得到
//    size_t bitmap_size; 	//自动得到
//    int font_width; 	// 自动得到
//    int font_rows;  	// 自动得到
//    uint8_t font_size; 		// 外部设置
//};

class COSD_OpenCL {
private:
    char *opencl_kernel_;
    char *opencl_kernel_function_;
    char *tff_path_;
    unsigned char *all_font_buffer;
public:
    /* variable */
    cl_program program;
    cl_command_queue queue;
    cl_context context;
    cl_kernel kernel;

    size_t img_width;
    size_t img_height;
    size_t bufferSize;
    int deeping;
    unsigned char *imageBuffer;
    FT_Library ft;
    FT_Face face;
    struct sFontAttr fontAttr_array[FONT_ARRAY_SIZE];

    /* function */
//    COSD_OpenCL(char *opencl_kernel_path, char *opencl_kernel_function, char *tff_path, size_t img_width, size_t img_height, int deeping);

    COSD_OpenCL(char *openclKernel, char *openclKernelFunction, char *tffPath, size_t imgWidth,
                size_t imgHeight, int deeping);

    ~COSD_OpenCL();
    uint8_t cl_kernel_init();
 /*!
  * 添加ID到指定地方去，可以跳过，总共就 FONT_ARRAY_SIZE 个空间大小，然后在openCL的kernel处理函数中，就全部便利这 FONT_ARRAY_SIZE 个空间
  * @param id 这个字符的id，唯一
  * @param font_select 显示的字符在字符库中的序号，如果为0，则自定义显示内容(自己生成内存数据)
  * @param r_position 字符距离image上面的距离
  * @param c_position 字符距离image左边的距离
  * @param font_size 字符的大小
  * @param buf 自定义的内存数据
  * @return 成功返回0
  */
    uint8_t create_font_add_array(int id, int font_select, int r_position, int c_position, uint8_t font_size, unsigned char *buf = NULL, unsigned int font_width = 0, unsigned int font_rows = 0);
    void process_font();

    void copyBitmapToImage(unsigned char *bitmap, unsigned char *image, int bitmapWidth, int bitmapHeight, int r_position, int c_position, int imageWidth, int imageHeight);
    uint8_t *draw_cross(int cross_width, int cross_height);
    /**
     * 画一个矩形框
     * @param box_width 宽度
     * @param box_height 高度
     * @param pound_width 线宽
     * @param type 0:实线 1:虚线
     * @return
     */
    uint8_t *draw_box(int box_width, int box_height, int pound_width, int type = 0);

    uint8_t *draw_yaw_rule(int rule_width, int rule_height, int part_num, int type = 0);
};


#endif //CDRAW_COSD_OPENCL_H
