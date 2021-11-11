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
Ks_FileReceiver::File_Status Ks_FileReceiver::ReceiveFile(std::string name,std::string ip ,std::string port)
{
    std::cout<<"started:" << name << std::endl << ip << std::endl << port;
    auto Connector = std::make_unique<Ks_Connector>(Ks_Connector::TYPE::CLIENT);
    Connector->Connect(ip.c_str(),port.c_str());
    while(Connector->IsConnected())
    {
        std::cout<<"Connected";
        auto Data = Connector->Recive();
        if(Data)
        {
            std::cout<<Data.value();
        }
    }
    ActiveThreads--;
    return File_Status{name,true};
}
void Ks_FileReceiver::StartReceiving(std::string ip , std::string port,std::string path)
{
    MAIN_CONNECTOR = std::make_unique<Ks_Connector>(Ks_Connector::TYPE::CLIENT);
    while(!StopReceiving)
    {
        if( ActiveThreads < MAX_THREADS)
        {
            MAIN_CONNECTOR->Connect(ip.c_str(),port.c_str());
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
                       std::packaged_task<Ks_FileReceiver::File_Status()> FileTask(std::bind(&Ks_FileReceiver::ReceiveFile,this,ParsedData[1],ip,ParsedData[0]));
                       StatusList.emplace_back(FileTask.get_future());
                       std::thread(std::move(FileTask)).detach();
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