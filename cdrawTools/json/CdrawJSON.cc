
#include "CdrawJSON.h"

#define __MODULE_NAME__ "cdrawJSON"
CdrawJSON::CdrawJSON(std::string(json_path), std::string(json_name), uint8_t(id)){
    json_path_ = json_path;
    json_name_ = json_name;
    id_ = id;
    full_path_ = json_path_ + "_" +json_name_;
}
CdrawJSON::~CdrawJSON(){

}
int
CdrawJSON::file_exists() {
    int ret = access(full_path_.c_str(), F_OK);
    if(ret == 0){
        LOG_WITH_MODULE(INFO) << "file exists ret = " << ret;
    } else {
        LOG_WITH_MODULE(ERROR) << "file not exists ret = "  << ret;
    }
    //ret == 0 文件存在，反之亦然
    return ret;
}
//template<typename T>
//void CdrawJSON::save_to_json_file(void func(cJSON *, int, T*, int, int),T t){
//    cJSON* save_json = cJSON_CreateObject();
//    func(save_json, 0, &t,0 ,0);
//    FILE *cfg_json_file = fopen(json_path_.c_str(), "wb+");
//    _cJSON_Print(save_json, cfg_json_file);
//    fclose(cfg_json_file);
//    cJSON_Delete(save_json);
//}
//template<typename T>
//T CdrawJSON::get_by_json(void func(cJSON *, int, T*, int, int)){
//    T t;
//    memset(&t, 0, sizeof(t));
//    cJSON *parse_json = _cJSON_FromFile(json_path_.c_str());
//    func(parse_json, 1, &t, 0, 0);
//    return t;
//}
