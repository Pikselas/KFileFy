#include"Ks_Connector.hpp"
#include"Ks_Connector.cpp"
#include<iostream>
#include<fstream>
int main()
{
    Ks_Connector kc(Ks_Connector::TYPE::CLIENT);
    std::ofstream out("kl.md");
    kc.Connect("127.0.0.1","700");
    if(kc.IsConnected())
    {
       while(kc.IsConnected())
       {
           auto Data = kc.Recive();
           if(Data)
           {
              out.write(Data.value().c_str(),Data.value().length());
              out.write("\n",1);
           }
           Data.reset();
       }
    }
    return 0;
}