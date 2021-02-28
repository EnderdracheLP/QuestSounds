#pragma once
#include "main.hpp"
#include "custom-types/shared/macros.hpp"
#include <dlfcn.h>

#include "UnityEngine/AudioSource.hpp"
#include "UnityEngine/AudioClip.hpp"

#include "UnityEngine\Networking\UnityWebRequestAsyncOperation.hpp"
#include "UnityEngine\Networking\UnityWebRequest.hpp"


DECLARE_CLASS_CODEGEN(QuestSounds, AudioClipLoader, Il2CppObject,

public:
    DECLARE_INSTANCE_FIELD(Il2CppString* filePath);
    DECLARE_INSTANCE_FIELD(int audioType = 14);
    DECLARE_INSTANCE_FIELD(UnityEngine::AudioSource* audioSource);
    DECLARE_INSTANCE_FIELD(UnityEngine::AudioClip* audioClip);
    DECLARE_INSTANCE_FIELD(UnityEngine::AudioSource* getClip());
    DECLARE_INSTANCE_FIELD(bool loaded = false);
    DECLARE_INSTANCE_FIELD(bool load());

    DECLARE_INSTANCE_FIELD(UnityEngine::Networking::UnityWebRequestAsyncOperation* audioClipRequest);
    DECLARE_INSTANCE_FIELD(UnityEngine::Networking::UnityWebRequest* audioClipAsync);

        DECLARE_METHOD(static void, audioClipCompleted);

        REGISTER_FUNCTION(AudioClipLoader,
            REGISTER_METHOD(audioClipCompleted);

            REGISTER_FIELD(filePath);
            REGISTER_FIELD(audioType);
            REGISTER_FIELD(audioSource);
            REGISTER_FIELD(audioClip);
            REGISTER_FIELD(getClip);
            REGISTER_FIELD(loaded);
            REGISTER_FIELD(load);
    )
)


//    // TODO: figure out all Il2CppObjects
//{
//    class loader : Il2CppObject
//    {
//    public:
//        //static Logger logger;
//        std::string filePath;
//        int audioType = 14;
//        UnityEngine::AudioSource* audioSource;
//        UnityEngine::AudioClip* audioClip;
//        UnityEngine::AudioSource* getClip(); //Audioclip
//        bool loaded = false;
//        bool load();
//    private:
//
//        UnityEngine::Networking::UnityWebRequest* audioClipAsync;
//        UnityEngine::Networking::UnityWebRequestAsyncOperation* audioClipRequest;
//        static void audioClipCompleted(loader* obj, Il2CppObject* asyncOp);
//    };
//}