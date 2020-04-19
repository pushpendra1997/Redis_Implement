

class RedisSet{
private:
    unordered_map<string, pair<string, long long> > cache;
    long long readCount=0;
    Semaphore resourceAccess;
    Semaphore readCountAccess;
    Semaphore serviceQueue;

public:
	friend std::ostream & operator << (std::ostream &out, RedisSet & obj)
	{
        obj.writeEnter();
        out << obj.cache.size() << "\n";
        for(auto& element : obj.cache){
            out << element.first << "\n" << element.second.first << "\n" << element.second.second << "\n";
        }
        obj.writeExit();
		return out;
	}

	friend std::istream & operator >> (std::istream &in,  RedisSet &obj)
	{
        obj.cache.clear();
        decltype(cache.size()) size;
		in >> size;
        for(decltype(size) i=0; i < size ;i++){
            string a,b; long long c;
            in >> a >> b >> c;
            obj.cache[a] = {b,c};
        }
		return in;
	}
     
    RedisSet(){

    };
    ~RedisSet() {

    };

    void readEnter(){
        serviceQueue.wait();
        readCountAccess.wait();

        if(readCount == 0)
            resourceAccess.wait();
        readCount++;
        serviceQueue.signal();
        readCountAccess.signal();
    }

    void readExit(){
        readCountAccess.wait();    
        readCount--;               
        if (readCount == 0)        
            resourceAccess.signal();
        readCountAccess.signal(); 
    }

    void writeEnter(){
        serviceQueue.wait();   
        resourceAccess.wait(); 
        serviceQueue.signal();
    }

    void writeExit(){
        resourceAccess.signal();
    }

    long long del(vector<string> keys){
        long long keydel = 0;
        writeEnter();
        for(auto &key:keys)
        {
            if(cache.count(key)){
                cache.erase(key);
                keydel++;
            }
        }

        writeExit();

        return keydel;

    }

    void set(string key, string value){
        writeEnter();

        cache[key]=make_pair(value, -1);

        writeExit();
    }

    bool expire(string key, long long value){
        bool check = false;

        writeEnter();

        if(cache.count(key)){
            cache[key].second = value;
            check = true;
        }

        writeExit();
        return check;
    }

    string get(string key){

        pair<string,long long> data;
        bool find = false;
        string value;

        readEnter();

        if(cache.count(key)){
            data = cache[key];
            find = true;
        }

        readExit();

        if( find && ((long long)data.second >= (long long)time(NULL)) or data.second == -1 ){
            return  data.first;
        } else {
            return "(nil)";
        }
    }

    bool isExist(string key){
        bool isexist=false;

        readEnter();

        isexist = cache.count(key);

        readExit();

        return isexist;

    }

    long long ttl(string key){
        long long remTime = -1;
        string value;

        readEnter();

        if( cache.count(key) ){
            remTime = cache[key].second - time(0);
            if(remTime < 0){
                remTime = -1;
            }
        }

        readExit();

        return remTime;

    }

};