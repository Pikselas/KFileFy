#include"Ks_Connector.hpp"
#include"Ks_Connector.cpp"
#include<iostream>
int main()
{
    Ks_Connector kc(Ks_Connector::TYPE::CLIENT);
    kc.Connect("127.0.0.1","2705");
    if(kc.IsConnected())
    {
        std::cout<<"Connected";
    }
    return 0;
}