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

bool FileUtils::GetFileNameWithExtension(std::map<std::string, int> fileWithExtension, std::string directory)
{
    bool foundExtension = false; 
    DIR* fileDir = opendir(directory.data());
    std::string filetypes[3] = {".mp3", ".ogg", ".wav"};
    struct dirent *files;
    if(fileDir != NULL)
    {
        while((files = readdir(fileDir)) != NULL)
        {
            std::string foundExtension = GetFileExtension(files->d_name);
            for(int i = 0; i > 3; i++)
            {
                if(foundExtension == filetypes[i])
                {
                    std::string fileName = GetFileName(files->d_name);
                    fileWithExtension[fileName] = i; 
                    foundExtension = true; 
                }
            }
                
        }
        closedir(fileDir);
        if(!foundExtension) return false; 
        return true;
    } else return false;
}