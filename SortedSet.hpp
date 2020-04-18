#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp> 
#include <ext/pb_ds/tree_policy.hpp>
#include "semaphore.hpp"
using namespace std;
using namespace __gnu_pbds; 
#define ordered_set tree<pair<int, string> , null_type,less<pair<int, string> >, rb_tree_tag,tree_order_statistics_node_update> 
class SortedSet{
public:
    map< string, ordered_set > zset;
    map< string, map< string, int > > cache_for_val;
    long readCount=0;
    long INF = 1e11;
    Semaphore resourceAccess;
    Semaphore readCountAccess;
    Semaphore serviceQueue ;

    SortedSet(){

    };
    ~SortedSet() {
        
    };

    void zadd(string key, string member, int score){
        serviceQueue.wait();   
        resourceAccess.wait(); 
        serviceQueue.signal();
        if(!cache_for_val[key].count(member)){
            zset[key].insert({score,member});
            cache_for_val[key][member]=score;
        }
        resourceAccess.signal(); 
    }

    string zrank(string key, string member){
        string data;   
        serviceQueue.wait();
        readCountAccess.wait();

        if(readCount == 0)
            resourceAccess.wait();
        readCount++;
        serviceQueue.signal();
        readCountAccess.signal();
        
        if(cache_for_val[key].count(member)){
            int score=cache_for_val[key][member];
            int getIndex=zset[key].order_of_key(make_pair(score, member));
            data = "(integer) " + to_string(getIndex);
        } else {
            data = "(nil)";
        }          
        
        readCountAccess.wait();    
        readCount--;               
        if (readCount == 0)        
            resourceAccess.signal();
        readCountAccess.signal(); 
        return data;
    }

    string zrange(string key){
        string data;
        bool find=false;
        serviceQueue.wait();
        readCountAccess.wait();

        if(readCount == 0)
            resourceAccess.wait();
        readCount++;
        serviceQueue.signal();
        readCountAccess.signal();


        readCountAccess.wait();    
        readCount--;               
        if (readCount == 0)        
            resourceAccess.signal();
        readCountAccess.signal(); 
        return data;
    }

};