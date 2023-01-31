//
// Created by 12414 on 2023/1/17.
//

#include "recvAndSend.hpp"
extern std::map<std::string,std::string>Map;
recvAndSend::recvAndSend(void*arg)
{
    clientInf=static_cast<parameterTransfer*>(arg);
}
bool recvAndSend::safeRecive(int fd,char *buf, uint32_t n, int flags)
{
    /*
     * description: 更安全的recv
     * return：判断成功
     */
    uint32_t count = 0;
    uint32_t i ;
    cout<<"safeRecive"<<endl;
    while((i = recv(fd, buf + count, n - count, flags)) > 0)
    {
        count += i;
        if (count >= n)
        {
            return true;
        }
    }
    return false;
}
bool recvAndSend::recvInf()
{
    /*
     * description: 接受头部
     */
    cout<<"recvInf"<<endl;
    if(!safeRecive(clientInf->client, reinterpret_cast<char *>(&magicNumber), sizeof(uint32_t), 0))
    {
        /*ERROR("recvInf")*/
        return false;
    }
    if(!safeRecive(clientInf->client, reinterpret_cast<char *>(&size), sizeof(uint32_t), 0))
    {
        ERROR("recvInf")
        return false;
    }
    if(!safeRecive(clientInf->client, reinterpret_cast<char *>(&type), sizeof(uint32_t), 0))
    {
        ERROR("recvInf")
        return false;
    }
    if(!safeRecive(clientInf->client, reinterpret_cast<char *>(&padding), sizeof(uint32_t), 0))
    {
        ERROR("recvInf")
        return false;
    }
    cout<<"type: "<<type<<endl;
    if(type == 0)
    {
        type=3;
        if(!putData())
        {
            ERROR("putData key value")
            sendData(false);
            return true;
        }
        sendData(true);
    }
    else if(type == 1)
    {
        type=4;
        if(!deleteData())
        {
            ERROR("deleteData key and value")
            sendData(false);
            return true;
        }
        sendData(true);
    }
    else if(type == 2)
    {
        type=5;
        cout<<"get begin"<<endl;
        sendData(getDate());

    }
    return true;
}
bool recvAndSend::putData()
{
    /*
     * description: 接受客户端的数据存入map
     * return: 是否成功
     */
    cout<<"putData"<<endl;
    pair<map<string,string>::iterator ,bool> insertDate;
    if(!safeRecive(clientInf->client, reinterpret_cast<char *>(&keysize), sizeof(uint32_t), 0))
    {
        ERROR("recvInf")
       return false;
    }
    key.resize(keysize);
    if(!safeRecive(clientInf->client, const_cast<char*>(key.data()), keysize, 0))
    {
        ERROR("recvInf")
        return false;
    }

    if(!safeRecive(clientInf->client, reinterpret_cast<char *>(&valuesize), sizeof(uint32_t), 0))
    {
        ERROR("recvInf")
        return false;
    }
    value.resize(valuesize);
    if(!safeRecive(clientInf->client, const_cast<char*>(value.data()), valuesize, 0))
    {
        ERROR("recvInf")
        return false;
    }
    try {
        WriteMutexMap.lock();
        Map.at(key)=value;
        WriteMutexMap.unlock();
        cout<<"putData end"<<endl;
        return true;
    }
    catch(...)
    {
//        insertDate=Map.insert(pair<string,string>(key, value));
//        if(!insertDate.second)
//        {
//            ERROR("insert")
//            WriteMutexMap.unlock();
//            return false;
//        }
        //insert效率较低？改用C++ 11新增的emplace
        Map.emplace(key,value);
        WriteMutexMap.unlock();
        cout<<"putData end"<<endl;
        return true;
    }


}
void recvAndSend::sendData(bool sendbool)
{
    /*
     * description: respose body
     */
    cout<<"sendData"<<endl;
    uint32_t sendsize=sizeof (bool);
    if(!safeSend(clientInf->client,reinterpret_cast< char *>(&magicNumber), sizeof(::uint32_t), 0))
        return;
    if(!safeSend(clientInf->client, reinterpret_cast< char *>(&sendsize), sizeof(::uint32_t), 0))
        return;
    if(!safeSend(clientInf->client, reinterpret_cast<char *>(&type), sizeof(::uint32_t), 0))
        return;
    if(!safeSend(clientInf->client, reinterpret_cast<char *>(&padding), sizeof(::uint32_t), 0))
        return;
    usleep(8000);
    if(!safeSend(clientInf->client, reinterpret_cast<char *>(&sendbool), sizeof(bool), 0))
        return;
    cout<<"sendData end"<<endl;
}
bool recvAndSend::deleteData()
{
    /*
     * description: 删除map里面的数据
     * return: 是否成功
     */
    cout<<"deleteData"<<endl;
    if(!safeRecive(clientInf->client, reinterpret_cast<char *>(&keysize), sizeof(uint32_t), 0))
    {
        ERROR("recvInf")
        return false;
    }
    key.resize(keysize);
    if(!safeRecive(clientInf->client, const_cast<char*>(key.data()), keysize, 0))
    {
        ERROR("recvInf")
        return false;
    }
    try {
        WriteMutexMap.lock();
//        Map.at(key);
        Map.erase(key);
        WriteMutexMap.unlock();
        cout<<"End deleteData"<<endl;
        return true;
    }
    catch(...)
    {
        ERROR("erase")
        WriteMutexMap.unlock();
        return false;
    }
}
string recvAndSend::getDate()
{
    /*
     * description: get数据库里面的数据
     * return: 返回客户端get的数据，没有返回NULl
     */
    cout<<"getDate"<<endl;
    string tem;
    if(!safeRecive(clientInf->client, reinterpret_cast<char *>(&keysize), sizeof(uint32_t), 0))
    {
        ERROR("recvInf")
        return "NULL";
    }
    key.resize(keysize);
    if(!safeRecive(clientInf->client, const_cast<char*>(key.data()), keysize, 0))
    {
        ERROR("recvInf")
        return "NULL";
    }
    try
    {
        ReadMutexMap.lock();
        tem=Map.at(key);
        ReadMutexMap.unlock();
        cout<<"End getDate"<<endl;
        return tem;
    }
    catch(...)
    {
        ReadMutexMap.unlock();
        ERROR("getDate")
        return "NULL";
    }
}
void recvAndSend::sendData(const string& sendString)
{
    /*
     * description: 发送get的数据
     */
    cout<<"sendData"<<endl;
    uint32_t sendsize= sendString.length() + sizeof (::uint32_t);
    if(!safeSend(clientInf->client, reinterpret_cast<char*>(&magicNumber), sizeof(::uint32_t), 0))
        return;
    if(!safeSend(clientInf->client, reinterpret_cast<char*>(&sendsize), sizeof(::uint32_t), 0))
        return;
    if(!safeSend(clientInf->client, reinterpret_cast<char*>(&type), sizeof(::uint32_t), 0) )
        return;
    if(!safeSend(clientInf->client,reinterpret_cast<char*>(&padding), sizeof(::uint32_t), 0) )
        return;
    sendsize=sendString.length();
    if(!safeSend(clientInf->client, reinterpret_cast<char*>(&sendsize), sizeof(::uint32_t), 0) )
        return;
    usleep(5000);
    if(!safeSend(clientInf->client, const_cast<char *>(sendString.data()), sendsize, 0) <= 0)
        return;
    cout<<"sendData end"<<endl;
}
bool recvAndSend::safeSend(int fd, char *buf, ::uint32_t n, int flags)
{
    ::uint32_t count=0; //已发送的数据字节数
    ::uint32_t i;
    while((i= send(fd,buf+count,n-count,flags))>0)
    // 这里一定要记得让buf产生偏移
    {
        count+=i;
        if(count>=i)
        {
            return true;
        }
    }
    return false;
}
