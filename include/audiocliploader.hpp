#include <dlfcn.h>
#include "../extern/beatsaber-hook/shared/utils/utils.h"
#include "../extern/beatsaber-hook/shared/utils/logging.hpp"
#include "../extern/beatsaber-hook/include/modloader.hpp"
#include "../extern/beatsaber-hook/shared/utils/typedefs.h"
#include "../extern/beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "../extern/beatsaber-hook/shared/utils/il2cpp-functions.hpp"
namespace audioClipLoader
{
    class loader : Il2CppObject
    {
        public:
            //static Logger logger;
            std::string filePath;
            int audioType;
            Il2CppObject* loadedClip; //Audioclip
            bool loaded = false;
            bool load();
        private: 
            Il2CppObject* audioClipAsync;
            Il2CppObject* audioClipRequest;
            static void audioClipCompleted(loader* obj, Il2CppObject* asyncOp);
    };
}