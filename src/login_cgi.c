/*
 * login CGI program
 */

#include "fcgi_config.h"
#include "fcgi_stdio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "sql.h"
#include "cfg.h"
#include "redis_op.h"

int get_login_info(char *login_buf, char* user, char* pwd)
{
	int ret = 0;
	cJSON* root = cJSON_Parse(login_buf);
	if (root == NULL)
	{
		ret = -1;
		return ret;
	}
	cJSON* child1 = cJSON_GetObjectItem(root, "user");
	if (child1 == NULL)
	{
		ret = -1;
		return ret;	
	}
	strcpy(user, child1->valuestring);
	cJSON* child2 = cJSON_GetObjectItem(root, "pwd");
	if (child2 == NULL)
	{
		ret = -1;
		return ret;
	}
	strcpy(pwd, child2->valuestring);
	if (root != NULL)
	{
		cJSON_Delete(root);
		root = NULL;
	}
	return ret;
}

int check_user_pwd(char* user, char* pwd)
{
	char sql_cmd[SQL_MAX_LEN] = {0};
	int ret = 0;
	char mysql_user[256] = {0};
	char mysql_pwd[256] = {0};
	char mysql_db[256] = {0};
	get_mysql_info(mysql_user, mysql_pwd, mysql_db);
	
	// connect to the database
	MYSQL* conn = mysql_conn(mysql_user, mysql_pwd, mysql_db);
	if (conn == NULL)
	{
		return -1;
	}
	//set db encoding format
	mysql_query(conn, "set names utf8");
	
	// search for pw for the corresponding user
	sprintf(sql_cmd, "select password from user where name = \"%s\"", user);

	// process result
	char temp[128] = {0};
	process_result_one(conn, sql_cmd, temp); // store results in temp;
	if (strcmp(temp, pwd) == 0)
	{
		ret = 0;
	}
	else
	{
		ret = -1;
	}
	mysql_close(conn);
	return ret;
}

// generate token string, store into redis db
int set_token(const char* user, const char* token)
{
	int ret = 0;
	redisContext* redis_conn = NULL;
	char redis_ip[30] = {0};
	char redis_port[10] = {0};

	get_cfg_value(CFG_PATH, "redis", "ip", redis_ip);
	get_cfg_value(CFG_PATH, "redis", "port", redis_port);
	
	// connect to redis db
	redis_conn = rop_connectdb_nopwd(redis_ip, redis_port);
	if (redis_conn == NULL)
	{
		ret = -1;
		return ret;
	}

	// encode
	// ...
	
	ret = rop_setex_string(redis_conn, user, 86400, token);

	if (redis_conn != NULL)
	{
		rop_disconnect(redis_conn);	
	}
	return ret;
}

void return_login_status(char* status_num, char* token)
{
	char* out = NULL;
	cJSON *root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "code", status_num);
	cJSON_AddStringToObject(root, "token", token);
	out = cJSON_Print(root);

	cJSON_Delete(root);
	if (out != NULL)
	{
		printf(out);
		free(out);
	}

}

int main()
{
	while(FCGI_Accept() >= 0)
	{
		char* contentLength = getenv("CONTENT_LENGTH");
		int len;
		char token[128] = {0};
		printf("Content-type : text/html\r\n\r\n");
		if (contentLength == NULL)
		{
			len = 0;
		}
		else 
		{
			len = atoi(contentLength);
		}
		if (len <= 0)
		{
			printf("No data from standard input.<p>\n");
		}
		else
		{
			char buf[4*1024] = {0};
			int ret = 0;
			ret = fread(buf, 1, len, stdin);
			if (ret == 0)
			{
				continue;
			}
			char user[512] = {0};
			char pwd[512] = {0};
			get_login_info(buf, user, pwd);
			ret = check_user_pwd(user, pwd);
			if (ret == 0)
			{
				// successfully login
				memset(token, 0, sizeof(token));
				ret = set_token(user, token);
			}
			if (ret == 0)
			{
				return_login_status("000", token);
			}
			else
			{
				return_login_status("001", token);
			}
		}
	}
	return 0;
}
