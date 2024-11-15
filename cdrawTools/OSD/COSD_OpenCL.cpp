//
// Created by cdhf on 2024/5/6.
//

#include "COSD_OpenCL.h"

#define MAX_SOURCE_SIZE (0x100000)


COSD_OpenCL::COSD_OpenCL(char *openclKernel, char *openclKernelFunction, char *tffPath,
                         size_t imgWidth, size_t imgHeight, int deeping, const std::string json_path, const std::string json_name, int id) :
        opencl_kernel_(openclKernel), opencl_kernel_function_(openclKernelFunction),
        tff_path_(tffPath), img_width_(imgWidth), img_height_(imgHeight),
        deeping_(deeping), json_(json_path, json_name, id){

    this->buffer_size_ = this->img_height_ * this->img_width_ * this->deeping_;
    this->image_buffer_ = (unsigned char *) malloc(sizeof(uint8_t) * this->buffer_size_);
    this->all_font_buffer_ = (unsigned char *) malloc(sizeof(uint8_t) * this->img_width_ * this->img_height_);
    memset(this->image_buffer_, 0, this->buffer_size_);
    memset(this->all_font_buffer_, 0, this->img_width_ * this->img_height_);
    memset(this->fontAttr_array_, 0, sizeof(this->fontAttr_array_));
    init_by_json<osdJSONArray>(sjb_bind_osdJSONArray);
}

COSD_OpenCL::~COSD_OpenCL() {
    clReleaseProgram(this->program_);
    clReleaseCommandQueue(this->queue_);
    clReleaseContext(this->context_);
    clReleaseKernel(this->kernel_);
    FT_Done_Face(this->face_);
    FT_Done_FreeType(this->ft_);
    free(image_buffer_);
    free(all_font_buffer_);
}

