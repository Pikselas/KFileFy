#include"Ks_Connector.hpp"
#include"Ks_Connector.cpp"
#include<iostream>
int main()
{
    Ks_Connector kc(Ks_Connector::TYPE::CLIENT);
    kc.Connect("192.168.0.101","2705");
    if(kc.IsConnected())
    {
        std::cout<<"Connected";
        auto Dt = kc.Recive(100);
        if(Dt)
        {
            std::cout<<Dt.value();
        }
    }
    return 0;
}