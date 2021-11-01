#include<iostream>
#include"Ks_Connector.hpp"
#include"Ks_Connector.cpp"
#include<fstream>
int main()
{
  Ks_Connector ks(Ks_Connector::TYPE::CLIENT);  
  while(true)
  {
    ks.Connect("192.168.43.39","700");
    if(ks.IsConnected())
    {
      ks.CloseConnection();
    }
    std::cin.get();
  } 
  return 0;
}