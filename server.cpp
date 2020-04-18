#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <mutex>
#include <bits/stdc++.h>
#include <sys/socket.h>
#include <unistd.h>
#include "constants.hpp"
#include "Redis.hpp"
using namespace std;
int maxPathLen = 10;
int maxHttpLen = 600;
int maxClient = 20000;
int serverPort = 8080;
struct timeval keepAliveTimeout({300, 0});

Redis global;

bool isValidInterger(string num){
    if(num.size()==0 || num.size()>18+(num[0]=='-'))
        return false;
    bool flag=true;
    for(auto &dig:num){
        if(flag && dig=='-')
            continue;
        if(!(dig>='0'  && dig<='9')){
            return false;
        }
        flag = false;
    }
    return true;
}

class Server{

private:
    struct sockaddr_in address;
    int server_fd;

public:
    static void getResponse(vector<string> &,string &);
    friend bool isValidInterger(string );
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

    static void __sendResponse(int clientSocket, string s) {
        char responseStr[(int)(s.size()+1)];
        strcpy(responseStr, s.c_str());
        write(clientSocket, responseStr, strlen(responseStr));
    }

    


    ~Server(){
        close(server_fd);
    }
};

 void Server::getResponse(vector<string> &data,string &res){
    int x;
    if(target.find(data[0])==target.end()){
        x = 0;
    } else {
        x =  target.find(data[0])->second;
    }
    switch (x)
    {
        case 1:
        {
            if(data.size()!=3){
                 res = setStringError;
            } else {
                global.set(data[1],data[2]);
                res = "OK";
            }
            break;
        }
        case 2:
        {   
            if(data.size()!=2){
                 res = getStringError;
            } else {
                res = global.get(data[1]);
            }
            break;
        }
        case 3:
        {
            if(data.size()!=3){
                 res = expireStringError;
            } else {
                if(isValidInterger(data[2])){
                    res = global.expire(data[1], stoll(data[2]));
                } else {
                    res = integerStringError;
                }
            }
            break;
        }
        case 4:
        {
            if(data.size()%2 || data.size()<4){
                res = zaddStringError;
            } else {
                int flag=1;
                vector<pair<string,long long>> newVal;
                for(int i=2;i<data.size();i+=2){
                    if(!isValidInterger(data[i])){
                        flag = 0;
                        break;
                    }

                    newVal.push_back(make_pair(data[i+1],stoll(data[i])));
                }
                if(flag){
                    res = global.zadd(data[1], newVal);
                } else {
                    res = integerStringError;
                }
            }
            break;
        }
        case 5:
        {
            if(data.size()!=3){
                 res = zrankStringError;
            } else {
                res = global.zrank(data[1],data[2]);
            }
            break;
        }
        case 6:
        {
            if(data.size()!=4){
                 res = zrangeStringError;
            } else {
                if(isValidInterger(data[2]) && isValidInterger(data[3])){
                    res = global.zrange(data[1], stoll(data[2]), stoll(data[3]));
                } else {
                    res = integerStringError;
                }
            }
            break;
        }
        default:
        {
            res = unkownStringError;
        }
    }
}
int main(int argc, char const *argv[]){

    Server server(serverPort);

    server.acceptConnections();


} 