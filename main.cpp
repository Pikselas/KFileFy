#include"Ks_Connector.hpp"
#include"Ks_Connector.cpp"
#include<iostream>
#include<fstream>
int main()
{
    Ks_Connector kc(Ks_Connector::TYPE::CLIENT);
    kc.Connect("127.0.0.1","700");
    if(kc.IsConnected())
    {
       while(kc.IsConnected())
       {
           auto Data = kc.Recive();
           if(Data)
           {
               std::cout<<Data.value();
           }
       }
    }
    return 0;
}