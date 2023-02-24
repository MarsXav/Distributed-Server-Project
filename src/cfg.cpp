#include "cfg.h"

int get_cfg_value(const char* profile, const char* title, const char* key, char* value)
{
	int ret = 0;
	char* buf = nullptr;
	FILE* fp = nullptr;
	if (profile == nullptr or title == nullptr or key == nullptr or value == nullptr)
	{
		return -1;
	}
	fp = fopen(profile, "rb");
	if (fp == nullptr)
	{
		perror("fopen");
		ret = -1;
		goto END;
	}

	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	buf = (char*)calloc(1, size+1);
	if (buf == nullptr)
	{
		perror("calloc");
		ret = -1;
		goto END;
	}
	fread(buf, 1, size, fp);
	cJSON* root = cJSON_Parse(buf);
	if (root == nullptr)
	{
		ret = -1;
		goto END;
	}
	cJSON* father = cJSON_GetObjectItem(root, title);
	if (father == nullptr)
	{
		ret = -1;
		goto END;
	}
	cJSON* son = cJSON_GetObjectItem(father, key);
	if (son == nullptr)
	{
		ret = -1;
		goto END;
	}
	strcpy(value, son->valuestring);
	cJSON_Delete(root);

END:
	if (fp != nullptr)
	{
		fclose(fp);
	}
	if (buf != nullptr)
	{
		free(buf);
	}
	return ret;
}

int get_mysql_info(char* mysql_user, char* mysql_pwd, char* mysql_db)
{
	if (get_cfg_value(CFG_PATH, "mysql" , "user", mysql_user) == -1)
	{
		return -1;
	}
	if (get_cfg_value(CFG_PATH, "mysql", "password", mysql_pwd) == -1)
	{
		return -1;
	}
	if (get_cfg_value(CFG_PATH, "mysql", "database", mysql_db) == -1)
	{
		return -1;
	}
	return 0;
}
