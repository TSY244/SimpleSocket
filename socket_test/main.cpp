
#include "socket/socket.hpp"
#include "csignal"

void catchSignal(int signum);

serverClass* global= nullptr;

int main()
{
    serverClass server;
    global=&server;
    signal(SIGTERM,catchSignal);
    server.manager();
    return 0;
}


void catchSignal(int signum)
{
    global->saveDate();
    exit(signum);
}