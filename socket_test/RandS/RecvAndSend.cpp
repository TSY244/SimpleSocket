//
// Created by 12414 on 2023/1/17.
//

#include "RecvAndSend.hpp"

RecvAndSend::RecvAndSend(void*arg)
{
     client_inf=(ParameterTransfer*)arg;
}
bool RecvAndSend::safeRecive(int fd,char * buf,::uint32_t n,int flags)
{
    int count = 0;
    int i = 0;
    while((i = recv(fd,buf+count,n-count,flags))>0)
    {
        count += i;
        if (count >= n)
        {
            return true;
        }
    }
    return false;
}
bool RecvAndSend::Recv( map<string,string> &Map)
{
    cout<<"Recv"<<endl;

    if(!safeRecive(client_inf->client,reinterpret_cast<char *>(&MagicNumber),sizeof(uint32_t),0))
    {
        Error("recv")
        return false;
    }
    if(!safeRecive(client_inf->client,reinterpret_cast<char *>(&Size),sizeof(uint32_t),0))
    {
        Error("recv")
        return false;
    }
    if(!safeRecive(client_inf->client,reinterpret_cast<char *>(&Type),sizeof(uint32_t),0))
    {
        Error("recv")
        return false;
    }
    if(!safeRecive(client_inf->client,reinterpret_cast<char *>(&Padding),sizeof(uint32_t),0))
    {
        Error("recv")
        return false;
    }
    cout<<"Type:"<<Type<<endl;
    if(Type==0)
    {
        Type=3;
        if(!Put(Map))
        {
            cout<<__LINE__<<endl;
            Error("Put key value");
            Send(false);
            return true;
        }
        cout<<__LINE__<<endl;
        Send(true);
    }
    else if(Type==1)
    {
        Type=4;
        if(!Delete(Map))
        {
            Error("Delete key and value")
            Send(false);
            return true;
        }
        Send(true);
    }
    else if(Type==2)
    {
        Type=5;
        Send(Get(Map));
    }
    return true;
}
bool RecvAndSend::Put( map<string,string> &Map)
{
    cout<<"PUT"<<endl;
    pair<map<string,string>::iterator ,bool> P_insert;
    if(!safeRecive(client_inf->client,reinterpret_cast<char *>(&keysize),sizeof(uint32_t),0))
    {
        Error("recv")
       return false;
    }
    key.resize(keysize);
    if(!safeRecive(client_inf->client,const_cast<char*>(key.data()),keysize,0))
    {
        Error("recv")
        return false;
    }

    if(!safeRecive(client_inf->client,reinterpret_cast<char *>(&valuesize),sizeof(uint32_t),0))
    {
        Error("recv")
        return false;
    }
    value.resize(valuesize);
    if(!safeRecive(client_inf->client,const_cast<char*>(value.data()),valuesize,0))
    {
        Error("recv")
        return false;
    }
    mutex_map.lock();
    P_insert=Map.insert(pair<string,string>(key, value));
    cout<<__LINE__<<endl;
    cout<<key<<endl;
    cout<<value<<endl;
    if(!P_insert.second)
    {
        Error("insert")
        mutex_map.unlock();
        return false;
    }
    cout << Map.at(key) << endl;
    mutex_map.unlock();
    return true;
}
void RecvAndSend::Send(bool sendbool)
{
    int len{0};
    uint32_t sendsize=sizeof (bool);
    len=send(client_inf->client, &MagicNumber, sizeof(::uint32_t), 0);
    if (len < 0)
    {
        Error("send");
        return ;
    } else if (len == 0)
    {
        cout << "exit communication" << endl;
        return ;
    }

    len= send(client_inf->client, &sendsize, sizeof(::uint32_t), 0);
    if (len < 0)
    {
        Error("send");
        return ;
    } else if (len == 0)
    {
        cout << "exit communication" << endl;
        return ;
    }

    len= send(client_inf->client, &Type, sizeof(::uint32_t), 0);
    if (len < 0)
    {
        Error("send");
        return ;
    } else if (len == 0)
    {
        cout << "exit communication" << endl;
        return;
    }

    len= send(client_inf->client, &Padding, sizeof(::uint32_t), 0);
    if (len < 0)
    {
        Error("send");
        return ;
    } else if (len == 0)
    {
        cout << "exit communication" << endl;
        return ;
    }

    len= send(client_inf->client, &sendbool, sizeof(bool), 0);
    if (len < 0)
    {
        Error("send");
        return ;
    } else if (len == 0)
    {
        cout << "exit communication" << endl;
        return ;
    }
}
bool RecvAndSend::Delete(map<string, string> &Map)
{
    if(Map.empty())
        return false;
    cout<<"Delete"<<endl;
    if(!safeRecive(client_inf->client,reinterpret_cast<char *>(&keysize),sizeof(uint32_t),0))
    {
        Error("recv")
        return false;
    }
    key.resize(keysize);
    cout<<__LINE__<<endl;
    if(!safeRecive(client_inf->client,const_cast<char*>(key.data()),keysize,0))
    {
        Error("recv")
        return false;
    }
    try {
        cout<<__LINE__<<endl;
        mutex_map.lock();
        Map.at(key);
        mutex_map.unlock();
        cout<<__LINE__<<endl;

    }
    catch(...)
    {
        Error("erase")
        mutex_map.unlock();
        return false;
    }
    Map.erase(key);
    cout<<"End Delete"<<endl;
    return true;
}
string RecvAndSend::Get(map<string, string> &Map)
{
    cout<<"Get"<<endl;
    string tem;
    if(Map.empty())
        return "NULL";
    if(recv(client_inf->client,&keysize,sizeof(uint32_t),0)<0)
    {
        Error("recv")
        return "NULL";
    }
    key.resize(keysize);
    if(recv(client_inf->client,const_cast<char*>(key.data()),keysize,0)<0)
    {
        Error("recv")
        return "NULL";
    }
    try
    {
        cout<<__LINE__<<endl;
        mutex_map.lock();
        cout<<__LINE__<<endl;
        tem=Map.at(key);
        cout<<__LINE__<<endl;
        mutex_map.unlock();
        cout<<"End Get"<<endl;
        return tem;
    }
    catch(...)
    {
        Error("Get")
        mutex_map.unlock();
        return "NULL";
    }
}
void RecvAndSend::Send(string SendString)
{
    cout<<"Send"<<endl;
    int len{0};
    cout<<__LINE__<<endl;
    cout<<SendString<<endl;
    uint32_t sendsize=SendString.length()+sizeof (::uint32_t);
    len=send(client_inf->client, &MagicNumber, sizeof(::uint32_t), 0);
    if (len < 0)
    {
        Error("send");
        return ;
    } else if (len == 0)
    {
        cout << "exit communication" << endl;
        return ;
    }

    len= send(client_inf->client, &sendsize, sizeof(::uint32_t), 0);
    if (len < 0)
    {
        Error("send");
        return ;
    } else if (len == 0)
    {
        cout << "exit communication" << endl;
        return ;
    }

    len= send(client_inf->client, &Type, sizeof(::uint32_t), 0);
    if (len < 0)
    {
        Error("send");
        return ;
    } else if (len == 0)
    {
        cout << "exit communication" << endl;
        return;
    }

    len= send(client_inf->client, &Padding, sizeof(::uint32_t), 0);
    if (len < 0)
    {
        Error("send");
        return ;
    } else if (len == 0)
    {
        cout << "exit communication" << endl;
        return ;
    }
    sendsize=SendString.length();
    len= send(client_inf->client, &sendsize, sizeof(::uint32_t), 0);
    if (len < 0)
    {
        Error("send");
        return ;
    } else if (len == 0)
    {
        cout << "exit communication" << endl;
        return ;
    }


    len= send(client_inf->client, const_cast<char *>(SendString.data()), sendsize, 0);
    if (len < 0)
    {
        Error("send");
        return ;
    } else if (len == 0)
    {
        cout << "exit communication" << endl;
        return ;
    }
    return ;
}
