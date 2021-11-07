#pragma once
#include"Ks_FileTransferer.hpp"
#include"Ks_FileTransferer.cpp"
class Ks_FileSender : public Ks_FileTransferer
{
     private:
      std::unique_ptr<Ks_Connector> MAIN_SERVER = nullptr;
     private:
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
      void SendFile(const char *);
      void IncreaseThread(const char *);
      void ClearServers();
    public:
     virtual void DecreaseThread() override;
};