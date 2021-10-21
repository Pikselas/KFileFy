#include"Ks_Connector.hpp"
int Ks_Connector::OBJCOUNT = 0;
Ks_Connector::Ks_Connector(Ks_Connector::TYPE type)
{
   if(OBJCOUNT == 0)
   {
        WSADATA wdt;
        WSAStartup(MAKEWORD(2,2),&wdt);
   }
    if(type == TYPE::SERVER)
    {
        listen = [&](const char * Port){
                        
                            }; 
    }
}
Ks_Connector::~Ks_Connector()
{
    if(!OBJCOUNT)
    {
       OBJCOUNT--; 
    }
    else
    {
        WSACleanup();
    }
}