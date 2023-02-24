#include "redis_op.h"

int rop_SelectDataBase(redisContext* conn, unsigned int db_no)
{
	int ret = 0;
	redisContext* reply = nullptr;
	reply = (redisContext*)redisCommand(conn, "select %d", db_no);
	if (reply == nullptr)
	{
		//log
		//...
		ret = -1;
		freeReplyObject(reply);
		return ret;
	}
	//log
	//...
	return ret;
}

int rop_FlushDataBase(redisContext* conn)
{
	int ret = 0;
	redisReply* reply = nullptr;
	reply = (redisReply*)redisCommand(conn, "FLUSHDB");
	if (reply == nullptr)
	{
		// log
		// ...
		ret = -1;
		freeReplyObject(reply);
		return ret;
	}
	//log
	//...
	return ret;
}

int rop_isKeyExists(redisContext* conn, char* key)
{
	int ret = 0;
	redisReply* reply = nullptr;
	reply = (redisReply*)redisCommand(conn, "EXISTS %s", key);
	if (reply->type != REDIS_REPLY_INTEGER)
	{
		// log
		// ...
		ret = -1;
		freeReplyObject(reply);
		return ret;
	}
	if (reply->integer == 1)
	{
		ret = 1;
	}
	else
	{
		ret = 0;
	}
	freeReplyObject(reply);
	return ret;
}

int rop_DelKey(redisContext* conn, char* key)
{
	int ret = 0;
	redisReply* reply = nullptr;
	reply  = (redisReply*)redisCommand(conn, "DEL %s", key);
	if (reply->type != REDIS_REPLY_INTEGER)
	{
		// log
		// ...
		ret = -1;
		freeReplyObject(reply);
		return ret;
	}
	if (reply->integer > 0)
	{
		ret = 0;
	}
	else 
	{
		ret = -1;
	}
	freeReplyObject(reply);
	return ret;
}

void rop_ShowKeys(redisContext* conn, char* pattern)
{
	redisReply* reply = nullptr;
	reply = (redisReply*)redisCommand(conn, "keys %s", pattern);
	if (reply->type != REDIS_REPLY_ARRAY)
	{
		//log
		//...
		freeReplyObject(reply);
		return;
	}
	for (size_t i = 0; i < reply->elements; ++i){
		printf("======[%s]=====\n", reply->element[i]->str);
	}
	freeReplyObject(reply);
	return;
}

int rop_redisAppend(redisContext* conn, RCOMMANDS cmds, int cmd_num)
{
	int ret = 0;
	redisReply* reply = nullptr;
	// append  commands into command pipes
	for (int i = 0; i < cmd_num; i++)
	{
		ret = redisAppendCommand(conn, cmds[i]);
		if (ret != REDIS_OK) 
		{
			//log
			//...
			ret = -1;			
			freeReplyObject(reply);
			return ret;
		}
		ret = 0;
	}
	// submit commands
	for (int i = 0; i < cmd_num; i++)
	{
		ret = redisGetReply(conn, (void**)&reply);
		if (ret != REDIS_OK)
		{
			ret = -1;
			// log
			// ...
			freeReplyObject(reply);
			return ret;
		}		
		freeReplyObject(reply);
		ret = 0;
	}
	return ret;

}

int rop_redisCommand(redisContext* conn, char* cmd)
{
	int ret = 0;
	redisReply* reply = nullptr;
	reply = (redisReply*)redisCommand(conn, cmd);
	if (reply == nullptr)
	{
		// log
		// ...
		ret = -1;
	}
	freeReplyObject(reply);
	return ret; 
}

redisContext* rop_connectdb_nopwd(char *ip_str, char* port_str)
{
	redisContext* conn = nullptr;
	uint16_t port = atoi(port_str);
	conn = redisConnect(ip_str, port);
	if (conn == nullptr)
	{
		//log
		//...
		return conn;
	}
	if (conn->err)
	{
		//log
		redisFree(conn);
		conn = nullptr;
		return conn;
	}
	//log
	return conn;
}

redisContext* rop_connectdb(char* ip_str, char* port_str, char* pwd)
{
	redisContext* conn = nullptr;
	uint16_t port = atoi(port_str);
	char auth_cmd[REDIS_COMMAND_SIZE];
	conn = redisConnect(ip_str, port);
	if (conn == nullptr)
	{
		//log
		//...
		return conn;
	}
	if (conn->err)
	{
		//log
		redisFree(conn);
		conn = nullptr;
		return conn;
	}
	redisReply* reply = nullptr;
	sprintf(auth_cmd, "auth %s", pwd);
	reply = (redisReply*)redisCommand(conn, auth_cmd);
	if (reply == nullptr)
	{
		conn = nullptr;
		return conn;
	}
	freeReplyObject(reply);
	//log
	return conn;
}

void rop_disconnect(redisContext* conn)
{
	if (conn == nullptr)
	{
		return;
	}
	redisFree(conn);
	//log
}

