#pragma once
#include<fstream>
#include"Ks_Connector.hpp"
#include"Ks_Connector.cpp"
#include<memory>
#include<queue>
#include<thread>
#include<mutex>
#include<future>
class Ks_FileTransferer
{
  private:
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
  private:
   int MAX_THREADS = 1;
   int ActiveThreads = 0;
  private:
    std::unique_ptr<Ks_Connector> MAIN_SERVER = nullptr;
  private:
   std::queue<std::string> AvailablePORTS; 
   std::queue<std::string> QueuedFiles;
   std::queue<std::shared_ptr<Ks_Connector>> AvailableServers;
   std::queue<std::future<File_Status>> StatusQueue;
  public:
    std::string LISTEN_PORT = "2144";
    std::string FIRST_THREAD_PORT = "2145";
  public:
    Ks_FileTransferer();
    ~Ks_FileTransferer();
  private:
    File_Status SendFileByServer(std::string ,std::shared_ptr<Ks_Connector>);
  public:
    size_t GetTotalPendings() const;
    int GetMAxThreads() const;
    int GetActiveThreads() const;
    void IncreaseThread(const char *);
    void DecreaseThread();
  public:
    void SendFile(const char *);
    void ReceiveFile(const char *);
  public:
    // void Kill();
};