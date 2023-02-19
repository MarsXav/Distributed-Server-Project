#include "sql.h"

void print_error(MYSQL* conn, const char* title)
{
	fprintf(stderr, "%s:\nError %u (%s)\n", title, mysql_errno(conn), mysql_error(conn));
}

MYSQL* mysql_conn(const char* user_name, const char* password, const char*  db_name)
{
	MYSQL* conn = nullptr;
	conn = mysql_init(nullptr);
	if (conn == nullptr)
	{
		fprintf(stderr, "mysql initialisation failed");
		return nullptr;
	}
	if (mysql_real_connect(conn, nullptr, user_name, password, db_name, 0, nullptr, 0) == nullptr)
	{
		fprintf(stderr, 
			"mysql_conn failed: Error %u(%s)\n", 
			mysql_errno(conn), 
			mysql_error(conn));
		mysql_close(conn);
		return nullptr;
	}
	return conn;

}

void process_result_test(MYSQL* conn, MYSQL_RES* res_set)
{
	MYSQL_ROW row;

	// mysql_fetch_row obtains a row from mysql_store_result then store it into a line structure
	while((row = mysql_fetch_row(res_set)) != nullptr)
	{
		for (uint i = 0; i < mysql_num_fields(res_set); i++)	
		{
			if (i > 0)
			{
				fputc('\t', stdout);
			}
			printf("%s", row[i] != nullptr ? row[i] : "NULL");
		}
		fputc('\n', stdout);
	}
	if (mysql_errno(conn) != 0)
	{
		print_error(conn, "mysql_fetch_row() failed");
	}
	else
	{
		printf("%lu rows returned\n", (ulong)mysql_num_rows(res_set));
	}
}

int process_result_one(MYSQL* conn, char* sql_cmd, char* buf)
{
	int ret = 0;
	MYSQL_RES* res_set = nullptr;
	if (mysql_query(conn, sql_cmd) != 0) // execute sql commands, mysql_query() returns 0 on sucess
	{
		print_error(conn, "mysql_query error\n");
		ret = -1;
		if (res_set != nullptr)
		{
			mysql_free_result(res_set);
			return ret;
		}
	}
	res_set = mysql_store_result(conn); // generate all 
	if (res_set == nullptr)
	{
		print_error(conn, "mysql_store_result error\n");
		ret = -1;
		if (res_set != nullptr)
		{
			mysql_free_result(res_set);
			return ret;
		}
	}
	MYSQL_ROW row;
	ulong line = 0;
	// mysql_num_rows accepts results from mysql_store_result, and returns num of lines of the structure
	line = mysql_num_rows(res_set);
	if (line == 0)
	{
		ret = 1;
		if (res_set != nullptr)
		{
			mysql_free_result(res_set);
			return ret;
		}
	}
	else if (line > 0 and buf == nullptr)
	{
		ret = 2;
		if (res_set != nullptr)
		{
			mysql_free_result(res_set);
			return ret;
		}
	}
	if ((row = mysql_fetch_row(res_set)) != nullptr)
	{
		if (row[0] != nullptr)
		{
			strcpy(buf, row[0]);
		}
	}

	if (res_set != nullptr)
	{
		mysql_free_result(res_set);
	}
	return ret;
}
