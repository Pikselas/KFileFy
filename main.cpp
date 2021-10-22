#include"Ks_Connector.hpp"
#include"Ks_Connector.cpp"
#include<iostream>
int main()
{
    Ks_Connector kc(Ks_Connector::TYPE::SERVER);
    kc.Listen("2705");
    if(kc.IsConnected())
    {
        std::cout<<"Got Connection";
        if(kc.Send("Hello world"))
        {
            std::cout<<"Sent";
        }
    }
    return 0;
}