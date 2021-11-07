#include<iostream>
#include "Ks_FileSender.hpp"
#include "Ks_FileSender.cpp"
#include<fstream>
int main()
{
  Ks_FileSender ksl;
  ksl.IncreaseThread("2146");
  ksl.SendFile("wjerhwjer");
  ksl.SendFile("wjerhwklejrwewjer");
  std::cin.get();
  return 0;
}