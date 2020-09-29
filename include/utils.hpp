#include <string>
#include <vector>
#include <utility>
#include <dirent.h>
#include <map>
class FileUtils 
{
    public: 
        static std::string GetFileExtension(const std::string& FileName);
        static bool GetFileNameWithExtension(std::map<std::string, int> fileWithExtension, std::string directory);
        static std::string GetFileName(const std::string& file);
};