static char* make_hmset_command(char* key, uint element_num, RFIELDS fields, RVALUES values)
{
	char *cmd = nullptr;
	uint buf_size = 0;
	uint use_size = 0;
	cmd = (char*)malloc(1024*1024);
	if (cmd == nullptr)
	{
		return cmd;
	}
	memset(cmd, 0, 1024*1024);
	buf_size += 1024*1024;
	strncat(cmd, "hmset", 6);
	use_size += 5;
	strncat(cmd, " ", 1);
	use_size += 1;
	strncat(cmd, key, 200);
	use_size += 200;
	for (uint i = 0; i < element_num; i++)
	{
		strncat(cmd, " ", 1);
		use_size += 1;
		if (use_size >= buf_size)
		{
			cmd = (char*)realloc(cmd, use_size + 1024*1024);
			if (cmd == nullptr)
			{
				return cmd;
			}
			buf_size += 1024*1024;
		}
		strncat(cmd, fields[i], FIELD_ID_SIZE);
		use_size += strlen(fields[i]);
		if (use_size >= buf_size)
		{
			cmd = (char*)realloc(cmd, use_size + 1024*1024);
			if (cmd == nullptr)
			{
				return cmd;
			}
			buf_size += 1024*1024;
		}
		strncat(cmd, values[i], VALUES_ID_SIZE);
		use_size += strlen(values[i]);
		if (use_size >= buf_size)
		{
			cmd = (char*)realloc(cmd, use_size + 1024*1024);
			if (cmd == nullptr)
			{
				return cmd;
			}
			buf_size += 1024*1024;
		}
	}
	return cmd;
}

int rop_hash_set_append(redisContext* conn, char* key, RFIELDS fields, RVALUES values, int val_num)
{
	int ret = 0;
	redisReply* reply = nullptr;
	// append commands into command pipes
	for (int i = 0; i < val_num; i++)
	{
		ret = redisAppendCommand(conn, "hset %s %s %s", key, fields[i], values[i]);
		if (ret != REDIS_OK)
		{
		// log
			ret = -1;
			return ret;
		}
		ret = 0;
	}
	// submit commands
	for (int i = 0; i < val_num; i++)
	{
		ret = redisGetReply(conn, (void**)&reply);
		if (ret != REDIS_OK)
		{
			ret = -1;
			//log
			freeReplyObject(reply);
			return ret;
		}
		freeReplyObject(reply);
		ret = 0;
	}
	return ret;
}

int rop_hash_set(redisContext* conn, char* key, char* field, char* value)
{
	int ret = 0;
	redisReply* reply = nullptr;
	reply = (redisReply*)redisCommand(conn, "hset %s %s %s", key, field, value);
	if (reply == nullptr or reply->type != REDIS_REPLY_INTEGER)
	{
		ret = -1;
		freeReplyObject(reply);
		return ret;
	}
	return ret;
}

int rop_hash_get(redisContext* conn, char* key, char* field, char* value)
{
	int ret = 0;
	size_t len = 0;
	redisReply* reply = nullptr;
	reply = (redisReply*)redisCommand(conn, "hget %s %s", key, field);
	if (reply == nullptr or reply->type != REDIS_REPLY_STRING)
	{
		//log
		ret = -1;
		freeReplyObject(reply);
		return ret;
	}
	len = reply->len > VALUES_ID_SIZE ? VALUES_ID_SIZE : reply->len;
	strncpy(value, reply->str,len);
	value[len] = '\0';
	freeReplyObject(reply);
	return ret;
}

int rop_hash_del(redisContext* conn, char* key, char* field)
{
	int ret = 0;
	redisReply* reply = nullptr;
	reply = (redisReply*)redisCommand(conn, "hdel %s %s", key, field);
	if (reply->integer != 1)
	{
		ret = -1;
		freeReplyObject(reply);
		return ret;
	}
	freeReplyObject(reply);
	return ret;

}

int rop_create_or_replace_hash_table(redisContext* conn, char* key, uint element_num, RFIELDS fields, RVALUES values)
{
	int ret = 0;
	redisReply* reply = nullptr;
	char* cmd = make_hmset_command(key, element_num, fields, values);
	if (cmd == nullptr) 
	{
		ret = -1;
		free(cmd);
	}
	reply = (redisReply*)redisCommand(conn, cmd);
	if (strcmp(reply->str, "OK") != 0)
	{
		ret = -1;
		free(cmd);
		freeReplyObject(reply);
	}
	return ret;
}

int rop_list_push_append(redisContext* conn, char* key, RVALUES values, int val_num)
{
	int ret = 0;
	redisReply* reply = nullptr;
	for (int i = 0; i < val_num; i++)
	{
		ret = redisAppendCommand(conn, "lpush %s %s", key, values[i]);
		if (ret != REDIS_OK)
		{
			ret = -1;
			return ret;
		}
		ret = 0;
	}
	for (int i = 0; i < val_num; i++)
	{
		ret = redisGetReply(conn, (void**)&reply);
		if (ret != REDIS_OK)
		{
			ret = -1;
			freeReplyObject(reply);
		}
		ret = 0;
	}
	return ret;
}

int rop_list_push(redisContext* conn, char* key, char* value)
{
	int ret = 0;
	redisReply* reply = nullptr;
	reply = (redisReply*)redisCommand(conn, "LPUSH %s %s", key, value);
	if (reply->type != REDIS_REPLY_INTEGER)
	{
		ret = -1;
	}
	freeReplyObject(reply);
	return ret;
}

int rop_setex_string(redisContext* conn, const char* key, uint seconds, const char* value)
{
	int ret = 0;
	redisReply* reply = nullptr;
	reply = (redisReply*)redisCommand(conn, "setex %s %u %s", key, seconds, value);
	if (strcmp(reply->str, "OK") != 0){
		ret = -1;
		freeReplyObject(reply);
		return ret;
	}
	freeReplyObject(reply);
	return ret;
}
