# Common commands in Redis
## Data Type in Redis
### String type
* create a key-value pair
	- SET key value
* obtain value corresponding to a key
	- GET key
* create multiple key-value pairs
	- MSET key value [key value ...]
* get multiple key values
	- MGET key [key ...]
* append string to an already existed value
	- append key value
* obtain the string length of the value
	- STRLEN key
* decrease(increase) value(the value must be 
	- DECR(INCR) DECRBY(INCRBY)

### List
* insert one or multiple values to the key
	- LPUSH key value [value...]; RPUSH key value [value...]
* insert one or multiple values to the key only when the key existed
	- LPUSHX key value [value...]
* traverse
	- LRANGE key start stop
		- stop: -1 the last element
* delete the leftmost/rightmost element with corresponding key
	- LPOP key; RPOP key
* obtain vales with corresponding index in the list 
	- INDEX
* remove specified number of elements in the list
	- LREM key count value

### Set
* add one or multiple member elements to key
    	- SADD key member [member...]
* traverse the set
	- SMEMBERS key
* obtain the different intersections among sets
    	- SDIFF key [key...]
* obtain the intersections among sets
	- SINTER key [key...]
* obtain the union among sets
    	- SUNION key [key...]
* store the results of SDIFF/SINTER/SUNION to a destination
	- SDIFFSTORE new_key key [key...]
* delete a member in a set
    	- SREM member [member...]

### SortedSet
key -> string
value [score member]
* add elements
	- ZADD key score member [score member...]
* traverse
    	- ZRANGE key start stop [withscores]
	- ZREVRANGE key start stop [withscores]
* count number of elements within a range
    	- ZCOUNT key min max
* show rank of a element in the set
	- ZRANK key member
	- ZREVRANK key member
* remove one or more members
    	- ZREM memberm [member]

### Hash
key -> string
value -> hash([key, value], [key, value], ...)
* add elements
	- HSET key field value
* get values
	- HGET key field
* add multiple elements
   	- HMSET key field value [field value]
* get multiple values
	- HMGET key field [field...]
* delete a key and its values
	- HDEL key field [field]
* identify if a key-value pair exist
	- HEXISTS key field [field]
* get all values in a key
	- HGETALL key
* get all fields in a key
    	- HKEYS key
* get number of fields
 	- HLEN key

### Key
* Delete key and its values
	- DEL key [key...]
* search all keys match the pattern
	- KEYS Pattern
* identify if key exists
	- EXISTS key
* set expire time delete 
    	- EXPIRE key seconds
* return remaining time of a key
    	- ttl key
* remove timer
    	- PERSIST key
* return types of a key
    	- TYPE key

# Redis data persistance

Two ways of persistance:
* rdb
	* Default persistance method
	* persistant file xxx.rdb
	* write in disk from memory
	* high efficiency
	* write in with custom set frequency
* aof
	* Shutdown on default
	* persistance file xxx.aof
	* write in disk directly
	* low efficiency
	* write in with default set frequency - every second
* Can aof and rdb activate simutaneously?
	- Yes
* Can aof and rdb shutdown simutaneously?
	- Yes. To shutdown rdb, save "". 