uint8_t COSD_OpenCL::cl_kernel_init() {
    char *kernelSource = (char *) malloc(MAX_SOURCE_SIZE);
    FILE *kernelFile = fopen(this->opencl_kernel_, "r");
    if (kernelFile == NULL) {
        fprintf(stderr, "Cannot open kernel_ source file.\n");
        return CL_BUILD_ERROR;
    }

    size_t sourceSize = fread(kernelSource, 1, MAX_SOURCE_SIZE, kernelFile);
    fclose(kernelFile);


    cl_platform_id platformId = NULL;
    cl_device_id deviceId = NULL;
    cl_uint numDevices, numPlatforms;
    cl_int ret = clGetPlatformIDs(1, &platformId, &numPlatforms);
    ret = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_GPU, 1, &deviceId, &numDevices);

    this->context_ = clCreateContext(NULL, 1, &deviceId, NULL, NULL, &ret);
    if (ret != CL_SUCCESS) {
        printf("ERROR clCreateContext: %s\n", NULL);
        return CL_BUILD_ERROR;
    }
    this->queue_ = clCreateCommandQueue(this->context_, deviceId, 0, &ret);
    if (ret != CL_SUCCESS) {
        printf("Error creating command queue_: %d\n", ret);
        return CL_BUILD_ERROR; // 或其他适当的错误处理
    }

    this->program_ = clCreateProgramWithSource(this->context_, 1, (const char **) &kernelSource,
                                              (const size_t *) &sourceSize, &ret);

    ret = clBuildProgram(this->program_, 1, &deviceId, NULL, NULL, NULL);
    if (ret != CL_SUCCESS) {
        // Error handling for failed compilation
        size_t log_size;
        clGetProgramBuildInfo(this->program_, deviceId, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        char *log = (char *) malloc(log_size);
        clGetProgramBuildInfo(this->program_, deviceId, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
        printf("0.OpenCL error with code %d happened.\n", ret);
        fprintf(stderr, "%s\n", log);
        free(log);
        return CL_BUILD_ERROR;
    }
    this->kernel_ = clCreateKernel(this->program_, this->opencl_kernel_function_, &ret);
    if (ret != CL_SUCCESS) {
        printf("Error creating kernel_: %d\n", ret);
        // 处理错误
        return CL_BUILD_ERROR;
    }

    // 查询设备的最大工作组大小：
    size_t maxWorkGroupSize;
    clGetKernelWorkGroupInfo(this->kernel_, deviceId, CL_KERNEL_WORK_GROUP_SIZE, sizeof(maxWorkGroupSize),
                             &maxWorkGroupSize, NULL);
    printf("Max work group size for this kernel_: %zu\n", maxWorkGroupSize);

    // 字库
    FT_Init_FreeType(&this->ft_);
    FT_New_Face(this->ft_, this->tff_path_, 0, &this->face_);
    /**
     * test
     */
    for (int i = 0; i < FONT_ARRAY_SIZE; ++i) {
        if(this->fontAttr_array_[i].used_ == 0){
            continue;
        }
        printf("fontAttr_array_[i].id_: %d\n", fontAttr_array_[i].id_);
        printf("fontAttr_array_[i].font_select_: %d\n", fontAttr_array_[i].font_select_);
        printf("fontAttr_array_[i].r_position_: %d\n", fontAttr_array_[i].r_position_);
        printf("fontAttr_array_[i].c_position_: %d\n", fontAttr_array_[i].c_position_);
    }

    return CL_SUCCESS;
}

uint8_t COSD_OpenCL::create_font_add_array(int id, int font_select, int r_position, int c_position, uint8_t font_size,
                                           unsigned char *buf, unsigned int font_width, unsigned int font_rows) {
    //printf("id: %d\n", id);
    this->fontAttr_array_[id].used_ = true;
    this->fontAttr_array_[id].id_ = id;
    this->fontAttr_array_[id].font_select_ = font_select;
    this->fontAttr_array_[id].r_position_ = r_position;
    this->fontAttr_array_[id].c_position_ = c_position;
    this->fontAttr_array_[id].font_size_ = font_size;

    if (font_select == 0) {
        if (font_width > this->img_width_) {
            font_width = this->img_width_;
        }
        if (font_rows > this->img_height_) {
            font_rows = this->img_height_;
        }
        if (font_width <= 0) {
            font_width = 0;
        }
        if (font_rows <= 0) {
            font_rows = 0;
        }
        this->fontAttr_array_[id].font_width_ = font_width;
        this->fontAttr_array_[id].font_rows_ = font_rows;
        this->fontAttr_array_[id].bitmap_buffer_ = buf;
    } else {
        FT_Set_Pixel_Sizes(this->face_, 0, this->fontAttr_array_[id].font_size_);
        FT_Load_Char(this->face_, this->fontAttr_array_[id].font_select_, FT_LOAD_RENDER);
        FT_Bitmap bitmap = this->face_->glyph->bitmap;

        this->fontAttr_array_[id].font_width_ = bitmap.width;
        this->fontAttr_array_[id].font_rows_ = bitmap.rows;

        this->fontAttr_array_[id].bitmap_size_ = bitmap.width * bitmap.rows;
        unsigned char *bitmap_buffer = (unsigned char *) malloc(this->fontAttr_array_[id].bitmap_size_);
        memcpy(bitmap_buffer, bitmap.buffer, this->fontAttr_array_[id].bitmap_size_);
        this->fontAttr_array_[id].bitmap_buffer_ = bitmap_buffer;  // 这里获取的是字形的灰度位图
    }

    return 0;
}

void COSD_OpenCL::process_font() {


    /*
     * 处理字符，把所有可用的字符位图拷贝到一个大的图像缓冲区中
     * */
    memset(all_font_buffer_, 0, this->img_width_ * this->img_height_);
    for (int i = 0; i < FONT_ARRAY_SIZE; i++) {
        if (!this->fontAttr_array_[i].used_) {
            continue;
        }

        this->copyBitmapToImage(this->fontAttr_array_[i].bitmap_buffer_, all_font_buffer_,
                                this->fontAttr_array_[i].font_width_, this->fontAttr_array_[i].font_rows_,
                                this->fontAttr_array_[i].r_position_, this->fontAttr_array_[i].c_position_, this->img_width_,
                                this->img_height_);
//        for (int ia = 0; ia < this->fontAttr_array_[i].font_rows; ++ia) {
//            for (int ja = 0; ja < this->fontAttr_array_[i].font_width; ++ja) {
//                printf("%02x ", this->fontAttr_array_[i].bitmap_buffer[ia * this->fontAttr_array_[i].font_width + ja]);
//            }
//            printf("\n");
//        }
//        printf("\n");
    }
    //exit(0);

    cl_int ret;
    cl_mem imgBuf = clCreateBuffer(this->context_, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, this->buffer_size_,
                                   this->image_buffer_, &ret);
    if (ret != CL_SUCCESS) {
        printf("0.OpenCL error with code %d happened.\n", ret);
        // 根据错误代码决定后续操作
    }
    // Setup kernel_ arguments
    clSetKernelArg(this->kernel_, 0, sizeof(cl_mem), &imgBuf);
    clSetKernelArg(this->kernel_, 1, sizeof(int), &this->img_width_);
    clSetKernelArg(this->kernel_, 2, sizeof(int), &this->img_height_);
    cl_mem fontArray = clCreateBuffer(this->context_, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                      this->img_width_ * this->img_height_, all_font_buffer_, &ret);
    if (ret != CL_SUCCESS) {
        printf("1.3. clCreateBuffer OpenCL error with code %d happened.\n", ret);
        // 根据错误代码决定后续操作
    }
    clSetKernelArg(this->kernel_, 3, sizeof(cl_mem), &fontArray);


    size_t globalSize[2] = {this->img_width_, this->img_height_};
    size_t localSize[2] = {16, 16}; // Set local size depending on device capabilities
    ret = clEnqueueNDRangeKernel(this->queue_, this->kernel_, 2, NULL, globalSize, NULL, 0, NULL, NULL);
    if (ret != CL_SUCCESS) {
        printf("1.2.OpenCL error with code %d happened.\n", ret);
        // 根据错误代码决定后续操作
    }
//    for (int i = 0; i < this->fontAttr_array_[0].font_rows; ++i) {
//        for (int j = 0; j < this->fontAttr_array_[0].font_width; ++j) {
//            printf("%02x ", this->fontAttr_array_[0].bitmap_buffer[i * this->fontAttr_array_[0].font_width + j]);
//        }
//        printf("\n");
//    }
//    printf("\n");
//    for (int i = 0; i < 100; ++i) {
//        printf("%02x ", this->image_buffer_[i]);
//    }
//    printf("\n");
    ret = clEnqueueReadBuffer(this->queue_, imgBuf, CL_TRUE, 0, this->buffer_size_, this->image_buffer_, 0, NULL, NULL);
    if (ret != CL_SUCCESS) {
        printf("2.OpenCL error with code %d happened.\n", ret);
        // 根据错误代码决定后续操作
        perror("clEnqueueReadBuffer");
    }
//    for (int i = 0; i < 100; ++i) {
//        printf("%02x ", this->image_buffer_[i]);
//    }
//    printf("\n");

    clReleaseMemObject(imgBuf);
    clReleaseMemObject(fontArray);

}

void COSD_OpenCL::copyBitmapToImage(unsigned char *bitmap, unsigned char *image, int bitmapWidth, int bitmapHeight,
                                    int r_position, int c_position, int imageWidth, int imageHeight) {
    // 确保字符位图不会在目标图像上越界
    int maxRow = r_position + bitmapHeight;
    int maxCol = c_position + bitmapWidth;

    // 限制字符位图的边界，防止越界复制
    if (maxRow > imageHeight) maxRow = imageHeight;
    if (maxCol > imageWidth) maxCol = imageWidth;

    for (int row = r_position; row < maxRow; ++row) {
        for (int col = c_position; col < maxCol; ++col) {
            // 计算图像中的索引和位图中的索引
            int imageIndex = row * imageWidth + col;
            int bitmapIndex = (row - r_position) * bitmapWidth + (col - c_position);

            // 复制像素值
            image[imageIndex] = bitmap[bitmapIndex];
        }
    }
}

uint8_t *COSD_OpenCL::draw_cross(int cross_width, int cross_height) {
    if (cross_width > this->img_width_) {
        cross_width = this->img_width_;
    }
    if (cross_height > this->img_height_) {
        cross_height = this->img_height_;
    }

    if (cross_width <= 0) {
        cross_width = 0;
    }
    if (cross_height <= 0) {
        cross_height = 0;
    }
    if (cross_width > this->img_width_) {
        cross_width = this->img_width_;
    }
    if (cross_height > this->img_height_) {
        cross_height = this->img_height_;
    }
    uint8_t *cross_buf = (uint8_t *) malloc(sizeof(uint8_t) * cross_width * cross_height);
    for (int i = 0; i < cross_height; i++) {
        for (int j = 0; j < cross_width; j++) {
            if ((j == (cross_width / 2)) || (i == (cross_height / 2))) {
                cross_buf[i * cross_width + j] = 0xFF;
            }
        }
    }
    return cross_buf;
}

uint8_t *COSD_OpenCL::draw_box(int box_width, int box_height, int pound_width, int type) {
    if (box_width > this->img_width_) {
        box_width = this->img_width_;
    }

    if (box_height > this->img_height_) {
        box_height = this->img_height_;
    }
    if (box_width <= 0) {
        box_width = 0;
    }

    if (box_height <= 0) {
        box_height = 0;
    }

    if (pound_width <= 0) {
        pound_width = 0;
    }
    uint8_t *cross_buf = (uint8_t *) malloc(sizeof(uint8_t) * box_width * box_height);
    for (int i = 0; i < box_height; i++) {
        for (int j = 0; j < box_width; j++) {
            if ((j <= 0 + pound_width) || (i <= 0 + pound_width) || (j >= box_width - 1 - pound_width) ||
                (i >= box_height - 1 - pound_width)) {
                if(type == 1){ // 虚线
                    if(j % 20 > 10 || i % 20 > 10){ // cDraw
                        if((j % 20 > 10 && i <= 0 + pound_width) || (i % 20 > 10 && j <= 0 + pound_width) ||
                           (j % 20 > 10 && i >= box_height - 1 - pound_width) || (i % 20 > 10 && j >= box_width - 1 - pound_width)){
                            continue;
                        }
                    }
                }
                cross_buf[i * box_width + j] = 0xFF;
                if (j == 0 || i == 0 || j == box_width - 1 || i == box_height - 1)
                    cross_buf[i * box_width + j] = 0x01;

            }
        }
    }
    return cross_buf;
}
uint8_t *COSD_OpenCL::draw_yaw_rule(int rule_width, int rule_height, int part_num, int type){
    if (rule_width > this->img_width_) {
        rule_width = this->img_width_;
    }

    if (rule_height > this->img_height_) {
        rule_height = this->img_height_;
    }
    if (rule_width <= 0) {
        rule_width = 0;
    }

    if (rule_height <= 0) {
        rule_height = 0;
    }

    if (part_num <= 0) {
        part_num = 2;
    }
    uint8_t pound_width = 2;
    uint8_t *rule_buf = (uint8_t *) malloc(sizeof(uint8_t) * rule_width * rule_height);
    int part_width = rule_width / 2 / part_num; // 计算每个部分的宽度
    for (int i = 0; i < rule_height; i++) {
        for (int j = 0; j < rule_width; j++) {
            // 画两边
            if ((j <= 0 + pound_width) || (j >= rule_width - 1 - pound_width) ||
                (i >= rule_height - 1 - pound_width)) {
                rule_buf[i* rule_width + j] = 0xFF;
            }
            // 画中间
            if(j <= rule_width / 2 - 1 + pound_width && j >= rule_width / 2 - 1 - pound_width){
                rule_buf[i* rule_width + j - 1] = 0xFF;
                rule_buf[i* rule_width + j] = 0xFF;
                rule_buf[i* rule_width + j + 1] = 0xFF;
            }
            // 画两边的等分线
            if(part_num > 1){
                if((j < rule_width / 2 || j >= rule_width / 2) && j % part_width == 0 && j != 0 && j != rule_width - 1){
                    if(i* rule_width + j - 1 < rule_width * rule_height) rule_buf[i* rule_width + j - 1] = 0xFF;
                    if(i* rule_width + j < rule_width * rule_height) rule_buf[i* rule_width + j] = 0xFF;
                    if(i* rule_width + j + 1 < rule_width * rule_height) rule_buf[i* rule_width + j + 1] = 0xFF;
//                    if((i* rule_width + j - 1) * 2 < rule_width * rule_height) rule_buf[(i* rule_width + j - 1) * 2] = 0xFF;
//                    if((i* rule_width + j) * 2 < rule_width * rule_height) rule_buf[(i* rule_width + j) * 2] = 0xFF;
//                    if((i* rule_width + j + 1) * 2 < rule_width * rule_height) rule_buf[(i* rule_width + j + 1) * 2] = 0xFF;
                }
            }
        }
    }
    return rule_buf;
}