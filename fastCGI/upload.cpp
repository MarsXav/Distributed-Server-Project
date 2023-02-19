#include "fcgi_config.h"
#include <stdlib.h>
#include <unistd.h>
#include "fcgi_stdio.h"

int main(){
	while(FCGI_Accept() >= 0)
	{
	// 1. obtain post length
	char* length = getenv("contect-length");	
	int len = atoi(length);
	// 2. read len to buffer -> json
	// 3. parse json object (user, md5, token, fileName)
	// 4. token verification (consult redis/sql)
	// 	fail return results
	// 5. search if md5 exists
	// 	if exist -> {"code":"006"}
	// 	if not -> {"code" : "007"}
	
	}
}
