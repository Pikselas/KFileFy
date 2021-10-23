#include"Ks_Connector.hpp"
#include"Ks_Connector.cpp"
#include<iostream>
int main()
{
    Ks_Connector kc(Ks_Connector::TYPE::SERVER);
    kc.Listen("2705");
    if(kc.IsConnected())
    {
       kc.Send("hello world");
       std::cout<<kc.Recive().value();
       kc.Send("nande monaie");
       kc.ShutDown();
    }
    return 0;
}