
#include "socket/socket.hpp"
#include "csignal"

void catchSignal1(int signum);
void catchSignal2(int signum);
serverClass* global= nullptr; //接受类的对象的地址，调用saveData
mutex WriteMutexMap{};  //用一个全局的锁Map防止

int main()
{
    serverClass server;
    global=&server;
    signal(SIGTERM,catchSignal1);
    signal(SIGPIPE,catchSignal2);
    server.manager();
    return 0;
}

void catchSignal2(int signum)
{
    cout<<"SIGTERM "<<signum<<endl;
}
//防止使用SIGTERM信号退出不调用saveData
void catchSignal1(int signum)
{
    global->saveData();
    exit(signum);
}