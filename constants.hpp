

const std::string setStringError = "ERR ERR wrong number of arguments for 'set' command";

const std::string getStringError = "ERR ERR wrong number of arguments for 'get' command";

const std::string expireStringError = "ERR ERR wrong number of arguments for 'expire' command";

const std::string zaddStringError = "ERR ERR wrong number of arguments for 'zadd' command";

const std::string zrankStringError = "ERR ERR wrong number of arguments for 'zrank' command";

const std::string zrangeStringError = "ERR ERR wrong number of arguments for 'zrange' command";

const std::string ttlStringError = "ERR ERR wrong number of arguments for 'ttl' command";

const std::string delStringError = "ERR ERR wrong number of arguments for 'del' command";

const std::string integerStringError = "ERR ERR value is not an integer or out of range";

const std::string unkownStringError = "ERR Unknown or disabled command";

const std::string emptyStringError = "NULL String";
 
std::unordered_map<std::string,int> target = { {"SET", 1}, {"GET", 2}, {"EXPIRE", 3}, {"ZADD", 4}, {"ZRANK", 5}, {"ZRANGE", 6} , {"TTL", 7} , {"DEL", 8}};
    
    
    
    

