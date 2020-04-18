#include <ext/pb_ds/assoc_container.hpp> 
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds; 
#define ordered_set tree<pair<long, string> , null_type,less<pair<long, string> >, rb_tree_tag,tree_order_statistics_node_update> 
class SortedSet{
public:
    map< string, ordered_set > zset;
    map< string, map< string, long > > cache_for_val;
    long readCount=0;
    long INF = 1e11;

    // All value of Semaphore is 1
    Semaphore resourceAccess;
    Semaphore readCountAccess;
    Semaphore serviceQueue ;

    SortedSet(){

    };
    ~SortedSet() {
        
    };

    int zadd(string key, vector<pair<string,long> > &newVal){
        int valCount=0;
        serviceQueue.wait();   
        resourceAccess.wait(); 
        serviceQueue.signal();
        for(auto val:newVal)
        {
            long score = val.second;
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
        
        if(cache_for_val[key].count(member)){
            long score=cache_for_val[key][member];
            long getIndex=zset[key].order_of_key(make_pair(score, member));
            data = "(longeger) " + to_string(getIndex);
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

    vector<pair<long,string> > zrange(string key,long start, long end){
        vector<pair<long,string> > data;
        bool find=false;
        serviceQueue.wait();
        readCountAccess.wait();

        if(readCount == 0)
            resourceAccess.wait();
        readCount++;
        serviceQueue.signal();
        readCountAccess.signal();

        long sz = zset[key].size();
        if(end>=sz)end=sz-1;
        if(end<0)end+=sz;
        if(start<0) start+=sz;
        if(start>=0 && end>=start){
            auto st = zset[key].find_by_order(start);
            auto ed = zset[key].find_by_order(end);

            do {
                data.push_back(*st);
                st++;

            }while(st!=ed);
        }

        readCountAccess.wait();    
        readCount--;               
        if (readCount == 0)        
            resourceAccess.signal();
        readCountAccess.signal(); 
        return data;
    }

};