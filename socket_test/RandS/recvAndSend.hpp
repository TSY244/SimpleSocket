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
#include <string>

using namespace std ;
#define ERROR(error) cout<<"[ "<<__LINE__<<" ]  "<<error<<" failed "<<endl;

struct parameterTransfer
{
    int client;
    sockaddr_in addr;
};


class recvAndSend
{
private:
    parameterTransfer* clientInf;
//    mutex ReadMutexMap{};
private:
    bool putData();
    void sendData(bool sendbool, uint32_t type);
    void sendData(const string sendString);
    bool deleteData();
    string getDate();
    static bool safeRecive(int fd, char *buf, ::uint32_t n, int flags);
    static bool safeSend(int fd, char *buf, ::uint32_t n, int flags);
public:
    explicit recvAndSend(void*arg);
    bool recvInf();
};

extern  mutex WriteMutexMap;
#endif //SOCKET_TEST_RECVANDSEND_HPP
