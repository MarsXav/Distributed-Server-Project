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
























