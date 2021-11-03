#include<iostream>
#include "Ks_FileTransferer.hpp"
#include "Ks_FileTransferer.cpp"
#include<fstream>
int main()
{
  Ks_FileTransferer kfl;
  kfl.SendFile("akjdjkla");
  std::cin.get();
  return 0;
}