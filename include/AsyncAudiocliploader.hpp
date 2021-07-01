#pragma once
#include <dlfcn.h>

#include "UnityEngine/AudioSource.hpp"
#include "UnityEngine/AudioClip.hpp"

#include "System/Threading/Tasks/Task.hpp"
#include "GlobalNamespace/MediaAsyncLoader.hpp"

//#include "GlobalNamespace/SongPreviewPlayer.hpp"

#include "UnityEngine\Networking\UnityWebRequestAsyncOperation.hpp"
#include "UnityEngine\Networking\UnityWebRequestMultimedia.hpp"
#include "UnityEngine\Networking\UnityWebRequest.hpp"

#include "custom-types/shared/coroutine.hpp"


namespace AsyncAudioClipLoader
{
    class loader : public Il2CppObject
    {
    public:
        //static Logger logger;
        std::string filePath;
        int audioType = 14;
        UnityEngine::AudioSource* audioSource;
        UnityEngine::AudioClip* getClip(); //Audioclip
        bool loaded = false;
        bool load();
        UnityEngine::AudioClip* OriginalAudioClip = nullptr;
    private:
        static void audioClipCompleted(loader* obj, Il2CppObject* asyncOp);
        Il2CppString* path;
        bool isOGG = false;
        System::Threading::Tasks::Task_1<UnityEngine::AudioClip*>* audioClipTask;

        UnityEngine::Networking::UnityWebRequestAsyncOperation* audioClipAsync;
        UnityEngine::Networking::UnityWebRequest* audioClipRequest;
        //custom_types::Helpers::Coroutine GetAudioClip(System::Action_1<UnityEngine::AsyncOperation*>* actionUWRM, AsyncAudioClipLoader::loader* self, int audioType, Il2CppString* filePathStr);
        custom_types::Helpers::Coroutine GetAudioClip(AsyncAudioClipLoader::loader* self, int audioType, Il2CppString* filePathStr/*, Logger& logger*/);
    };
}