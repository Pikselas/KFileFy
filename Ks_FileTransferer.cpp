#include"Ks_FileTransferer.hpp"
Ks_FileTransferer::Ks_FileTransferer()
{
    AvailablePORTS.push(FIRST_THREAD_PORT);
}
size_t Ks_FileTransferer::GetTotalPendings() const
{
    return QueuedFiles.size();
}
int Ks_FileTransferer::GetMAxThreads() const
{
    return MAX_THREADS;
}
int Ks_FileTransferer::GetActiveThreads() const
{
    return ActiveThreads;
}
void Ks_FileTransferer::IncreaseThread(const char * port)
{
    AvailablePORTS.push(port);
    MAX_THREADS++;
}
void Ks_FileTransferer::DecreaseThread()
{
    AvailablePORTS.pop();
    MAX_THREADS--;
}
void Ks_FileTransferer::SendFileByServer(std::string File,std::shared_ptr<Ks_Connector> Server)
{
    Server->AllowConnection();
    if(Server->IsConnected())
    {
        Server->Send("hey its the sub child\n");
        Server->CloseConnection();
    }
    ActiveThreads--;
    //if no file is pending then no need to keep a server alive
    if(!QueuedFiles.empty())
    {
        AvailableServers.push(Server);
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
                            bool WillShare;
                            std::string port;
                            std::shared_ptr<Ks_Connector> FileServer = nullptr;
                            auto FileName = QueuedFiles.front();
                            if(!AvailablePORTS.empty())
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
                                port = "0"; 
                            }
                            while(MAIN_SERVER->IsConnected())
                            {
                                if(MAIN_SERVER->Send(port))
                                {
                                    if(WillShare)
                                    {
                                        ActiveThreads++;
                                        std::thread([this,FileName,FileServer](){
                                            SendFileByServer(FileName,FileServer);
                                        }).detach();
                                        if(!AvailableServers.empty())
                                        {
                                         AvailableServers.pop();
                                        }
                                        QueuedFiles.pop();
                                        MAIN_SERVER->CloseConnection();
                                    }
                                    break;
                                }
                                ///now find a method how to push the port of destoryed file server
                                // to the queue for reuse
                            }   
                        }
                    }
                    //Destroying the main server because no file is left
                    MAIN_SERVER->ShutDown();
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
    std::cout<< QueuedFiles.size() << std::endl << AvailablePORTS.front() << std::endl << AvailableServers.size();
}
Ks_FileTransferer::~Ks_FileTransferer()
{
    if(MAIN_SERVER != nullptr)
    {
     MAIN_SERVER->ShutDown();
    }
}