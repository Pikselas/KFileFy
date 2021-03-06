#include"Ks_Connector.hpp"
int Ks_Connector::OBJCOUNT = 0;
Ks_Connector::Ks_Connector(Ks_Connector::TYPE type)
{
    this->type = type;
    HasActiveConnection = false;
    SERVER_SOCKET = INVALID_SOCKET;
    CLIENT_SOCKET = INVALID_SOCKET;
   if(OBJCOUNT == 0)
   {
        WSADATA wdt;
        WSAStartup(MAKEWORD(2,2),&wdt);
   }         
    if(type == TYPE::SERVER)
    {
        Listen = [this](const char* Port){
            PORT = Port;
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
                
            }
            else
            {
                return;
            }

                                };
        AllowConnection = [this]()
                        {
                                if(CLIENT_SOCKET == INVALID_SOCKET && SERVER_SOCKET != INVALID_SOCKET)
                                {
                                    CLIENT_SOCKET = accept(SERVER_SOCKET,nullptr,nullptr);
                                    if(CLIENT_SOCKET != INVALID_SOCKET)
                                    {
                                        HasActiveConnection = true;
                                    }
                                }            
                        };
        GetClientIp = [this](){
            SOCKADDR_IN client_info = {0};
            int addrsize = sizeof(client_info);
            getpeername(CLIENT_SOCKET,reinterpret_cast<sockaddr*>(&client_info),&addrsize);
            return std::string(inet_ntoa(client_info.sin_addr));
        };
       ListeningOn = [this]()
                    {
                      return PORT;
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
                    freeaddrinfo(res);
                }
            };
    }
}
Ks_Connector::Ks_Connector(TYPE type , std::function<void(Ks_Connector*)> CallAble) :  Ks_Connector(type) 
{
    this->CallAble = CallAble;
}
bool Ks_Connector::IsConnected() const
{
    return HasActiveConnection;
}
bool Ks_Connector::Send(std::string Data)
{
    if(send(CLIENT_SOCKET,Data.c_str(),Data.length(),0) == SOCKET_ERROR)
    {
        closesocket(CLIENT_SOCKET);
        HasActiveConnection = false;
        return false;
    }
    return true;
}
std::optional<std::string> Ks_Connector::Recive() 
{
     const auto Buffer = std::make_unique<char[]>(MAX_DATA_SIZE);
     int Res = recv(CLIENT_SOCKET,Buffer.get(),MAX_DATA_SIZE,0);
    if( Res > 0)
    {
        return std::string(Buffer.get());
    }
    HasActiveConnection = false;
    CloseConnection();
    return {};
}
std::vector<std::string> Ks_Connector::GetDeviceIps() const
{
    std::vector<std::string> TempAddress;
    auto NameBuffer = std::make_unique<char[]>(MAX_HOST_LENGTH); 
    if(gethostname(NameBuffer.get(),MAX_HOST_LENGTH) != SOCKET_ERROR)
    {
        hostent * hosts = gethostbyname(NameBuffer.get());
        if(hosts != nullptr)
        {
            for(int i = 0;hosts->h_addr_list[i] != nullptr;i++)
            {
                in_addr adrs;
                memcpy(&adrs,hosts->h_addr_list[i],sizeof(in_addr));
                TempAddress.emplace_back(inet_ntoa(adrs));
            }
        }
    }
    return TempAddress;
}
void Ks_Connector::CloseConnection()
{
    if(CLIENT_SOCKET != INVALID_SOCKET)
    {
        if(HasActiveConnection)
        {
            shutdown(CLIENT_SOCKET,SD_BOTH);
        }
        closesocket(CLIENT_SOCKET);
        CLIENT_SOCKET = INVALID_SOCKET;
        HasActiveConnection = false;
    }
}
void Ks_Connector::ShutDown()
{
    CloseConnection();
    PORT.clear();
    if(SERVER_SOCKET != INVALID_SOCKET)
    {
        closesocket(SERVER_SOCKET);
        SERVER_SOCKET = INVALID_SOCKET;
    }
}
Ks_Connector::~Ks_Connector()
{
    if(CallAble != nullptr)
    {
        CallAble(this);
    }
    ShutDown();
    if(OBJCOUNT)
    {
       OBJCOUNT--; 
    }
    else
    {
        WSACleanup();
    }
}