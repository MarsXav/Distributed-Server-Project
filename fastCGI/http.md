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
```http
- http:// protocal
- /domain
- /[commands the web server needs to process]
- ? : connector
- & : separator
```

How does server process a dynamic request?
- remove protocal
- remove domain/IP
- remove port
- remove ? and following


