#include"Ks_FileSender.hpp"
Ks_FileSender::~Ks_FileSender()
{
    if(MAIN_SERVER != nullptr)
    {
     MAIN_SERVER->ShutDown();
    }
}
Ks_FileSender::File_Status Ks_FileSender::SendFileByServer(std::string File,std::shared_ptr<Ks_Connector> Server)
{
    Server->AllowConnection();
    if(Server->IsConnected())
    {
        Server->Send("hey its the sub child\n");
        Server->CloseConnection();
    }
    std::mutex mtx;
    std::lock_guard<std::mutex> guard(mtx);
    ActiveThreads--;
    //if no file is pending then no need to keep a server alive
    if(!QueuedFiles.empty())
    {
        AvailableServers.push(Server);
    }
    return File_Status{File,true};
}
void Ks_FileSender::SendFile(const char * path)
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
                                port = "0"; 
                            }
                            while(MAIN_SERVER->IsConnected())
                            {
                                if(MAIN_SERVER->Send(port))
                                {
                                    if(WillShare)
                                    {
                                        ActiveThreads++;

                                        //using Tserver = std::shared_ptr<Ks_Connector>;
                                        
                                        StatusQueue.push(std::async(std::launch::async,&Ks_FileSender::SendFileByServer,this,FileName,FileServer));

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