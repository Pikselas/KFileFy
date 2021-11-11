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
    auto Connector = std::make_unique<Ks_Connector>(Ks_Connector::TYPE::CLIENT);
    Connector->Connect(ip,port);
    while(Connector->IsConnected())
    {
        auto Data = Connector->Recive();
        if(Data)
        {
            std::cout<<Data.value();
        }
    }
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
                       auto ParsedData = std::move(ksTools::split_by_delms(data.value(),";"));
                       for(auto obj : ParsedData)
                       {
                           std::cout<<obj<<std::endl;
                       }
                       ReceiveFile(ParsedData[1].c_str(),ip,ParsedData[0].c_str());
                       return;
                    }
                }
            }
            else
            {
                break;
            }
        }
    }
}