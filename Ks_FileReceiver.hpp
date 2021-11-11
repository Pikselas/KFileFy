#pragma once
#include "Ks_FileTransferer.hpp"
#include "Ks_FileTransferer.cpp"
#include "KsXstr.hpp"
class Ks_FileReceiver : public Ks_FileTransferer
{
    private:
     bool StopReceiving = false;
    private:
     std::unique_ptr<Ks_Connector> MAIN_CONNECTOR = nullptr;
    private:
     File_Status ReceiveFile(const char* , const char*,const char*);  
    public:
     void IncreaseThread();
     void DecreaseThread() override;
     void StartReceiving(const char *,const char *,const char *);
};