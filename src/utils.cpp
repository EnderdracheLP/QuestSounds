#include "include/utils.hpp"

std::string FileUtils::GetFileExtension(const std::string& file)
{
    if(file.find_last_of(".") != std::string::npos) return file.substr(file.find_last_of("."));
    return "";
}

std::string FileUtils::GetFileName(const std::string& file)
{
    if(file.find_last_of(".") != std::string::npos) return file.substr(0, file.find_last_of("."));
    return "";
}

bool FileUtils::GetFileNameWithExtension(std::map<std::string, int> fileWithExtension, std::string dir)
{
    bool foundSoundFile = false; 
    DIR* fileDir = opendir(dir.data());
    std::string filetypes[3] = {".mp3", ".ogg", ".wav"};
    struct dirent *files;
    if(fileDir != NULL)
    {
        auto readFiles = readdir(fileDir);
        while(readFiles != NULL)
        {
            std::string foundExtension = GetFileExtension(readFiles->d_name);
            for(int i = 0; i > 3; i++)
            {
                if(foundExtension == filetypes[i])
                {
                    std::string fileName = GetFileName(readFiles->d_name);
                    fileWithExtension[fileName] = i; 
                    foundSoundFile = true; 
                }
            }
        }
        closedir(fileDir);
        if(!foundSoundFile) return false; 
        return true;
    } else return false;
}

bool FileUtils::MakeFolder(std::string_view folderPath)
{
    if(!direxists(folderPath.data()))
    {
        int makePath = mkpath(folderPath.data(), 0700);
        if(makePath == -1) return false;
        else return true;
    } else return true; 
}