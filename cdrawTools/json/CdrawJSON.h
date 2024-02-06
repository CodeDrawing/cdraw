
/********************************************************************************
* @author: CodeDrawing
* @email: cs845391508@gmail.com
* @date: 2024/2/5 15:16
* @version: 1.0
* @description: 
********************************************************************************/


#ifndef CDRAW_CDRAWJSON_H
#define CDRAW_CDRAWJSON_H

#include "ostream"
#include "string"

#include <unistd.h>

#include "module_glog.h"
#include "cxz_json.h"


class CdrawJSON {
 private:
    uint8_t id_;
    std::string json_path_;
    std::string json_name_;

 public:


    CdrawJSON(std::string(json_path), std::string(json_name), uint8_t(id));
    ~CdrawJSON();
    /**
     * @author: codeDrawing
     * @description:    放到.cc文件就有问题，
     * @return:
    */
    template<typename T>
    void
    save_to_json_file(void func(cJSON *, int, T*, int, int),T t){
        cJSON* save_json = cJSON_CreateObject();
        func(save_json, 0, &t,0 ,0);
        FILE *cfg_json_file = fopen(json_path_.c_str(), "wb+");
        _cJSON_Print(save_json, cfg_json_file);
        fclose(cfg_json_file);
        cJSON_Delete(save_json);
        LOG_WITH_MODULE(INFO) << "create a json file";
    }
    /**
     * @author: codeDrawing
     * @description:    读取json文件，初始化类的参数，如果没有json文件，就使用回调函数创建一组默认值或者根据调用该函数时类成员变量的值创建json文件
     * @return:
    */
    template<typename T>
    void
    call_init_json(void bind(cJSON *, int, T*, int, int), T t){
        save_to_json_file(bind, t);
    }
    //从文件中读取json信息
    template<typename T>
    T
    get_by_json(void func(cJSON *, int, T*, int, int)){
        T t;
        memset(&t, 0, sizeof(t));
        cJSON *parse_json = _cJSON_FromFile(json_path_.c_str());
        func(parse_json, 1, &t, 0, 0);
        return t;
    }
    int
    file_exists();
};


#endif //CDRAW_CDRAWJSON_H
