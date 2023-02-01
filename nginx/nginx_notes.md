# Engine X - nginx

## What can nginx do?
1. Web server
2. Reverse proxy server 
3. Email server

## Advantages of nginx
1. High concurrency
2. Low memory cost

## Reverse proxy server
1. Client sends connection request to the server, user doesn't know the server address, only the address of reverse proxy is pulic
2. Requests directly send to reverse proxy
3. Reverse proxy forward requests to web server
	* reverse proxy will forward requests in turns
4. Web server handle requests after receiving requests
5. Web server sends results to the reverse proxy
6. Proxy sends forward results to client

## Nginx configuration file
* Structure
```nginx
user nobody # worker process belongs to nobody, permission will be denied
worker_processes # amount of worker processes, = number of cpu cores
events{
	use epoll;
	worker_connections 1024;
}
```
* HTTP module
	* Server module: every server corresponds to a web server
```nginx
listen 80; # listen port of web server
server_name localhost; #corresponds to a domain
charset utf8;
location  # process client's requests
```
* Location module: process client's requests
```nginx
location #commands(/)
{
	# locate a static webpage based on a relative directory
	root [directory]
	# clients requests a directory, nginx needs to locate a default web page
	index [default web page]
}
```


## Nginx usage
* Deployment of static web pages
    * Default storage directory: /usr/local/nginx/html

* Deployment of reverse proxy
	* Load balancing
	The example can be review on Windows: nginx.conf

```conf
server{
	listen 80;
	server_name [domain to visit]
	location /  { 
		proxy_pass [http://xxx.com];	
	}
}
upstream xxx.com {
	server [webserver] [weight];
}
```

## Nginx data forward to fastCGI - modify nginx.conf
```conf
# converts request url to a command, http://localhost/login?user=Mars&passwd=123 for example
	- remove protocal
	- remove domain/IP and port
	- remove file name (if any)
	- remove ?....
location /login {
	# forward data to a fastCGI process
	fastcgi_pass [address] : [port]; # address: usually localhost; port: assign freely
	#the file includes environmental variables needs for http 
	include fastcgi.conf;
}
```

