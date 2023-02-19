## Server configuration
```conf
location /reg
{
	fastcgi_pass localhost:10000;
	include fastcgi.conf;
}
```

## fastcgi prgram
```c
int main(){
	while(FCGI_Accept() >= 0){
		// 1. obtain post data length
		// 2. read data to buffer
		// 3. parse json object, obtain userid, password, email and phone
		// 4. connect database
		// 5. validation
		// if conflicted - alert client 
		// 6. if no conflict - store into the data base
		// 7. alert client on success -> {"code" : "002"}
		// 8. alert format
		printf("contect-type: application/json\r\n");
		printf("{\"code\": \"002\"}");
	}
}
```

## login protocal
* On server end
```conf
location /login
{
	# forward data
	fastcgi_pass localhost:10001;
	include fastcgi.conf;
}
```
