#include"Ks_FileTransferer.hpp"
size_t Ks_FileTransferer::GetTotalPendings() const
{
    return QueuedFiles.size();
}
int Ks_FileTransferer::GetMAxThreads() const
{
    return MAX_THREADS;
}
void Ks_FileTransferer::IncreaseThread(const char * port)
{
    PORTS.emplace_back(port);
    MAX_THREADS++;
}
void Ks_FileTransferer::ChangePort(int index,const char* port)
{
    PORTS.emplace(PORTS.begin() + index ,port);
}
void Ks_FileTransferer::DecreaseThread()
{
    PORTS.pop_back();
    MAX_THREADS--;
}
void Ks_FileTransferer::SendFileByServer(std::string File,std::shared_ptr<Ks_Connector> Server)
{
    Server->AllowConnection();
    if(Server->IsConnected())
    {
        Server->Send("hey its the sub child\n");
        Server->CloseConnection();
        ThreadCount--;
    }
}
void Ks_FileTransferer::SendFile(const char * path)
{
  QueuedFiles.push(path);
  //there is no active file servers so buliding them
  if(MAIN_SERVER == nullptr)
  {
   MAIN_SERVER = std::make_unique<Ks_Connector>(Ks_Connector::TYPE::SERVER);
   MAIN_SERVER->Listen(LISTEN_PORT.c_str());
   std::thread([this](){
                    while(!QueuedFiles.empty())
                    {
                        MAIN_SERVER->AllowConnection(); 
                        if(MAIN_SERVER->IsConnected())
                        {
                            std::string port;
                            std::shared_ptr<Ks_Connector> FileServer = nullptr;
                            auto FileName = QueuedFiles.front();
                            if(ThreadCount < MAX_THREADS)
                            {
                                if(!AvailableServers.empty())
                                {
                                    FileServer = AvailableServers.front();
                                    port = FileServer->ListeningOn();
                                }
                                else
                                {
                                    port = PORTS[ThreadCount];
                                    FileServer = std::make_shared<Ks_Connector>(Ks_Connector::TYPE::SERVER);
                                    FileServer->Listen(port.c_str());
                                    ThreadCount++;
                                }
                            }
                            else
                            {
                                port = "0"; 
                            }
                            while(MAIN_SERVER->IsConnected())
                            {
                                if(MAIN_SERVER->Send(port))
                                {
                                    if(!AvailableServers.empty())
                                    {
                                      AvailableServers.pop();
                                    }
                                    QueuedFiles.pop();
                                    std::thread([this,FileName,FileServer](){
                                         SendFileByServer(FileName,FileServer);
                                    }).detach();
                                    MAIN_SERVER->CloseConnection();
                                    break;
                                }
                            }   
                        }
                    }
                    //Destroying the main server because no file is left
                    MAIN_SERVER = nullptr;
                   // No file is left so destroying all active servers
                   while(!AvailableServers.empty())
                   {
                       AvailableServers.pop();
                   }//need to research if copy and swap idom will work or not?
                }).detach();
  }
}
void Ks_FileTransferer::ReceiveFile(const char * directory )
{

}