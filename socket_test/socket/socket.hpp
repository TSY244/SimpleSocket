// Created by 12414 on 2023/1/6.
//

#ifndef SOCKET_TEST_SOCKET_HPP
#define SOCKET_TEST_SOCKET_HPP


#include "../RandS/RecvAndSend.hpp"
using namespace std;

#define PORT  8989
#define IP " 172.24.147.211"

static map<string,string>Map;

//客户端
int CreateClient();



//子线程的工作函数
int working(void *arg);

//服务端
class ServerClass
{
public:
    //函数区
    //创建服务端
    ServerClass();
    //多并发的实现
    void Manager();
    friend int working(void *arg);
private:
    void SaveDate();
    bool ImportDate();
private:
    int server_fd;
    queue<ParameterTransfer> ClientIfo;//用于存放client的信息


};


#endif //SOCKET_TEST_SOCKET_HPP
