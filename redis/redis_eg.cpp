#include <hiredis/hiredis.h>
#include <iostream>

int main(){
	// connect redis server
	auto* conn = redisConnect("127.0.0.1", 6379);
	if (conn->err) return -1;

	// execute redis commands	
	auto *prt = redisCommand(conn, "HMSET user userName Mars password 123 age 23 sex male");
	redisReply* rply = (redisReply*)prt;
	if (rply->type == 5) 
	{ // state output
		std::cout << "status:" << rply->str << std::endl;
	}
	freeReplyObject(rply);

	// get data from database
	prt = redisCommand(conn, "HGETALL user");
	rply = (redisReply*)prt;
	if (rply->type == 2)
	{
		for (size_t i = 0; i < rply->elements; i+=2){
			printf("key: %s, value: %s\n", rply->element[i]->str, rply->element[i+1]->str);
		}	
	}
	freeReplyObject(rply);
	redisFree(conn);
	return 0;
}
