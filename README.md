# Run Instructions
Complie the server using the following command:
```
 g++ server.cpp -o server -lpthread
```

Next, complie the included client using the command
```
g++ client.cpp -o client -lpthread
```
Boot up the server using,
```
./server
```
Boot up the client using,
```
./client
```
Begin entering the commands! You can also have multiple clients connected at once.
# Functionality

- **SET** - Store ${key,value}$ pair in the cache. Average runtime complexity - $O(1)$
- **GET**  - Retrive $value$ associated to $key$ from cache. Average runtime complexity - $O(1)$ 
- **DELETE** - Delete $key$ and associated $value$from cache. Average runtime complexity - $O(1)$ 
- **TTL** -  Display time-to-live of $key$ in cache. Average runtime complexity - $O(1)$ 
- **EXPIRE** - Set an expirty time for a given $key$. Average runtime complexity - $O(1)$ 
- **ZADD** -  Add an element to an ordered set. Runtime complexity $O(log(N))$  where $N$ is number of element associated to  $key$ of the ordered set.
- **ZRANGE** - Display elements within specified range in an ordered set. Runtime complexity - $O(M+log(N))$ where $N$ is number of element associated to  $key$ of the ordered set.
- **ZRANK** - Display rank of element in an ordered set.  Runtime complexity -  $O(log(N))$ where $N$ is number of element associated to  $key$ of the ordered set.


- **Persistence** - The cache data persists through multiple sessions. It periodically takes a snapshot of data within the cache and reloads the data upon startup. Additionally, it automatically creates a snapshot upon termination.


# Why C++ ?

The key reason is performance. While C++ lacks the elegance and ease-of-use when compared to other languages like Python, it makes up for it in performance.
Since Redis is meant to be used primarily as an in-memory cache/database, operations performed must be lightning fast. With C++, we have complete control of memory management and there is no risk in operation stall due to garbage collection. 
Furthermore, the STL (Standard Template Library) offers fantastic multithreading support including asynchronous threads, mutexes, locks, condition variables etc that when combined, can result in highly effecient programs that can process large inputs in paralle.

# Futher Improvements

- Expired keys are not removed from the cache immediately upon expiry. Instead, they are invalidated after a certain amount of time. Immediate expiry is an improvement that can result in freeing up extra cache space.
- The parsing can be improved to support input options.
- It would be worthwhile to explore lock-free structures to reduce average waiting time.

# Data Structures

- A Hash Map was used for implementing the GET/SET/EXPIRE/TTL/DELETE commands . Through this data structure, we are able to support the mentioned operations in an average runtime complexity of $O(1)$ which is constant time.
- For the ordered set functionality, an inbuilt container named 'Ordered set' has been used. Internally, it is a self balancing binary search tree through which, we can where we can find index of elements in a sorted set in $log(N)$ where $N$ is the number of elements in the set. Additionally, We can also find all element int the range between the two indices in $O(M+logn(N))$.


# Multithreading Support

Yes, the implementation offers multithreading support. 
It can handle multiple clients connected to the cache in parallel. Race conditions are handled through the use of mutexes, locks, semaphores and condition variables to assure thread-safety.
