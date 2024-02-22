
//#include "CdrawQueue.h"
//#include "CdrawNet.h"
#include "CdrawUart.h"
#include "sfud.h"
#include <iostream>
#include <cstring>



#include "CdrawTime.h"
#include "CommonTool.h"
//#include "CdrawJSON.h"
//#define net_json
//#define uart_json

//#define CREATE_FILE
#define WRITE_SPI_FLASH
#define READ_SPI_FLASH_100
#define __MODULE_NAME__ "CdrawMain"




int main(int argc, char **argv) {
    log_init(argv[0]);
    CdrawTime *time1 = new CdrawTime();

    sfud_init();
    sfud_flash *dev = sfud_get_device(0);
    uint8_t read_flash_data[103] = {0};
    printf("read_flash_data : \n");
    uint32 read_addr = 0x400000;
    time1->getTimeDiff();
    int read_len = 0;

#ifdef CREATE_FILE
    uint8_t buf[10] = {0x5a, 0x5a, 0x5a, 0x5a ,0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
    int create_file_fd  = open("/home/cdhf/zx/hf_0242_xc7k325t.bin", O_RDWR | O_CREAT, 0666);

    write(create_file_fd, buf, 10);
    std::cout << "create file success " << std::endl;

#endif

#ifdef WRITE_SPI_FLASH
    int bin_fd = open("/home/cdhf/zx/hf_0242_xc7k325t.bin", O_RDONLY);
    if (bin_fd < 0) {
        printf("open file failed\n");
        return -1;
    }
    uint8_t *bin_data = (uint8_t *)malloc(1024 * 1024 * 1024);
    read_len = read(bin_fd, bin_data, 1024 * 1024 * 1024);
    uint8_t sum = get_check_sum_uint8(bin_data, 0, read_len);
    std::cout << "read_len :" << read_len << std::endl;
    sfud_erase_write(dev, 0x400000, read_len, bin_data);
    std::cout << "write_spi_flash to chip success" << std::endl;
    free(bin_data);
#endif

#ifdef READ_SPI_FLASH_100
    int read_bin_fd  = open("/home/cdhf/zx/hf_0242_xc7k325t_read.bin", O_RDWR | O_CREAT | O_TRUNC, 0666);
    uint8 *tmp_bit = NULL;
    uint8_t *tmp_buf = (uint8_t *)malloc(read_len);
    for (int i = 0; i < read_len / 100 + 1; ++i) {
        printProgress((float )i / (float)(read_len / 100 + 1));
        if(i * 100 < read_len - 100){
            sfud_read(dev, read_addr + (i * 100), 104, read_flash_data);
            tmp_bit = reinterpret_cast<uint8 *>(read_flash_data);
            write(read_bin_fd, tmp_bit, 100);
        }else{
            sfud_read(dev, read_addr + (i * 100), read_len - i * 100, read_flash_data);
            tmp_bit = reinterpret_cast<uint8 *>(read_flash_data);
            write(read_bin_fd, tmp_bit, read_len - i * 100);
        }
    }
    //没关就会出现文件写入不完整
    close(read_bin_fd);
    int read_tmp_fd = open("/home/cdhf/zx/hf_0242_xc7k325t_read.bin", O_RDONLY);
    read(read_tmp_fd, tmp_buf, read_len);
    std::cout << "read_flash_data to file success " << std::endl;
#endif
    std::cout << "check sum result: " << cmp_check_sum<uint8_t>(sum, tmp_buf, 0, (uint32_t)read_len)<< std::endl;
    time1->getTimeDiff();

    printf("\n");
    return 0;
}


