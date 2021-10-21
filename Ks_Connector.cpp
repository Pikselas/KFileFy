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