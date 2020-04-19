#include <ext/pb_ds/assoc_container.hpp> 
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds; 
#define ordered_set tree<pair<long long, string> , null_type,less<pair<long long, string> >, rb_tree_tag,tree_order_statistics_node_update> 


class SortedSet{
private:
    map< string, ordered_set > zset;
    map< string, map< string, long long > > cache_for_val;
    long long readCount=0;
    long long INF = 1e11;

    // All value of Semaphore is 1
    Semaphore resourceAccess;
    Semaphore readCountAccess;
    Semaphore serviceQueue ;
public:

    SortedSet(){

    };
    ~SortedSet() {
        
    };

    int zadd(string key, vector<pair<string,long long> > &newVal){
        int valCount=0;
        serviceQueue.wait();   
        resourceAccess.wait(); 
        serviceQueue.signal();
        for(auto val:newVal)
        {
            long long score = val.second;
            string member = val.first;
            if(!cache_for_val[key].count(member)){
                zset[key].insert({score,member});
                cache_for_val[key][member]=score;
                valCount++;
            }
        }
        resourceAccess.signal(); 
        return valCount;
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
        
        if(cache_for_val.count(key) && cache_for_val[key].count(member)){
            long long score=cache_for_val[key][member];
            long long getIndex=zset[key].order_of_key(make_pair(score, member));
            data = "(integer) " + to_string(getIndex);
        } else {
            data = "nil";
        }          
        
        readCountAccess.wait();    
        readCount--;               
        if (readCount == 0)        
            resourceAccess.signal();
        readCountAccess.signal(); 
        return data;
    }

    vector<pair<long long,string> > zrange(string key,long long start, long long end){
        vector<pair<long long,string> > data;
        bool find=false;
        serviceQueue.wait();
        readCountAccess.wait();

        if(readCount == 0)
            resourceAccess.wait();
        readCount++;
        serviceQueue.signal();
        readCountAccess.signal();
        if(zset.count(key))
        {
            long long sz  = zset[key].size();
            if(end>=sz)end=sz-1;
            if(end<0)end+=sz;
            if(start<0) start+=sz;
            if(start>=0 && end>=start){
                auto st = zset[key].find_by_order(start);
                auto ed = zset[key].find_by_order(end+1);

                while(st!=ed) {
                    data.push_back(*st);
                    st++;

                }
            }
        }

        readCountAccess.wait();    
        readCount--;               
        if (readCount == 0)        
            resourceAccess.signal();
        readCountAccess.signal(); 
        return data;
    }

};