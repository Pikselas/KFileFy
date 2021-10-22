#pragma once
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<functional>
#include<optional>
#include<memory>
#include<string>
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
     TYPE type;
    private:
     bool HasActiveConnection;
    private:
     SOCKET SERVER_SOCKET;
     SOCKET CLIENT_SOCKET;
    public:
     std::function<void(const char*)> Listen = nullptr;
     std::function<void(const char*,const char*)> Connect = nullptr;
     std::function<void()> Shutdown = nullptr;
    public:
     bool IsConnected() const;
     bool Send(std::string) const;
     std::optional<std::string> Recive(size_t) const;
};