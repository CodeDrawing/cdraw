#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <tiff.h>


#include "sfud.h"

#define WRITE_SPI_FLASH
#define READ_SPI_FLASH_100

int main(){
    sfud_init();
    sfud_flash *dev = sfud_get_device(0);
    uint8_t read_flash_data[103] = {0};
    printf("read_flash_data : \n");
    uint32 read_addr = 0x400000;
    int read_len = 0;

#ifdef WRITE_SPI_FLASH
    int bin_fd = open("/home/cdhf/zx/hf_0242_xc7k325t.bin", O_RDONLY);
    if (bin_fd < 0) {
        printf("open file failed\n");
        return -1;
    }
    uint8_t *bin_data = (uint8_t *)malloc(1024 * 1024 * 1024);
    read_len = read(bin_fd, bin_data, 1024 * 1024 * 1024);
    printf("read_len : %d", read_len);
    sfud_erase_write(dev, 0x400000, read_len, bin_data);
    printf("write_spi_flash to chip success" );
    free(bin_data);
#endif

#ifdef READ_SPI_FLASH_100
    int read_bin_fd  = open("/home/cdhf/zx/hf_0242_xc7k325t_read.bin", O_RDWR | O_CREAT | O_TRUNC, 0666);
    uint8 *tmp_bit = NULL;
    uint8_t *tmp_buf = (uint8_t *)malloc(read_len);
    for (int i = 0; i < read_len / 100 + 1; ++i) {
        if(i * 100 < read_len - 100){
            sfud_read(dev, read_addr + (i * 100), 104, read_flash_data);
            tmp_bit = (uint8 *)(read_flash_data);
            write(read_bin_fd, tmp_bit, 100);
        }else{
            sfud_read(dev, read_addr + (i * 100), read_len - i * 100, read_flash_data);
            tmp_bit = (uint8 *)(read_flash_data);
            write(read_bin_fd, tmp_bit, read_len - i * 100);
        }
    }
    printf("read_flash_data to file success ");
#endif
    printf("\n");
    return 0;
}