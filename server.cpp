#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <mutex>
#include <bits/stdc++.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Redis.hpp"
using namespace std;
int maxPathLen = 10;
int maxHttpLen = 600;
int maxClient = 20000;
int serverPort = 8080;
struct timeval keepAliveTimeout({300, 0});
Redis global;
bool isInterger(string num){
    if(num.size()>18)
        return false;
    for(auto &dig:num){
        if(!(dig>='0'  && dig<='9')){
            return false;
        }
    }
    return true;
}
class Server{

private:
    struct sockaddr_in address;
    int server_fd;

public:
    static void getResponse(vector<string> &,string &);
    friend bool isInterger();
    Server(int port=8000){
        int opt = 1;
        int addrlen = sizeof(address);
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        bind(server_fd, (struct sockaddr *)&address, sizeof(address));
        listen(server_fd, 3);

    }

    void acceptConnections(){
            int addrlen = sizeof(address);
            int clientSocket[maxClient];
            pthread_t thread_id[maxClient];
            cout << "Server accepting connections on localhost: 8000\n";
            for(int i=0; i<maxClient; i++){
                clientSocket[i] = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
                pthread_create(&thread_id[i], NULL, this->__serveClient, &clientSocket[i]);
            }
    }
    
    static void *__serveClient(void *arg){
        int clientSocket = *((int *)arg);

        cout<<clientSocket<<"\n";
        setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO,
                    (struct timeval *)&keepAliveTimeout,
                    sizeof(struct timeval));

        char request[maxHttpLen];
        while(true) {
            int sz = read(clientSocket , request , maxHttpLen);
            request[sz] = '\0';
            stringstream req(request);
            string word;
            vector<string> data;
            while (req >> word)
            {
                data.push_back(word);
            }
            string res="empty";
            if(data.size()){
                getResponse(data, res);
                
            }
            __sendResponse(clientSocket, res);
           
        }
    }

    static void __sendResponse(int clientSocket, string &s) {
        char responseStr[(int)(s.size()+1)];
        strcpy(responseStr, s.c_str());
        write(clientSocket, responseStr, strlen(responseStr));
    }

    


    ~Server(){
        close(server_fd);
    }
};

 void Server::getResponse(vector<string> &data,string &res){
    map<string,int> target;
    target["SET"] = 1;
    target["GET"] = 2;
    target["EXPIRE"] = 3;
    target["ZADD"] = 4;
    target["ZRANK"] = 5;
    target["ZRANGE"] = 6;
    switch (target[data[0]])
    {
        case 1:
        {
            if(data.size()!=3){
                 res = "ERR ERR wrong number of arguments for 'set' command";
            } else {
                global.set(data[1],data[2]);
                res = "OK";
            }
            break;
        }
        case 2:
        {   
            if(data.size()!=2){
                 res = "ERR ERR wrong number of arguments for 'get' command";
            } else {
                res = global.get(data[1]);
            }
            break;
        }
        case 3:
        {
            if(data.size()!=3){
                 res = "ERR ERR wrong number of arguments for 'expire' command";
            } else {
                long val=0,flag=true;
                for(auto &dig:data[2]){
                    if(dig>='0'  && dig<='9'){
                        val = val*10 + dig -'0';
                    } else {
                        flag = false;
                        break;
                    }
                }
                if(flag){
                    res = global.expire(data[1], val);
                } else {
                    res = "ERR ERR value is not an integer or out of range";
                }
            }
            break;
        }
        case 4:
        {
            if(data.size()%2){
                res = "ERR ERR wrong number of arguments for 'set' command";
            } else {
                int flag=1;
                vector<pair<string,long>> newVal;
                for(int i=2;i<data.size();i+=2){
                    if(data[i].size()>18){
                        flag = 0;
                        break;
                    }
                    long val=0;
                    
                    newVal.push_back(make_pair(data[i+1],val));
                }
                if(flag){
                    res = global.zadd(data[1], newVal);
                } else {
                    res = "ERR ERR value is not an integer or out of range";
                }
            }
        }
        case 5:
        {
            if(data.size()!=3){
                 res = "ERR ERR wrong number of arguments for 'set' command";
            } else {
                res = global.zrank(data[1],data[2]);
            }
            break;
        }
        case 6:
        {

            break;
        }
        default:
        {}
    }
}
int main(int argc, char const *argv[]){

    Server server(serverPort);

    server.acceptConnections();


} 