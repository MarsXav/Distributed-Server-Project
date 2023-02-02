## HTTP related
* Requests
> Four parts: request line, request head, vacant line, request data
> request line: GET/POST [GET: submitted info] [http version]
> request head: key-value pairs
> vacant line
> request data

* Response
> Four parts: state line, message header, vacant line, response data
> state line
> 2-11: response header
> vacant line
> response data: data server sends to client

* Dynamic requests
> http:// protocal
> /domain
> /[commands the web server needs to process]
> ? : connector
> & : separator

### How does server process a dynamic request?
- remove protocal
- remove domain/IP
- remove port
- remove ? and following
> * If a request line is obtained, how to find the process command?
> 1. Look into the second part of the request line
> 	- POST: the command is the second part of the request line
> 	- GET: the command is the part before '?'

## CGI
> 1. Client visits server through browser, sends a request
> 2. The server receives data, parses data
> 3. nginx cannot process all data, so forwards data to cgi
> 	* server creates a cgi process
> 4. CGI
> 	* load configuration
> 	* connect other servers such as databases
> 	* logic processing
> 	* returns results and sends back to the server
> 	* exits
> 5. Server sends data back to client
> On server end CGI will be created and destroyed consequtively

## FastCGI
> * How is FastCGI activated?
> 1. Client visits server through browser, sends a request
> 2. The server receives data, parses data
> 3. nginx cannot process all data, so forwards data to cgi
> 	* Using local socket/IP socket
> 4. How is fastCGI activated?
> 	* Through  a fastCGI process admin
> 5. fastCGI activated
> 	* Loading configuration
> 	* connect to other servers
> 	* run in cycle
> 		* if there's requests -> process -> send back thru socket
> 		* if not -> idle and block

## FastCGI working process
> 1. Client visits, sends requests
> 2. nginx web server, unable to process data client submitted
> 3. spawn-fcgi
> 	* receives data passively
> 	* bind IP and port when spwan-fcgi activates
> 4. fastCGI program
> 	* login.c -> login
> 	* use spawn-fcgi process admin to activate login program, obtain a process

## How does spawn-fcgi activate?
```sh
# prerequisite: fastCGI has completed programming -> exe login
spawn-fcgi -a $IP -p $port -f $executable_program
# IP: same in nginx.conf
# port: same in nginx.conf
```

## How to write fastCGI program
```c
#include "fcgi_stdio.h" // when compiling, gcc -I/usr/include/
int main(){
	// block function, when nginx sends data to fastCGI, unblock.
	while(FCGI_Accept() >= 0){
		//1. receive data
		// 1.1 GET - data is at the second part of request line
		char* text = getenv("QUERY_STRING");
		// 1.2 POST
		char* contentLength  = getenv("CONTENT_LENGTH"); // identify if read in turns

		//2. process based on logic
		//3. send results back to nginx
		// when sending, tell nginx the format of processed data - Content-Type
		printf("Content-type: text/html\r\n");
		printf("<html> [results] </html>");
	}
}

