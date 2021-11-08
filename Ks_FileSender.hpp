#pragma once
#include"Ks_FileTransferer.hpp"
#include"Ks_FileTransferer.cpp"
class Ks_FileSender : public Ks_FileTransferer
{
     private:
      bool StopSending = false;
     private:
      std::unique_ptr<Ks_Connector> MAIN_SERVER = nullptr;
     private:
      std::queue<std::string> QueuedFiles;
      std::queue<std::string> AvailablePORTS; 
      std::queue<std::shared_ptr<Ks_Connector>> AvailableServers;
    private:
      File_Status SendFileByServer(std::string ,std::shared_ptr<Ks_Connector>);
    public:
      Ks_FileSender() = default;
      ~Ks_FileSender();
    public:
      std::string LISTEN_PORT = "2144";
    public:
      void AddFile(const char *);
      void StartSending();
      void IncreaseThread(const char *);
      void ClearPendings();
      void ClearServers();
    public:
      size_t GetTotalPendings() const;
    public:
     virtual void DecreaseThread() override;
};