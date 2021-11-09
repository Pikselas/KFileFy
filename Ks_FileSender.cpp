#include"Ks_FileSender.hpp"
Ks_FileSender::~Ks_FileSender()
{
    if(MAIN_SERVER != nullptr)
    {
     MAIN_SERVER->ShutDown();
    }
}
void Ks_FileSender::ClearPendings()
{
    while(!QueuedFiles.empty())
    {
        QueuedFiles.pop();
    }
}
size_t Ks_FileSender::GetTotalPendings() const
{
    return QueuedFiles.size();
}
Ks_FileSender::File_Status Ks_FileSender::SendFileByServer(std::string File,std::shared_ptr<Ks_Connector> Server)
{
    
    Server->AllowConnection();
    while(Server->IsConnected())
    {
        Server->Send("");
        Server->CloseConnection();
    }
    std::mutex mtx;
    std::lock_guard<std::mutex> guard(mtx);
    ActiveThreads--;
    AvailableServers.push(Server);
    return File_Status{File,true};
}
void Ks_FileSender::AddFile(const char * path)
{
    QueuedFiles.push(path);
}
void Ks_FileSender::StartSending()
{
   StopSending = false;
   MAIN_SERVER = std::make_unique<Ks_Connector>(Ks_Connector::TYPE::SERVER);
   MAIN_SERVER->Listen(LISTEN_PORT.c_str());
   while(!QueuedFiles.empty() && !StopSending)
    {
        MAIN_SERVER->AllowConnection(); 
        if(MAIN_SERVER->IsConnected())
        {
            bool WillShare;
            std::string port;
            std::shared_ptr<Ks_Connector> FileServer = nullptr;
            auto FileName = QueuedFiles.front();
            if(!AvailablePORTS.empty() || !AvailableServers.empty())
            {
                WillShare = true;
                if(!AvailableServers.empty())
                {
                    FileServer = AvailableServers.front();
                    port = FileServer->ListeningOn();
                }
                else
                {
                    port = AvailablePORTS.front();
                    //passing a local function so whenever this server will be destroyed the port will be freed
                    // and pushed to the port queue
                    FileServer = std::make_shared<Ks_Connector>(Ks_Connector::TYPE::SERVER,[this](Ks_Connector* OBJ){
                            AvailablePORTS.push(OBJ->ListeningOn());
                    });
                    FileServer->Listen(port.c_str());
                    AvailablePORTS.pop();
                }
            }
            else
            {
                WillShare = false;
                port = "-1"; 
            }
            while(MAIN_SERVER->IsConnected())
            {
                if(MAIN_SERVER->Send(port + ";" + FileName + std::to_string(QueuedFiles.size() - 1)))
                {
                    if(WillShare)
                    {
                        ActiveThreads++;

                        //using Tserver = std::shared_ptr<Ks_Connector>;
                        
                        StatusList.push_back(std::async(std::launch::async,&Ks_FileSender::SendFileByServer,this,FileName,FileServer));

                        if(!AvailableServers.empty())
                        {
                            AvailableServers.pop();
                        }
                        QueuedFiles.pop();
                        MAIN_SERVER->CloseConnection();
                    }
                    break;
                }
            }   
        }
    }
    //Destroying the main server because no file is left
    MAIN_SERVER->ShutDown();
    MAIN_SERVER = nullptr;
  }
void Ks_FileSender::ClearServers()
{
    while(!AvailableServers.empty())
    {
        AvailableServers.pop();
    }
}
void Ks_FileSender::IncreaseThread(const char * port)
{
    AvailablePORTS.push(port);
    MAX_THREADS++;
}
void Ks_FileSender::DecreaseThread()
{
    if(!AvailablePORTS.empty())
    {
        AvailablePORTS.pop();
        MAX_THREADS--;
    }
}