#include "main.hpp"
#include <dlfcn.h>

#include "UnityEngine/AudioSource.hpp"
#include "UnityEngine/AudioClip.hpp"

#include "UnityEngine\Networking\UnityWebRequestAsyncOperation.hpp"
#include "UnityEngine\Networking\UnityWebRequestMultimedia.hpp"
#include "UnityEngine\Networking\UnityWebRequest.hpp"


    // TODO: figure out all Il2CppObjects
namespace audioClipLoader
{
    class loader : public Il2CppObject
    {
    public:
        //static Logger logger;
        std::string filePath;
        int audioType = 14;
        UnityEngine::AudioSource* audioSource;
        UnityEngine::AudioClip* audioClip;
        UnityEngine::AudioSource* getClip(); //Audioclip
        bool loaded = false;
        bool load();
    private:
        static void audioClipCompleted(loader* obj, Il2CppObject* asyncOp);

        UnityEngine::Networking::UnityWebRequestAsyncOperation* audioClipAsync;
        UnityEngine::Networking::UnityWebRequest* audioClipRequest;
    };
}