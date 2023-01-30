// Created by 12414 on 2023/1/6.
//

#ifndef SOCKET_TEST_SOCKET_HPP
#define SOCKET_TEST_SOCKET_HPP


#include "../RandS/recvAndSend.hpp"
using namespace std;

#define PORT  8989



//server
class serverClass
{
public:
    //functions
    serverClass();
    ~serverClass();
    [[noreturn]] void manager();
    void saveDate();
private:
    void working();
    static bool importDate();
private:

    int serverFd;
    queue<parameterTransfer> clientIfo;  //存放客户端的信息
    mutex mutexServer;
    mutex saveLock;

    void accepted();
};


#endif //SOCKET_TEST_SOCKET_HPP
