#ifndef REDIS_OP_H
#define REDIS_OP_H

#include <hiredis/hiredis.h>
#include <stdlib.h>
#include <cstring>

#define REDIS_COMMAND_SIZE 300 
#define FIELD_ID_SIZE 100
#define VALUES_ID_SIZE 1024

// store redis commands
typedef char(*RCOMMANDS)[REDIS_COMMAND_SIZE];
// store field string type
typedef char(*RFIELDS)[FIELD_ID_SIZE];
// store values string type
typedef char(*RVALUES)[VALUES_ID_SIZE];

// Select a redis database, return 0 on success, -1 on fail
int rop_SelectDataBase(redisContext* conn, unsigned int db_no);

// flush all info in the database, return 0 on success, -1 on fail
int rop_FlushDataBase(redisContext* conn);

// idnetify if key exists, return 1 if exists, on 0 if not exist, on -1 if fail
int rop_isKeyExists(redisContext* conn, char* key);

// delete a key, return 0 on success, -1 on fail
int rop_DelKey(redisContext* conn, char* key);

// print all keys match pattern
void rop_ShowKeys(redisContext* conn, char* pattern);

// execute redis commands on batch, return 0 on success, -1 on fail
int rop_redisAppend(redisContext* conn, RCOMMANDS cmds, int cmd_num);

// execute single redis command, return 0 on success, -1 on fail
int rop_redisCommand(redisContext* conn, char* cmd);

// connect redis database in tcp mode, return a connection on success, nullptr on fail, without pw
redisContext* rop_connectdb_nopwd(char *ip_str, char* port_str);

// connect redis database in tcp mode, return a connection on success, nullptr on fail, with pw
redisContext* rop_connectdb(char* ip_str, char* port_str, char* pwd);

// disconnect from a server
void rop_disconnect(redisContext* conn);

// hmset
static char* make_hmset_command(char* key, uint element_num, RFIELDS fields, RVALUES values);

// append a hash set, return 0 on success, -1 on fail
int rop_hash_set_append(redisContext* conn, char* key, RFIELDS fields, RVALUES values, int val_num);

// add a pair of data in hash table, return 0 on success, -1 on fail
int rop_hash_set(redisContext* conn, char* key, char* field, char* value);

// get a key-value pair from hash, return 0 on success, -1 on fail
int rop_hash_get(redisContext* conn, char* key, char* field, char* value);

// remove a field from a hash, return 0 on success, -1 on fail
int rop_hash_del(redisContext* conn, char* key, char* field);

// create or override a hash table
int rop_create_or_replace_hash_table(redisContext* conn, char* key, uint element_num, RFIELDS fields, RVALUES values);

// append by batch to the list's head, return 0 on success, -1 on fail
int rop_list_push_append(redisContext* conn, char* key, RVALUES values, int val_num);

// append a single data to the list
int rop_list_push(redisContext* conn, char* key, char* value);

// 
int rop_setex_string(redisContext* conn, char* user, uint seconds, char* value);
 
#endif
