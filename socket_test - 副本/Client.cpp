//
// Created by 12414 on 2023/1/6.
//

#include "socket/socket.hpp"


int main()
{
    int Client=CreateClient();
    if(Client==-1)
        return -1;
    char Recv[BUFSIZ]={0};
    char Send[BUFSIZ]={0};
    cout<<"be connected successfully! "<<endl;
    while(1)
    {
        cout<<"send messages：";
        cin.getline(Send,BUFSIZ);
        int len=send(Client,Send,BUFSIZ,0);
        if(len<0)
        {
            Error("send");
            break;
        }
        else if(len==0)
        {
            cout<<"exit communication"<<endl;
            break;
        }
        memset(Send,0, BUFSIZ);
        cout<<"receive messages:";
        if(recv(Client,Recv,BUFSIZ,0)<1)
        {
            Error("recv");
            break;
        }
        cout<<Recv<<endl;
        memset(Recv,0,BUFSIZ);
    }

    close(Client);
    return 0;
}

