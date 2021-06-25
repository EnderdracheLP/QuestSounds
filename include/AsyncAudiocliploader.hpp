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

namespace AsyncAudioClipLoader
{
    class loader : public Il2CppObject
    {
    public:
        //static Logger logger;
        std::string filePath;
        int audioType = 14;
        UnityEngine::AudioSource* audioSource;
        UnityEngine::AudioClip* audioClip;
        UnityEngine::AudioClip* getClip(); //Audioclip
        bool loaded = false;
        bool load();
    private:
        static void audioClipCompleted(loader* obj, Il2CppObject* asyncOp);
        Il2CppString* path;
        bool isOGG = false;
        System::Threading::Tasks::Task_1<UnityEngine::AudioClip*>* audioClipTask;

        UnityEngine::Networking::UnityWebRequestAsyncOperation* audioClipAsync;
        UnityEngine::Networking::UnityWebRequest* audioClipRequest;

    };
}