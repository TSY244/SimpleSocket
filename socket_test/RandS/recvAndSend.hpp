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
    uint32_t magicNumber{};
    uint32_t size{};
    uint32_t type{};
    uint32_t padding{};
    parameterTransfer* clientInf;
    uint32_t keysize{};
    string key;
    uint32_t valuesize{};
    string value;
    mutex mutexMap;
private:
    bool putData();
    void sendData(bool sendbool);
    void sendData(const string& sendString);
    bool deleteData();
    string getDate();
    static bool safeRecive(int fd, char *buf, ::uint32_t n, int flags);
    static bool safeSend(int fd, char *buf, ::uint32_t n, int flags);
public:
    explicit recvAndSend(void*arg);
    bool recvInf();
};


#endif //SOCKET_TEST_RECVANDSEND_HPP
