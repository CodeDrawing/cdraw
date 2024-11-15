//
// Created by cdhf on 2024/5/6.
//

#ifndef CDRAW_COSD_OPENCL_H
#define CDRAW_COSD_OPENCL_H

#include <CL/cl.h>
#include <ft2build.h>
#define osd_json
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <tiff.h>
#include <stdlib.h>

#include "font_attr.h"
#include "CdrawJSON.h"
#include FT_FREETYPE_H

#define FONT_ARRAY_SIZE (30)

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
    unsigned char *all_font_buffer_;
public:
    /* variable */
    cl_program program_;
    cl_command_queue queue_;
    cl_context context_;
    cl_kernel kernel_;

    size_t img_width_;
    size_t img_height_;
    size_t buffer_size_;
    int deeping_;
    unsigned char *image_buffer_;
    FT_Library ft_;
    FT_Face face_;
    struct sFontAttr fontAttr_array_[FONT_ARRAY_SIZE];
    CdrawJSON json_;
    osdJSONArray osd_json_;

    /* function */
//    COSD_OpenCL(char *opencl_kernel_path, char *opencl_kernel_function, char *tff_path, size_t img_width_, size_t img_height_, int deeping_);

    COSD_OpenCL(char *openclKernel, char *openclKernelFunction, char *tffPath, size_t imgWidth,
                size_t imgHeight, int deeping, const std::string json_path = "/etc/cdhf/",
                const std::string json_name = "osd_param.json", int id = 0);

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

    template<class T>
    uint8_t init_by_json(void func(cJSON *, int, T*, int, int)){
            if(json_.file_exists() != 0){
                //如果不存在，则根据本类的初始化参数，创建一个json文件
                memset(&osd_json_, 0, sizeof(osdJSONArray));
                json_.call_init_json<osdJSONArray>(func, osd_json_);
            }else{
                for(int i = 0; i < FONT_ARRAY_SIZE; i++){
                    osd_json_ = json_.get_by_json<osdJSONArray>(func);
                    fontAttr_array_[i].id_ = osd_json_.osdJSONs[i].id;
                    fontAttr_array_[i].used_ = osd_json_.osdJSONs[i].used;
                    fontAttr_array_[i].font_select_ = osd_json_.osdJSONs[i].font_select;
                    fontAttr_array_[i].r_position_ = osd_json_.osdJSONs[i].r_position;
                    fontAttr_array_[i].c_position_ = osd_json_.osdJSONs[i].c_position;
                    fontAttr_array_[i].font_size_ = osd_json_.osdJSONs[i].font_size;
                }
            }


        return 0;
    }
};


#endif //CDRAW_COSD_OPENCL_H
