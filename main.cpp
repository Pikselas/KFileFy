#include<iostream>
#include"Ks_Connector.hpp"
#include"Ks_Connector.cpp"
#include<fstream>
int main()
{
    Ks_Connector kc(Ks_Connector::TYPE::SERVER);
    for(auto ip : kc.GetDeviceIps())
    {
        std::cout<<ip<<std::endl;
    }
    return 0;
}