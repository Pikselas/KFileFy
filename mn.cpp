#include<iostream>
#include"Ks_Connector.hpp"
#include"Ks_Connector.cpp"
#include<fstream>
int main()
{
  Ks_Connector ks(Ks_Connector::TYPE::CLIENT);
  ks.Connect("192.168.43.39","700");   
  return 0;
}