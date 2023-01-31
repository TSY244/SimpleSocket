
#include "socket/socket.hpp"
#include "csignal"

void catchSignal1(int signum);
void catchSignal2(int signum);
serverClass* global= nullptr;

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
void catchSignal1(int signum)
{
    global->saveData();
    exit(signum);
}