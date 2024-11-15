
STRUCT(osdJSON){
    FIELD(uint32, id);
    FIELD(uint8, used);
    FIELD(uint32, font_select);
    FIELD(uint32, r_position);
    FIELD(uint32, c_position);
    FIELD(uint8, font_size);

};


STRUCT(osdJSONArray){
    ARRAY(osdJSON, osdJSONs, 30);

};