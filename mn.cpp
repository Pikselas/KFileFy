#include"Ks_Connector.hpp"
#include"Ks_Connector.cpp"
#include<iostream>
#include<fstream>
int main()
{
    std::fstream in("README.md");
    Ks_Connector kc(Ks_Connector::TYPE::SERVER);
    kc.Listen("700");
    std::string st;
    while(std::getline(in,st))
    {
        std::cout<<st<<std::endl;
        if(!kc.Send(st))
        {
            std::cout<<"breaking";
            break;
        }
    }
    kc.ShutDown();
    return 0;
}