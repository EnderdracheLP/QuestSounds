#include <dlfcn.h>
#include "../extern/beatsaber-hook/shared/utils/utils.h"
#include "../extern/beatsaber-hook/shared/utils/typedefs.h"
namespace audioClipLoader
{
    class loader : Il2CppObject*
    {
        public:
            std::string filePath;
            int audioType;
            Il2CppObject* loadedClip; //Audioclip
            Il2CppObject* action;
            bool loaded;
            bool load();
        private: 
            Il2CppObject* audioClipAsync;
            static void audioClipCompleted(AssetBundle* obj, Il2CppObject* asyncOp);
    };
}