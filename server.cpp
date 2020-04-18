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
struct timeval keepAliveTimeout({30, 0});

Redis global;

class Server{

private:
    struct sockaddr_in address;
    int server_fd;

public:

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
            map<string,int> target;
            target["SET"] = 1;
            target["GET"] = 2;
            target["EXPIRE"] = 3;
            target["ZADD"] = 4;
            target["ZRANK"] = 5;
            target["ZRANGE"] = 6;
            if(data.size()){
                int x=target[data[0]];
                cout<<x<<endl;
                string res="";
                switch (x)
                {
                    case 1:
                    {
                        cout<<data[1]<<endl;
                        if(data.size()<3){
                             res = "ERR ERR wrong number of arguments for 'set' command";
                        } else if(data.size()>3){
                            res = "ERR ERR syntax error";
                        } else {
                            res = "OK";
                        }
                        break;
                    }
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    case 5:
                        break;
                    case 6:
                        break;
                    default:
                    {}
                }

            }
           
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

int main(int argc, char const *argv[]){

    Server server(serverPort);

    server.acceptConnections();


} 