#include"Ks_Connector.hpp"
#include"Ks_Connector.cpp"
#include<iostream>
int main()
{
    Ks_Connector kc(Ks_Connector::TYPE::SERVER);
    kc.Listen("2705");
    std::cout<<"GotConnection";
    return 0;
}