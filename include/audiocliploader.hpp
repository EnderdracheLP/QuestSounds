#pragma once
//#include <dlfcn.h>
#include "main.hpp"
//#include <dlfcn.h>
//#include "../extern/beatsaber-hook/shared/utils/utils.h"
//#include "../extern/beatsaber-hook/shared/utils/logging.hpp"
//#include "modloader/shared/modloader.hpp"
//#include "../extern/beatsaber-hook/shared/utils/typedefs.h"
//#include "../extern/beatsaber-hook/shared/utils/il2cpp-utils.hpp"
//#include "../extern/beatsaber-hook/shared/utils/il2cpp-functions.hpp"
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