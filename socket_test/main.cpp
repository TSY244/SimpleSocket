
#include "socket/socket.hpp"
#include "threadpool/ThreadPool.hpp"


int main()
{
    ServerClass server;
    ThreadPool pool(6);
    server.Manager();
    auto tem=bind(&ServerClass::Manager,&server);
    pool.SubmitTask(tem);
    return 0;
}