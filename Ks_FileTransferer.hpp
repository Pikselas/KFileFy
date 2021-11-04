#pragma once
#include<fstream>
#include"Ks_Connector.hpp"
#include"Ks_Connector.cpp"
#include<memory>
#include<queue>
#include<thread>
class Ks_FileTransferer
{
  private:
   int MAX_THREADS = 1;
   int ActiveThreads = 0;
  private:
    std::unique_ptr<Ks_Connector> MAIN_SERVER = nullptr;
  private:
   std::queue<std::string> AvailablePORTS; 
   std::queue<std::string> QueuedFiles;
   std::queue<std::shared_ptr<Ks_Connector>> AvailableServers;
  public:
    std::string LISTEN_PORT = "2144";
    std::string FIRST_THREAD_PORT = "2145";
  public:
    Ks_FileTransferer();
    ~Ks_FileTransferer();
  private:
    void SendFileByServer(std::string ,std::shared_ptr<Ks_Connector>);
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