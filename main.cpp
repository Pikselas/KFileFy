#include<iostream>
#include"Ks_Connector.hpp"
#include"Ks_Connector.cpp"
#include<fstream>
int main()
{
    Ks_Connector kc(Ks_Connector::TYPE::SERVER);
    kc.Listen("700");
    while(true)
    {
        kc.AllowConnection();
        if(kc.IsConnected())
        {
            std::cout<<kc.GetClientIp();
            kc.CloseConnection();
        }
    }
    return 0;
}