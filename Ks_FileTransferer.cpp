#include"Ks_FileTransferer.hpp"
Ks_FileTransferer::File_Status::File_Status(std::string Name , bool Status) : details(std::make_pair(Name,Status)){}
std::pair<std::string,bool> Ks_FileTransferer::File_Status::GetDetails() const
{
    return details;
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
void Ks_FileTransferer::ReceiveFile(const char * directory )
{
    auto[name,type] = StatusQueue.front().get().GetDetails();
    std::cout<< QueuedFiles.size() << std::endl 
             << name << std::endl << type;
}