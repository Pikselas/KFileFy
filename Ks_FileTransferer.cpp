#include"Ks_FileTransferer.hpp"
Ks_FileTransferer::File_Status::File_Status(std::string Name , bool Status) : details(std::make_pair(Name,Status)){}
std::pair<std::string,bool> Ks_FileTransferer::File_Status::GetDetails() const
{
    return details;
}
int Ks_FileTransferer::GetMAxThreads() const
{
    return MAX_THREADS;
}
int Ks_FileTransferer::GetActiveThreads() const
{
    return ActiveThreads;
}
void Ks_FileTransferer::ClearStatus()
{
    StatusList.clear();
}