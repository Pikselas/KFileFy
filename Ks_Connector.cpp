#include"Ks_Connector.hpp"
int Ks_Connector::OBJCOUNT = 0;
Ks_Connector::Ks_Connector(Ks_Connector::TYPE type) : type(type) ,HasActiveConnection(false), SERVER_SOCKET(INVALID_SOCKET) , CLIENT_SOCKET(INVALID_SOCKET)
{
   if(OBJCOUNT == 0)
   {
        WSADATA wdt;
        WSAStartup(MAKEWORD(2,2),&wdt);
   }         
    if(type == TYPE::SERVER)
    {
        Listen = [this](const char* Port){
            addrinfo hint,*res;
            hint = {0};
            hint.ai_family = AF_INET;
            hint.ai_protocol = IPPROTO_TCP;
            hint.ai_socktype = SOCK_STREAM;
            hint.ai_flags = AI_PASSIVE;
            getaddrinfo(nullptr,Port,&hint,&res);
            SERVER_SOCKET = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
            if(SERVER_SOCKET != INVALID_SOCKET)
            {
                bind(SERVER_SOCKET,res->ai_addr,res->ai_addrlen);
            }
            else
            {
                freeaddrinfo(res);
                return;
            }
            freeaddrinfo(res);
            if(listen(SERVER_SOCKET,SOMAXCONN) != SOCKET_ERROR)
            {
                CLIENT_SOCKET = accept(SERVER_SOCKET,nullptr,nullptr);
                if(CLIENT_SOCKET != INVALID_SOCKET)
                {
                    HasActiveConnection = true;
                }
            }
            else
            {
                return;
            }

                                };
    }
    else
    {
        Connect = [this](const char* Address,const char* Port){
                addrinfo hint,*res;
                hint = {0};
                hint.ai_family = AF_UNSPEC;
                hint.ai_protocol = IPPROTO_TCP;
                hint.ai_socktype = SOCK_STREAM;
                if(getaddrinfo(Address,Port,&hint,&res) != 0)
                {
                    return;
                }
                else
                {
                    for(auto AddrInfo = res;AddrInfo != nullptr;AddrInfo = AddrInfo->ai_next)
                    {
                        CLIENT_SOCKET = socket(AddrInfo->ai_family,AddrInfo->ai_socktype,AddrInfo->ai_protocol);
                        if(CLIENT_SOCKET == INVALID_SOCKET)
                        {
                            return;
                        }
                        if(connect(CLIENT_SOCKET,AddrInfo->ai_addr,AddrInfo->ai_addrlen) == SOCKET_ERROR)
                        {
                            closesocket(CLIENT_SOCKET);
                            CLIENT_SOCKET = INVALID_SOCKET;
                            return;
                        }
                        else
                        {
                            HasActiveConnection = true;
                            break;
                        }
                    }
                }
            };
    }
}
bool Ks_Connector::IsConnected() const
{
    return HasActiveConnection;
}
bool Ks_Connector::Send(std::string Data) const
{
    if(send(CLIENT_SOCKET,Data.c_str(),Data.length(),0) == SOCKET_ERROR)
    {
        return false;
    }
    return true;
}
std::optional<std::string> Ks_Connector::Recive(size_t size) const
{
     const auto Buffer = std::make_unique<char[]>(size);
    if(recv(CLIENT_SOCKET,Buffer.get(),size,0) > 0)
    {
        return std::string(Buffer.get());
    }
    return {};
}
Ks_Connector::~Ks_Connector()
{
    if(OBJCOUNT)
    {
       OBJCOUNT--; 
    }
    else
    {
        WSACleanup();
    }
}