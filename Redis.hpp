#include "semaphore.hpp"
#include "RedisSet.hpp"
#include "SortedSet.hpp"

class Redis{

private:
    RedisSet hasing;
    SortedSet sortedSet;

public:

    string set(string key, string value){
        if(sortedSet.isExist(key)){
            sortedSet.del({key});
        }
    	hasing.set(key,value);
        return "ok";
    }

    string expire(string key, long long extime){
    	extime +=time(0);
    	bool flag=hasing.expire(key, extime);
    	return 	"(integer)" + to_string(flag);
    }

    string get(string key){
    	return hasing.get(key);
    }

    string ttl(string key){
        long long remTime = hasing.ttl(key);
        return "(integer) " + to_string(remTime);
    }

    string del(vector<string> &key){
        long long keydel = hasing.del(key) + sortedSet.del(key);
        return "(integer) " + to_string(keydel);
    }   

    string zadd(string key, vector< pair <string,long long> > &value){
        if(hasing.isExist(key)){
            return "ERR WRONGTYPE Operation against a key holding the wrong kind of value";
        }
    	int cnt = sortedSet.zadd(key, value);
    	return "(integer) " + to_string(cnt); 
    }

    string zrank(string key, string member){
    	return sortedSet.zrank(key, member);
    }

    string zrange(string key, long long start, long long end){
    	vector<pair<long long,string> > zran = sortedSet.zrange(key, start, end);
    	string res;
    	for(auto i:zran){
    		res.append(i.second + "\n");
    	}
    	if(res.size())res.pop_back();
    	return res;
    }


};