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
     Ks_Connector(TYPE type , std::function<void(Ks_Connector*)> CallAble);
     void operator=(const Ks_Connector&) = delete;
     Ks_Connector(const Ks_Connector&) = delete;
     ~Ks_Connector();
    private:
     static int OBJCOUNT;
    private:
     TYPE type;
    private:
     bool HasActiveConnection;
     size_t MAX_DATA_SIZE = 32768;
     size_t MAX_HOST_LENGTH = 80;
     std::string PORT;
    private:
     SOCKET SERVER_SOCKET;
     SOCKET CLIENT_SOCKET;
    private:
     std::function<void(Ks_Connector*)> CallAble = nullptr;
    public:
     std::function<void(const char*)> Listen = nullptr;
     std::function<void(const char*,const char*)> Connect = nullptr;
     std::function<std::string()> GetClientIp = nullptr;
     std::function<void()> AllowConnection = nullptr;
     std::function<std::string()> ListeningOn = nullptr; 
    public:
     bool IsConnected() const;
     bool Send(std::string);
     std::optional<std::string> Recive();
     void CloseConnection();
     void ShutDown();
    public:
     std::vector<std::string> GetDeviceIps() const;
};