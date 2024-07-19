#pragma once
#include "UnityEngine/AudioSource.hpp"
#include "UnityEngine/AudioClip.hpp"
#include "UnityEngine/GameObject.hpp"

#include "System/Threading/Tasks/Task.hpp"
#include "GlobalNamespace/MediaAsyncLoader.hpp"

//#include "GlobalNamespace/SongPreviewPlayer.hpp"

#include "UnityEngine/Networking/UnityWebRequestAsyncOperation.hpp"
#include "UnityEngine/Networking/UnityWebRequestMultimedia.hpp"
#include "UnityEngine/Networking/UnityWebRequest.hpp"

#include "custom-types/shared/coroutine.hpp"


namespace QuestSounds::Utils 
{
    // TODO: Cleanup this class
    class AsyncAudioClipLoader
    {
    public:
        std::string filePath;
        int audioType = 14;
        UnityEngine::AudioSource* audioSource;
        UnityEngine::AudioClip* getClip(); //Audioclip
        bool loaded = false;
        bool load();
        void set_OriginalClip(UnityEngine::AudioClip* OriginalAudioClip); //Audioclip
        UnityEngine::AudioClip* get_OriginalClip(); //Audioclip
        UnityEngine::AudioSource* OriginalAudioSource = nullptr;
    private:
        custom_types::Helpers::Coroutine LoadAudioClip();
        StringW loadPath;
        // custom_types::Helpers::Coroutine loadCoroutine;
        UnityEngine::GameObject* audioClipGO;
        void audioClipCompleted();
        // StringW path;
        bool UsesUWR = false;
        System::Threading::Tasks::Task_1<::UnityW<UnityEngine::AudioClip>>* audioClipTask;

        UnityEngine::Networking::UnityWebRequestAsyncOperation* audioClipAsync;
        UnityEngine::Networking::UnityWebRequest* audioClipRequest;
    };
}