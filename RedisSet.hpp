

class RedisSet{
private:
    map<string, pair<string, long long> > cache;
    long long readCount=0;
    Semaphore resourceAccess;
    Semaphore readCountAccess;
    Semaphore serviceQueue;
    long long INF = 10000000000;
public:
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
        cache[key]=make_pair(value, -1);
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

        if(find && ((long long)data.second>=(long long)time(NULL)) or data.second==-1){
            return  data.first;
        } else {
            return "(nil)";
        }
    }

    long long ttl(string key){
        long long remTime = -1;
        string value;

        serviceQueue.wait();
        readCountAccess.wait();

        if(readCount == 0)
            resourceAccess.wait();
        readCount++;
        serviceQueue.signal();
        readCountAccess.signal();


        if(cache.count(key)){
            remTime=cache[key].second-time(0);
            if(remTime < 0){
                remTime = -1;
            }
        }

        readCountAccess.wait();    
        readCount--;               
        if (readCount == 0)        
            resourceAccess.signal();
        readCountAccess.signal();

        return remTime;

    }

};