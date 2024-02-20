
//#include "CdrawQueue.h"
//#include "CdrawNet.h"
#include "CdrawUart.h"
#include "sfud.h"
#include <iostream>
#include <cstring>



#include "CdrawTime.h"
//#include "CdrawJSON.h"
//#define net_json
//#define uart_json

//#define CREATE_FILE
#define WRITE_SPI_FLASH
//#define READ_SPI_FLASH
#define __MODULE_NAME__ "CdrawMain"




int main(int argc, char **argv) {
    log_init(argv[0]);
    CdrawTime *time1 = new CdrawTime();

    sfud_init();
    sfud_flash *dev = sfud_get_device(0);
    uint8_t read_flash_data[5] = {0};
    printf("read_flash_data : \n");
    uint32 read_addr = 0x0000000;
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
    uint8_t *tmp_data = (uint8_t *)malloc(read_len + 4);
    memset(tmp_data, 0xFF, read_len + 4);
    memcpy(tmp_data, bin_data + 4, read_len);
    std::cout << "read_len :" << read_len << std::endl;
    sfud_erase_write(dev, 0x0000000, read_len, tmp_data);
    std::cout << "write_spi_flash to chip success" << std::endl;
#endif

#ifdef READ_SPI_FLASH
    int read_bin_fd  = open("/home/cdhf/zx/hf_0242_xc7k325t_read.bin", O_RDWR | O_CREAT, 0666);
    uint8 tmp_bit = 0;
    for (int i = 0; i < read_len; ++i) {
        sfud_read(dev, read_addr + i, 5, read_flash_data);
        tmp_bit = read_flash_data[4];
        write(read_bin_fd, &tmp_bit, 1);
    }
    std::cout << "read_flash_data to file success " << std::endl;
#endif

    time1->getTimeDiff();
    free(bin_data);
    printf("\n");
    return 0;
}


