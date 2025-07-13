#pragma once
#include "UnityEngine/AudioSource.hpp"
#include "UnityEngine/AudioClip.hpp"
#include "UnityEngine/GameObject.hpp"

#include "System/Type.hpp"
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
    // TODO: Allow loading multiple audio clips at once
    class AsyncAudioClipLoader
    {
    public:
        std::string filePath;
        int audioType = 14;
        UnityEngine::AudioSource* audioSource;
        UnityEngine::AudioClip* getClip(); //Audioclip
        // ListW<UnityEngine::AudioClip*> getClips(); //Audioclip
        bool streamAudio = true;
        bool loaded = false;
        bool load();
        void set_OriginalClip(UnityEngine::AudioClip* OriginalAudioClip); //Audioclip
        UnityEngine::AudioClip* get_OriginalClip(); //Audioclip
        UnityEngine::AudioSource* OriginalAudioSource = nullptr;
    private:
        custom_types::Helpers::Coroutine LoadAudioClip();
        StringW loadPath;
        // ListW<StringW> loadPaths;
        UnityEngine::GameObject* audioClipGO;
        void audioClipCompleted(::UnityW<UnityEngine::AudioClip> audioClip);
        System::Threading::Tasks::Task_1<::UnityW<UnityEngine::AudioClip>>* audioClipTask;

        UnityEngine::Networking::UnityWebRequestAsyncOperation* audioClipAsync;
        UnityEngine::Networking::UnityWebRequest* audioClipRequest;
    };

    struct EnumHelper {
        template <class T>
        static StringW GetEnumName(T value)
        {
            auto enumType = (System::Type*)csTypeOf(T);
            return enumType->GetEnumName((System::Object*)il2cpp_functions::value_box(classof(T), &value));
        }
    };
}