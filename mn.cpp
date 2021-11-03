#include<iostream>
#include"Ks_Connector.hpp"
#include"Ks_Connector.cpp"
#include<memory>
#include<fstream>
#include<functional>
int main()
{
  Ks_Connector ks(Ks_Connector::TYPE::CLIENT);
  ks.Connect(ks.GetDeviceIps()[0].c_str(),"2144");
  if(ks.IsConnected())
  {
   while(ks.IsConnected())
   {
     auto kData = ks.Recive();
     if(kData)
     {
       std::cout<<kData.value();
        ks.Connect(ks.GetDeviceIps()[0].c_str(),kData.value().c_str());
        if(ks.IsConnected())
        {
          kData = ks.Recive();
          if(kData)
          {
            std::cout<<kData.value();
          }
        }
       break;
     }
   }
  }
  return 0;
}