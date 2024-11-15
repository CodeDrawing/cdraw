__kernel void render_text(__global uchar *image, const int width, const int height,
                          __global uchar *font_buffer) {
    int x = get_global_id(0);
    int y = get_global_id(1);

    // 防止处理超出图像边界的像素
    if (x < 0 || y < 0 || x >= width || y >= height) {
        return;
    }
    // 交换R和B通道
    char tmp = image[(y * width + x)*4];
    image[(y * width + x)*4] = image[(y * width + x)*4 + 2] ;
    image[(y * width + x)*4 + 2] = tmp;

    // 计算对应的索引位置
    int index = y * width + x;
    int rgba_index = index * 4; // 因为RGBA格式每个像素占用4字节

    // 从灰度图像获取灰度值
    uchar gray_value = font_buffer[index];

    // 将灰度值复制到RGBA图像的R、G、B通道
    if(gray_value > 0) {
        image[rgba_index + 0] = gray_value; // R
        image[rgba_index + 1] = gray_value; // G
        image[rgba_index + 2] = gray_value; // B
        image[rgba_index + 3] = gray_value;        // A (设置为255，完全不透明)
    }

}
