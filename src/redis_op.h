#ifndef REDIS_OP_H
#define REDIS_OP_H

#include <hiredis/hiredis.h>
#include <stdlib.h>
#include <string>

#define REDIS_COMMAND_SIZE 300 

// store redis commands
typedef char(*RCOMMANDS)[REDIS_COMMAND_SIZE];

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

#endif
