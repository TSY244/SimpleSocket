//
// Created by 12414 on 2023/1/6.
//

#include "socket.hpp"
map<string,string>Map;

//创建主线程
serverClass::serverClass()
{

    //creat socket
    serverFd= socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd == -1) {
        ERROR("socket")
        exit(-1);
    }

    //addr
    sockaddr_in addr{};
    addr.sin_addr.s_addr=INADDR_ANY;
    addr.sin_port= htons(PORT);
    addr.sin_family=AF_INET;

    //port reuse
    int option=1;
    if(setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, (const char*)(&option), sizeof(option)))
    {
        ERROR("setsockopt")
        close(serverFd);
        exit(-1);
    }


    //bind
    int ret=::bind(serverFd, (sockaddr*)&addr, sizeof(addr));
    if (ret == -1)
    {
        ERROR("bind")
        close(serverFd);
        exit(-1);
    }

    //listen
    ret=listen(serverFd, 10);
    if (ret == -1) {
        ERROR("listen")
        close(serverFd);
        exit(-1);
    }


}

[[noreturn]] void serverClass::manager()
{
    /*
     * description: 用于存储数据和创建accept线程
     */
    cout<<"wait connection........"<<endl;
    if(!importDate())
    {
        ERROR("importDate")
    }
    vector<thread> tids;
    for (int i = 0; i < 5; ++i) {
        tids.emplace_back(&serverClass::accepted, this);
    }
    while(true)
    {
        if(saveLock.try_lock()) {
            saveDate();
            saveLock.unlock();
        }
        usleep(10000000);
    }
}
void serverClass::accepted()
{
    /*
     * description: 调用accept并将客户端数据存入clientIfo
     */
    parameterTransfer tem{};
    int addrLen=sizeof(tem.addr);
    while(true)
    {
        tem.client=accept(serverFd, (sockaddr*) &tem.addr, (socklen_t *)(&addrLen));
        if(tem.client ==-1)
        {
            ERROR("accept")
            continue;
        }
        mutexServer.lock();
        clientIfo.push(tem);
        mutexServer.unlock();
        thread tid(&serverClass::working, this);
        tid.detach();

    }
}
void serverClass::working()
{
    /*
     * description: 调用accept，将客户端数据存入clientIfo，ras用于数据库操作
     */
    /*cout<<"Thread "<<this_thread::get_id()<<" is working, now"<<endl;*/
    mutexServer.lock();
    parameterTransfer clientInf=clientIfo.front();
    clientIfo.pop();
    mutexServer.unlock();
    char ip[32]={0};
    cout << "be connected successfully!    " << endl <<
         "IP：" << inet_ntop(AF_INET, &clientInf.addr.sin_addr.s_addr, ip, sizeof(ip)) << endl <<
         "PORT：" << ntohs(clientInf.addr.sin_port) << endl;
    recvAndSend ras(&clientInf);
    while(true)
    {
        if(!ras.recvInf())
        {
            /*ERROR("recvInf")*/
            break;
        }
    }
    close(clientInf.client);
    /*cout<<"Thread "<<this_thread::get_id()<<" power cut-off"<<endl;*/
}

void serverClass::saveDate()
{
    /*
     * description: 存储数据
     */
    cout<<"saveDate"<<endl;
    if(Map.empty())
        return;
    fstream file;
    file.open("date.txt",ios::out);
    if(!file.is_open())
    {
        ERROR("open")
        return;
    }
    mutexServer.lock();
    for (auto & iter : Map)
    {
        file<<iter.first<<endl;
        file<<iter.second<<endl;
    }
    mutexServer.unlock();
    file.close();
    cout<<"saveDate end"<<endl;
}

bool serverClass::importDate()
{
    /*
     * description: 从外界导入数据
     */
    cout<<"importDate"<<endl;
    fstream  file;
    string temKey,temValue;
    file.open("date.txt", ios::in);
    if(!file.is_open())
    {
        ERROR("open")
        return false;
    }
    while(!file.eof())
    {
        file >> temKey;
        file >> temValue;
        Map.insert(pair<string,string>(temKey, temValue));
    }
    file.close();
    cout<<"importDate end"<<endl;
    return true;
}

serverClass::~serverClass()
{
    saveDate();
    close(serverFd);
}


