//
// Created by 12414 on 2023/1/17.
//

#ifndef SOCKET_TEST_RECVANDSEND_HPP
#define SOCKET_TEST_RECVANDSEND_HPP

#include "iostream"
#include"unistd.h"
#include "arpa/inet.h"
#include "cstring"
#include "thread"
#include "string"
#include "queue"
#include "mutex"
#include "map"
#include "fstream"
using namespace std ;
#define Error(error) cout<<"[ "<<__LINE__<<" ]  "<<error<<" failed "<<endl;

//用于working函数的参数传递
struct ParameterTransfer
{
    int client;
    sockaddr_in addr;
};


class RecvAndSend
{
private:
    uint32_t MagicNumber;
    uint32_t Size;
    uint32_t Type;
    uint32_t Padding;
    ParameterTransfer* client_inf;
    uint32_t keysize;
    string key;
    uint32_t valuesize;
    string value;
    mutex mutex_map;
private:
    bool Put( map<string,string> &Map);
    void Send(bool sendbool);
    void Send(string SendString);
    bool Delete(map<string,string> &Map);
    string Get(map<string,string> &Map);
    bool safeRecive(int fd,char * buf,::uint32_t n,int flags);
public:
    RecvAndSend(void*arg);
    bool Recv( map<string,string> &Map);
};


#endif //SOCKET_TEST_RECVANDSEND_HPP
