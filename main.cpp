#include<iostream>
#include "Ks_FileTransferer.hpp"
#include "Ks_FileTransferer.cpp"
#include<fstream>
int main()
{
  Ks_FileTransferer kfl;
  kfl.SendFile("jg");
  std::cin.get();
  kfl.ReceiveFile("rfgr");
  return 0;
}