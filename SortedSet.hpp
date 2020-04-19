#include <ext/pb_ds/assoc_container.hpp> 
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds; 
#define ordered_set tree<pair<long long, string> , null_type,less<pair<long long, string> >, rb_tree_tag,tree_order_statistics_node_update> 


class SortedSet{

private:
    unordered_map< string, ordered_set > zset;
    unordered_map< string, unordered_map< string, long long > > cache_for_val;
    long long readCount=0;

    // All value of Semaphore is 1
    Semaphore resourceAccess;
    Semaphore readCountAccess;
    Semaphore serviceQueue ;

public:
	friend std::ostream & operator << (std::ostream &out, SortedSet & obj)
	{
        obj.writeEnter();
        out << obj.zset.size() << "\n";
        for(auto& element : obj.zset){
            out << element.first << "\n" << element.second.size() << "\n";
            for(auto& ordEle : element.second){
                out << ordEle.first << "\n" << ordEle.second << "\n";
            }
        }

        out << obj.cache_for_val.size() << "\n";
        for(auto& element : obj.cache_for_val){
            out << element.first << "\n" << element.second.size() << "\n";
            for(auto& mapEle : element.second){
                out << mapEle.first << "\n" << mapEle.second << "\n";
            }
        }        
        obj.writeExit();
		return out;
	}

	friend std::istream & operator >> (std::istream &in,  SortedSet &obj)
	{
        obj.zset.clear();
        obj.cache_for_val.clear();

        decltype(zset.size()) size;
		in >> size;
        for(decltype(size) i=0; i < size ;i++){
            string a; decltype(size) os;
            in >> a >> os;
            ordered_set temp_s;
            for(decltype(os) j=0 ; j<os ;j++){
                long long b; string c;
                in >> b >> c;
                temp_s.insert({b,c});
            }
        obj.zset[a] = temp_s;
        }


		in >> size;
        for(decltype(size) i=0; i < size ;i++){
            string a; decltype(size) os;
            in >> a >> os;
            unordered_map< string, long long > temp_m;
            for(decltype(os) j=0 ; j<os ;j++){
                long long c; string b;
                in >> b >> c;
                temp_m[b] = c;
            }
            obj.cache_for_val[a] = temp_m;
        }        
		return in;
	}    
    SortedSet(){

    };
    ~SortedSet() {
        
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

    int zadd(string key, vector<pair<string,long long> > &newVal){
        int valCount=0;
        writeEnter();

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

        writeExit();

        return valCount;
    }

    long long del(vector<string> keys){
        long long keydel = 0;
        writeEnter();
        for(auto &key:keys)
        {
            if(zset.count(key)){
                zset.erase(key);
                keydel++;
            }
            if(cache_for_val.count(key)){
                cache_for_val.erase(key);
                keydel++;
            }
        }

        writeExit();

        return keydel;

    }

    string zrank(string key, string member){
        string data;   

        readEnter();
        
        if(cache_for_val.count(key) && cache_for_val[key].count(member)){
            long long score=cache_for_val[key][member];
            long long getIndex=zset[key].order_of_key(make_pair(score, member));
            data = "(integer) " + to_string(getIndex);
        } else {
            data = "nil";
        }          
        
        readExit();

        return data;
    }

    bool isExist(string key){
        bool isexist=false;

        readEnter();

        isexist = zset.count(key);

        readExit();

        return isexist;

    }

    
    vector<pair<long long,string> > zrange(string key,long long start, long long end){
        vector<pair<long long,string> > data;
        bool find=false;

        readEnter();

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

        readExit();

        return data;
    }

};