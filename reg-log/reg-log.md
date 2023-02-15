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
		// obtain post data length
		// read data to buffer
		// pase json object, obtain userid, password, email and phone
		// connect database
		// validation
		// if conflicted - alert client 
		// if no conflict - store into the data base
		// alert client on success -> {"code" : "002"}
		// alert format
		printf("contect-type: application/json\r\n");
		printf("{\"code\": \"002\"}");
	}
}
```
