#include"Ks_Connector.hpp"
#include"Ks_Connector.cpp"
#include<iostream>
int main()
{
    Ks_Connector kc(Ks_Connector::TYPE::CLIENT);
    kc.Connect("127.0.0.1","2705");
    if(kc.IsConnected())
    {
        std::cout<<kc.Recive().value();
        kc.Send("Nope");
        std::cout<<kc.Recive().value();
        kc.ShutDown();
    }
    return 0;
}