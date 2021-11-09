#include"Ks_FileReceiver.hpp"
void Ks_FileReceiver::IncreaseThread()
{
    MAX_THREADS++;
}
void Ks_FileReceiver::DecreaseThread()
{
    if(MAX_THREADS > 0)
    {
     MAX_THREADS--;
    }
}
Ks_FileReceiver::File_Status Ks_FileReceiver::ReceiveFile(const char* name, const char* ip , const char* port)
{
    return File_Status{name,true};
}
void Ks_FileReceiver::StartReceiving(const char * ip , const char* port,const char* path)
{
    MAIN_CONNECTOR = std::make_unique<Ks_Connector>(Ks_Connector::TYPE::CLIENT);
    while(!StopReceiving)
    {
        if( ActiveThreads < MAX_THREADS)
        {
            MAIN_CONNECTOR->Connect(ip,port);
            if(MAIN_CONNECTOR->IsConnected())
            {
                while(MAIN_CONNECTOR->IsConnected())
                {
                    auto data = MAIN_CONNECTOR->Recive();
                    if(data)
                    {
                       
                       break; 
                    }
                }
            }
        }
    }
}