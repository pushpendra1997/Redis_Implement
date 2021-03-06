#include <bits/stdc++.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
using namespace std;
#define PORT 8080 
   
int main(int argc, char const *argv[]) 
{ 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char hello[1024] = {0}; 
    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        cout<<"\n Socket creation error \n"; 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) { 
        cout<<"\nInvalid address/ Address not supported \n"; 
        return -1; 
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
        cout<<"\nConnection Failed \n"; 
        return -1; 
    }
    while(true) {
        cout<<"redis > ";
        string hello;
        getline(cin,hello);
        if(hello.size())
        {
            send(sock , hello.c_str(), hello.size(), 0);  
            valread = read( sock , buffer, 1024); 
            cout<<buffer<<endl;
        }
    }
    return 0; 
} 