#include<iostream>
#include "Ks_FileSender.hpp"
#include "Ks_FileSender.cpp"
#include<fstream>
int main()
{
  Ks_FileSender ksl;
  ksl.SendFile("wjerhwjer");
  std::cin.get();
  return 0;
}