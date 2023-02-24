/*
 * cgi program for registration
 */

#include "fcgi_config.h"
#include "fcgi_stdio.h"
#include <sys/time.h>
#include "cfg.h"
#include "cJSON.h"
#include "sql.h"

// parse the json package when registrating
int get_reg_info(char* reg_buf, char* id, char* pwd, char *phone, char* email)
{
	int ret = 0;
	cJSON* root = cJSON_Parse(reg_buf);
	if (root == NULL)
	{
		ret = -1;
		return ret;
	}
	
	// user
	cJSON* child1 = cJSON_GetObjectItem(root, "userName");
	if (child1 == NULL)
	{
		ret = -1;
		if (root != NULL) 
		{
			cJSON_Delete(root);
		}
		return ret;
	}
	strcpy(id, child1->valuestring);
	
	// password
	cJSON* child2 = cJSON_GetObjectItem(root, "password");
	if (child2 == NULL)
	{
		ret = -1;
		if (root != NULL) 
		{
			cJSON_Delete(root);
		}
		return ret;
	}
	strcpy(pwd, child2->valuestring);

	// phone
	cJSON* child3 = cJSON_GetObjectItem(root, "phone");
	if (child3 == NULL)
	{
		ret = -1;
		if (root != NULL) 
		{
			cJSON_Delete(root);
		}
		return ret;
	}
	strcpy(phone, child3->valuestring);

	// email
	cJSON* child4 = cJSON_GetObjectItem(root, "email");
	if (child4 == NULL)
	{
		ret = -1;
		if (root != NULL) 
		{
			cJSON_Delete(root);
		}
		return ret;
	}
	strcpy(email, child4->valuestring);
	return ret;
}

//register user, return 0 on success, -1 on fail
int user_register(char* reg_buf)
{
	int ret = 0;
	MYSQL *conn = NULL;

	char mysql_user[256] = {0};
	char mysql_pwd[256] = {0};
	char mysql_db[256] = {0};
	ret = get_mysql_info(mysql_user, mysql_pwd, mysql_db);

	if (ret != 0)
	{
		ret = -1;
		return ret;
	}

	char user[128];
	char pwd[128];
	char tel[128];
	char email[128];
	ret = get_reg_info(reg_buf, user, pwd, tel, email);
	if (ret != 0)
	{
		ret = -1;
		return ret;
	}
	mysql_query(conn, "set names utf8");
	char sql_cmd[SQL_MAX_LEN] = {0};
	sprintf(sql_cmd, "SELECT * FROM user WHERE name = '%s'", user);
	// find if user exists
	int ret2 = 0;
	ret2 = process_result_one(conn, sql_cmd, NULL);
	if (ret2 == 2)
	{
		ret = -2;
		if (conn != NULL)
		{
			mysql_close(conn);
			return ret;
		}
	}
	struct timeval tv;
	struct tm* ptm;
	char time_str[128];

	gettimeofday(&tv, NULL);
	ptm = localtime(&tv.tv_sec);
	strftime(time_str, sizeof(time_str), "Y-%m-%d %H:%M:%S", ptm);
	sprintf(sql_cmd, "INSERT INTO user (id, password, phone, createtime, email) VALUES ('%s', '%s', '%s', '%s', '%s')", user, pwd, tel, time_str, email);
	if (mysql_query(conn, sql_cmd) != 0) 
	{
		ret = -1;
		if (conn != NULL)	
		{
			mysql_close(conn);
			return ret;
		}
	}
}

int main()
{
	
}
