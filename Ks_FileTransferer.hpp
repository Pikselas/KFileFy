#pragma once
#include<fstream>
#include"Ks_Connector.hpp"
#include"Ks_Connector.cpp"
#include<memory>
#include<queue>
#include<vector>
#include<thread>
#include<mutex>
#include<future>
class Ks_FileTransferer
{
  protected:
   class File_Status
   {
      private:
       std::pair<std::string,bool> details;
      public:
       // ok we need an default constructor for future object
        File_Status() = default;
        File_Status(std::string,bool);
        ~File_Status() = default;
      public:
        std::pair<std::string,bool> GetDetails() const;
   };
  protected:
   int MAX_THREADS = 0;
   int ActiveThreads = 0;
  protected:
   std::vector<std::future<File_Status>> StatusList;
  public:
    int GetMAxThreads() const;
    int GetActiveThreads() const;
  public:
    virtual void DecreaseThread() = 0;
  public:
    void ClearStatus();
  public:
    // void Kill();
};