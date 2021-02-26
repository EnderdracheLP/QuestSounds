#include "main.hpp"
#include <dlfcn.h>
namespace audioClipLoader
{
    class loader : Il2CppObject
    {
        public:
            //static Logger logger;
            std::string filePath;
            int audioType = 14;
            Il2CppObject* audioSource;
            Il2CppObject* audioClip;
            Il2CppObject* getClip(); //Audioclip
            bool loaded = false;
            bool load();
        private: 
            
            Il2CppObject* audioClipAsync;
            Il2CppObject* audioClipRequest;
            static void audioClipCompleted(loader* obj, Il2CppObject* asyncOp);
    };
}