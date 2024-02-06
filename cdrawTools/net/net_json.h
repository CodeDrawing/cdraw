STRUCT(netJSON){
        //output
        STRING(out_ip, 32);
        STRING(local_ip, 32);
        FIELD(uint32, out_port);
        FIELD(uint32, local_port);

};