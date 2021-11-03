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
   int ThreadCount = 0;
   int MAX_THREADS = 1;
   int ServerActivated = false;
  private:
    std::unique_ptr<Ks_Connector> MAIN_SERVER = nullptr;
  private:
   std::vector<std::string> PORTS = {"2145"}; 
   std::queue<std::string> QueuedFiles;
   std::queue<std::shared_ptr<Ks_Connector>> AvailableServers;
  public:
    std::string LISTEN_PORT = "2144";
  private:
    void SendFileByServer(std::string ,std::shared_ptr<Ks_Connector>);
  public:
    size_t GetTotalPendings() const;
    int GetMAxThreads() const;
    void IncreaseThread(const char *);
    void ChangePort(int , const char *);
    void DecreaseThread();
  public:
    void SendFile(const char *);
    void ReceiveFile(const char *);
  public:
    // void Kill();
};