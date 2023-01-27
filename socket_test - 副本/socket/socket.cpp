//
// Created by 12414 on 2023/1/6.
//

#include "socket.hpp"


//创建主线程
ServerClass::ServerClass()
{

    //创建socket
    server_fd= socket(AF_INET,SOCK_STREAM,0);
    if (server_fd == -1) {
        Error("socket");
        exit(-1);
    }

    //初始化addr
    sockaddr_in addr;
    addr.sin_addr.s_addr=INADDR_ANY;  // 走动绑定IP地址
    addr.sin_port= htons(PORT);
    addr.sin_family=AF_INET;

    //打开端口复用功能
    int option=1;
    if(setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,(const char*)(& option),sizeof(option)))
    {
        Error("setsockopt");
        close(server_fd);
        exit(-1);
    }


    //bind
    int ret=bind(server_fd,(sockaddr*)&addr,sizeof(addr));
    if (ret == -1) {
        Error("bind");
        close(server_fd);
        exit(-1);
    }

    //监听
    ret=listen(server_fd,10);
    if (ret == -1) {
        Error("listen");
        close(server_fd);
        exit(-1);
    }


}

void ServerClass::Manager()
{
    cout<<"wait connection........"<<endl;
    ParameterTransfer tem;
    int addr_len=sizeof(tem.addr);
    if(!ImportDate())
    {
        Error("ImportDate")
        return;
    }
    while(1)
    {
        tem.client=accept(server_fd, (sockaddr*) &tem.addr, (socklen_t *)(&addr_len));
        if(tem.client ==-1)
        {
            Error("accept");
            break;
        }
        ClientIfo.push(tem);
        thread tid(working,&ClientIfo.front());
        tid.detach();
        ClientIfo.pop();
        cout<<"queue size:"<<ClientIfo.size()<<endl;
        usleep(100);
        if(ClientIfo.empty())
        {
            SaveDate();
        }
    }
    close(server_fd);
}

int working(void *arg)
{
    cout<<"Thread "<<this_thread::get_id()<<" is working, now"<<endl;
    ParameterTransfer* client_inf=(ParameterTransfer*)arg;
    char ip[32]={0};
    cout<<"be connected successfully!    "<<endl<<
        "IP："<<inet_ntop(AF_INET,&client_inf->addr.sin_addr.s_addr,ip,sizeof(ip))<<endl<<
        "PORT："<<ntohs(client_inf->addr.sin_port)<<endl;
    RecvAndSend RAS(client_inf);
    while(true)
    {
        if(!RAS.Recv(Map))
        {
            Error("Recv");
            break;
        }
    }
    cout<<"Quit connect"<<endl;
    close(client_inf->client);
    cout<<"Thread "<<this_thread::get_id()<<" end working"<<endl;

}

void ServerClass::SaveDate()
{

    cout<<"SaveDate"<<endl;
    fstream file;
    file.open("date.txt",ios::out);
    if(!file.is_open())
    {
        Error("open")
        return;
    }
    for (auto iter=Map.begin(); iter!=Map.end() ;iter++)
    {
        cout<<iter->first<<endl;
        file<<iter->first<<endl;
        cout<<iter->second<<endl;
        file<<iter->second<<endl;
        cout<<"save"<<endl;
    }
    cout<<"SaveDate  End"<<endl;
    file.close();
}

bool ServerClass::ImportDate()
{
    fstream  file;
    string TemKey,TemValue;
    int size=0;
    char tem_c[BUFSIZ];
    file.open("date.txt", ios::in);
    if(!file.is_open())
    {
        Error("file.open")
        return false;
    }
    while(!file.eof())
    {
        file.getline(tem_c,BUFSIZ);
        for (; tem_c[size]!='\0'; size++)
        {}
        TemKey.resize(size);
        for (int i = 0; i<=size; ++i)
        {
            TemKey[i]=tem_c[i];
        }
        file.getline(tem_c,BUFSIZ);
        for (size=0; tem_c[size]!='\0'; size++)
        {}
        TemValue.resize(size);
        for (int i = 0; i<=size; ++i)
        {
            TemValue[i]=tem_c[i];
        }
        Map.insert(pair<string,string>(TemKey,TemValue));
    }
    cout<<"IMPORT END"<<endl;
    file.close();
    return true;
}


int CreateClient()
{
    //创建socket
    int fd= socket(AF_INET,SOCK_STREAM,0);
    if (fd == -1) {
        Error("socket");
        return -1;
    }

    //初始化addr
    sockaddr_in addr;
    inet_pton(AF_INET,IP,&addr.sin_addr.s_addr);
    addr.sin_port= htons(PORT);
    addr.sin_family=AF_INET;

    //connect
    int ret= connect(fd,(sockaddr*)&addr,sizeof(addr));
    if(ret == -1)
    {
        Error("connect");
        close(fd);
        return -1;
    }

    return fd;
}
