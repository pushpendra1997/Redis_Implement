#include <bits/stdc++.h>
using namespace std;



class RedisSet{
public:
    map<string, pair<string, long long> > cache;
    long long readCount=0;
    Semaphore resourceAccess;
    Semaphore readCountAccess;
    Semaphore serviceQueue;
    long long INF = 10000000000;
    RedisSet(){

    };
    ~RedisSet() {

    };

    void del(string key){
        serviceQueue.wait();   
        resourceAccess.wait(); 
        serviceQueue.signal();
        cache.erase(key);
        resourceAccess.signal(); 
    }

    void set(string key, string value){
        serviceQueue.wait();   
        resourceAccess.wait(); 
        serviceQueue.signal();
        cache[key]=make_pair(value, (long long)time(0) + INF);
        resourceAccess.signal(); 
    }

    bool expire(string key, long long value){
        bool check=false;
        serviceQueue.wait();   
        resourceAccess.wait(); 
        serviceQueue.signal();
        if(cache.count(key)){
            cache[key].second=value;
            check = true;
        }
        resourceAccess.signal();
        return check;
    }

    string get(string key){
        pair<string,long long> data;
        bool find=false;
        string value;

        serviceQueue.wait();
        readCountAccess.wait();

        if(readCount == 0)
            resourceAccess.wait();
        readCount++;
        serviceQueue.signal();
        readCountAccess.signal();


        if(cache.count(key)){
            data=cache[key];
            find=true;
        }

        readCountAccess.wait();    
        readCount--;               
        if (readCount == 0)        
            resourceAccess.signal();
        readCountAccess.signal();

        if(find && (long long)data.second>=(long long)time(NULL)){
            return  data.first;
        } else {
            return "(nil)";
        }
    }

};