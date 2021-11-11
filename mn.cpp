#include<iostream>
#include"Ks_FileReceiver.hpp"
#include"Ks_FileReceiver.cpp"
int main()
{
  Ks_FileReceiver krs;
  krs.IncreaseThread();
  krs.StartReceiving("127.0.0.1","2144",".w/q");
  return 0;
}