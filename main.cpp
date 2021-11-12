#include<iostream>
#include "Ks_FileSender.hpp"
#include "Ks_FileSender.cpp"
#include<fstream>
int main()
{
  
  Ks_FileSender kfs;
  kfs.AddFile("ejr");
  kfs.IncreaseThread("2156");
  kfs.StartSending();
  std::cin.get();

  return 0;
}