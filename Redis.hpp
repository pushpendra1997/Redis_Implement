#include "semaphore.hpp"
#include "RedisSet.hpp"
#include "SortedSet.hpp"
const string REDIS_SET_BACKUP = "RsetBackup.txt";
const string SORTED_SET_BACKUP = "SsetBackup.txt";
class Redis{

private:
    decltype(std::chrono::seconds(1)) interval{std::chrono::seconds(10)};
    RedisSet hashing;
    SortedSet sortedSet;
    void serialize(){
        std::ofstream out(REDIS_SET_BACKUP);
        out<<hashing;
        out.close();
        std::ofstream out2(SORTED_SET_BACKUP);
        out2<<sortedSet;
        out2.close();

    }

    void deSerialize(){
        std::ifstream in1(REDIS_SET_BACKUP);
        if(in1.is_open()){
            in1>>hashing;
        }
        std::ifstream in2(SORTED_SET_BACKUP);
        if(in2.is_open()){
            in2>>sortedSet;
        }        
    }

    void periodicDeserialize(){
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::seconds(interval));
            this->serialize();
        }
    }
public:
    Redis(){
        this->deSerialize();
        thread([=]{periodicDeserialize();}).detach();
    }

    ~Redis(){
        this->serialize();
    }


    string set(string key, string value){
        if(sortedSet.isExist(key)){
            sortedSet.del({key});
        }
    	hashing.set(key,value);
        return "ok";
    }

    string expire(string key, long long extime){
    	extime +=time(0);
    	bool flag=hashing.expire(key, extime);
    	return 	"(integer)" + to_string(flag);
    }

    string get(string key){
    	return hashing.get(key);
    }

    string ttl(string key){
        long long remTime = hashing.ttl(key);
        return "(integer) " + to_string(remTime);
    }

    string del(vector<string> &key){
        long long keydel = hashing.del(key) + sortedSet.del(key);
        return "(integer) " + to_string(keydel);
    }   

    string zadd(string key, vector< pair <string,long long> > &value){
        if(hashing.isExist(key)){
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