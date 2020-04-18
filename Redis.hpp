#include <bits/stdc++.h>
using namespace std;

#include "semaphore.hpp"
#include "RedisSet.hpp"
#include "SortedSet.hpp"

class Redis{
public:
    RedisSet rediskv;
    SortedSet sortedset;

    void set(string key, string value){
    	rediskv.set(key,value);
    }

    string expire(string key, long extime){
    	extime +=time(0);
    	bool flag=rediskv.expire(key, extime);
    	if(flag){
    		return 	"(integer) 1";
    	} else {
    		return 	"(integer) 0";
    	}
    }

    string get(string key){
    	return rediskv.get(key);
    }

    string zadd(string key, vector<pair<string,long> > &value){
    	int cnt = sortedset.zadd(key, value);
    	return "(integer) " + to_string(cnt); 
    }
    
    string zrank(string key, string member){
    	return sortedset.zrank(key, member);
    }

};