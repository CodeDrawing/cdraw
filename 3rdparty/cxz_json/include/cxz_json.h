#ifndef __cxz_json_h__
#define __cxz_json_h__

#include "sjb_struct_begin.h"
#include "cxz_user_def.h"
#include "sjb_struct_end.h"


#include "sjb_struct_api.h"
#include "cxz_user_def.h"
#include "sjb_struct_end.h"

static cJSON* _cJSON_FromFile(const char* filename)
{
	FILE *f=fopen(filename,"rb");fseek(f,0,SEEK_END);long len=ftell(f);fseek(f,0,SEEK_SET);
	char *data=(char*)malloc(len+1);fread(data,1,len,f);fclose(f);
	cJSON* json = cJSON_Parse(data);
	free(data);
	if (!json)
	{
		printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		return 0;
	}
	return json;
}

static cJSON* _cJSON_CreateNode(cJSON* json, const char* name)
{
	cJSON* node = cJSON_CreateObject();
	cJSON_AddItemToObject( json, name, node);
	return node;
}

static void _cJSON_Print(cJSON* json, FILE* out)
{
	char* print = cJSON_Print(json);
	if(print)
	{
        int ret = fprintf( out, "%s", print);
        fflush(out);
        free(print);
        print = NULL;
	}
}


#endif