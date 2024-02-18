STRUCT(uartJSON){
    STRING(uart_name, 20);
    FIELD(uint16, timeout);
    FIELD(uint32, read_size);
    FIELD(uint32, in_speed);
    FIELD(uint32, out_speed);


};