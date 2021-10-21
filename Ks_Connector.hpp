#pragma once
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<functional>
#include<optional>
#pragma comment (lib, "Ws2_32.lib")

class Ks_Connector
{
    public:
        enum class TYPE
        {
              SERVER,
              CLIENT
        };
    public:
     Ks_Connector(TYPE type);
     ~Ks_Connector();
    private:
     static int OBJCOUNT;
    private:
     SOCKET SERVER_SOCKET;
     SOCKET CLIENT_SOCKET;
    public:
     std::function<void(const char*)> listen = nullptr;
     std::function<void(const char*,const char*)> connect = nullptr;
     std::function<void()> Send = nullptr;
     std::function<std::optional<std::string>()> Recive = nullptr;
};