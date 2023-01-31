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
    void saveData();
private:
    void worker(parameterTransfer clientIfo);
    static bool importData();
private:

    int serverFd;
    mutex mutexServer;
    mutex saveLock;

    void accepted();
};


#endif //SOCKET_TEST_SOCKET_HPP
