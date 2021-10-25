#include"Ks_Connector.hpp"
#include"Ks_Connector.cpp"
#include<iostream>
#include<fstream>
int main()
{
    std::ifstream in("README.md",std::ios::binary);
    char buff[100];
    Ks_Connector kc(Ks_Connector::TYPE::SERVER);
    kc.Listen("700");
    while(in.read(buff,sizeof(buff)))
    {
        if(!kc.Send(buff))
        {
            std::cout<<"breaking";
            break;
        }
    }
    kc.ShutDown();
    return 0;
}