#ifndef SQL_H
#define SQL_H

#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

// print error messages of sql operations
void print_error(MYSQL* conn, const char* title);

// connect to a mysql database, connects to a db on success, return nullptr on fail
MYSQL* mysql_conn(const char* user_name, const char* password, const char*  db_name);

// process results of database
void process_result_test(MYSQL* conn, MYSQL_RES* res_set);

// process sql results, store the data in a buffer, return 0 on success and store data, return 1 on no records are found, return 2 on found but not stored, return -1 on fail
int process_result_one(MYSQL* conn, char* sql_cmd, char* buf);


#endif
