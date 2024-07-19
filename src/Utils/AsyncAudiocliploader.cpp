// #include "QSoundsConfig.hpp"
#include "Utils/AsyncAudioClipLoader.hpp"
#include "main.hpp"
#include "Config.hpp"
#include "ObjectInstances.hpp"

#include <dirent.h>
#include <regex>

#include <chrono>

#include "custom-types/shared/delegate.hpp"
#include "bsml/shared/BSML/MainThreadScheduler.hpp"
#include "bsml/shared/BSML/SharedCoroutineStarter.hpp"

#include "UnityEngine/Networking/DownloadHandlerAudioClip.hpp"
#include "UnityEngine/Networking/UnityWebRequestMultimedia.hpp"
using namespace UnityEngine::Networking;

#include "System/Threading/Tasks/TaskStatus.hpp"
#include "System/Threading/Tasks/Task.hpp"
#include "System/Threading/Tasks/Task_1.hpp"
#include "System/Action_1.hpp"
using namespace System;

#include "UnityEngine/AudioType.hpp"
#include "UnityEngine/AsyncOperation.hpp"
#include "UnityEngine/GameObject.hpp"
using namespace UnityEngine;

int getAudioType(std::string path) {
    if (path.ends_with(".ogg")) {
        getLogger().debug("File is OGGVORBIS");
        return (int)UnityEngine::AudioType::OGGVORBIS;
    }
    else if (path.ends_with(".mp3") || path.ends_with(".mp2")) {
        getLogger().debug("File is MPEG");
        return (int)UnityEngine::AudioType::MPEG;
    }
    else if (path.ends_with(".wav")) {
        getLogger().debug("File is WAV");
        return (int)UnityEngine::AudioType::WAV;
    }
    else if (path.ends_with(".aiff") || path.ends_with(".aif")) {
        getLogger().debug("File is AIFF");
        return (int)UnityEngine::AudioType::AIFF;
    }
    return 0;
}

System::Threading::Tasks::Task_1<UnityEngine::AudioClip*>* audioClipTask;
//bool NeedReload = false;

bool QuestSounds::Utils::AsyncAudioClipLoader::load()
{
    loaded = false;
    //Stage 0 
    bool fileError;
    getLogger().info("Starting Stage 0");
    getLogger().info("FilePath to check is {}", filePath.c_str());
    if (filePath.starts_with("https://") || filePath.starts_with("http://")) {
        getLogger().info("Filepath is URL, skipping file checks and try loading it");
        fileError = true;
    }
    else {
        // Checks if the given File in the config exists and
        fileError = fileexists(filePath);
        getLogger().info("File error is {}", fileError);
        if (!fileError)
        {
            getLogger().error("File {} not found", filePath.c_str());
            getLogger().error("Stage 0 Failed");
            return false;
        }
        else { getLogger().info("Stage 0 Done with {}", filePath.c_str()); }
    }

    //Stage 1
    if (!(filePath == QuestSounds::Config.Sounds.HitSound.FilePath) && filePath.ends_with(".ogg")) {
        getLogger().info("Stage 1: Running MediaAsyncLoader for FilePath {}", filePath.c_str());
        UsesUWR = false;
        loadPath = filePath;
        if (!classof(GlobalNamespace::MediaAsyncLoader*)->initialized) il2cpp_functions::Class_Init(classof(GlobalNamespace::MediaAsyncLoader*));
        // if (!mediaAsyncLoader) mediaAsyncLoader = ::il2cpp_utils::NewSpecific<::GlobalNamespace::MediaAsyncLoader*, il2cpp_utils::CreationType::Manual>();
        audioClipTask = GlobalNamespace::MediaAsyncLoader::New_ctor()->LoadAudioClipFromFilePathAsync(loadPath);

        audioClipTask->GetAwaiter().OnCompleted(custom_types::MakeDelegate<System::Action*>(std::function([this]()
        { 
            audioClipCompleted(); 
        })));
    }
    else {
        getLogger().info("Stage 1: Running UnityWebRequestMultimedia for FilePath {}", filePath.c_str());
        UsesUWR = true;
        if (filePath.starts_with("https://") || filePath.starts_with("http://")) {
            loadPath = filePath;
        }
        else {
            // EscapeURL but also replaces spaces with %20 since the UnityWebRequest replaces spaces with + which causes unitys curl call to fail
            loadPath = "file:///" + UnityEngine::Networking::UnityWebRequest::EscapeURL(std::regex_replace(filePath, std::regex(" "), "%20"));
        }
        audioType = getAudioType(filePath);

        BSML::SharedCoroutineStarter::get_instance()->StartCoroutine(custom_types::Helpers::CoroutineHelper::New(LoadAudioClip()));
    }
    getLogger().info("Stage 1 done with filepath {}", filePath.c_str());
    return true;
}

custom_types::Helpers::Coroutine QuestSounds::Utils::AsyncAudioClipLoader::LoadAudioClip()
{
    getLogger().info("Stage 1: Running UnityWebRequestMultimedia for LoadPath {}", loadPath);
    audioClipRequest = UnityEngine::Networking::UnityWebRequestMultimedia::GetAudioClip(loadPath, audioType);
    audioClipAsync = audioClipRequest->SendWebRequest();
    audioClipAsync->allowSceneActivation = false;
    co_yield reinterpret_cast<System::Collections::IEnumerator *>(audioClipAsync);

    if (audioClipRequest->GetError() != UnityEngine::Networking::UnityWebRequest::UnityWebRequestError::OK) {
        getLogger().error("UnityWebRequestMultimedia for file '{}' failed with error {}", loadPath, (int)audioClipRequest->GetError());
        co_return;
    }
    else {
        while (!audioClipRequest->isDone);
        audioClipCompleted();
    }
    co_return;
}

void QuestSounds::Utils::AsyncAudioClipLoader::audioClipCompleted()
{
    // Stage 2
    getLogger().info("Starting Stage 2");
    UnityEngine::AudioClip* temporaryClip = nullptr;
    if (!this->UsesUWR) {
        getLogger().info("UsesUWR is false, getting result from audioClipTask");
        getLogger().debug("Is Task null? {}", fmt::ptr(audioClipTask));
        getLogger().debug("Task Status is {}", (int)audioClipTask->get_Status());
        temporaryClip = this->audioClipTask->get_ResultOnSuccess();
        // gc_free_specific(this->audioClipTask);
        // this->audioClipTask = nullptr;
    }
    else if (this->UsesUWR && audioClipRequest->get_result() == UnityWebRequest::Result::Success){
        getLogger().info("UsesUWR is true, getting result from audioClipRequest");
        getLogger().debug("Is Request null? {}", fmt::ptr(audioClipRequest));
        temporaryClip = UnityEngine::Networking::DownloadHandlerAudioClip::GetContent(audioClipRequest); // TODO: This method takes too long
    }

    if(temporaryClip != nullptr)
    {  
        getLogger().info("TemporaryClip is not nullptr setting it to AudioSource for filepath {}", filePath);
        if (!this->audioClipGO) {
            static ConstString goName = "AudioClipGO";
            audioClipGO = GameObject::New_ctor(goName);
            audioSource = audioClipGO->AddComponent<AudioSource*>();
            UnityEngine::Object::DontDestroyOnLoad(audioClipGO);
        }
        audioSource->set_playOnAwake(false);
        audioSource->set_clip(temporaryClip);
        audioSource->set_volume(0.6f);
        loaded = true;
        getLogger().info("Stage 2 done with temporaryClip");
    }
    else {
        getLogger().error("Stage 2 failed with temporaryClip being nullptr {} Object FilePath: {}", fmt::ptr(temporaryClip), filePath.c_str());
        if (!UsesUWR) getLogger().error("Task Status was {}", (int)audioClipTask->get_Status());
        return;
    } // Finished
}

UnityEngine::AudioClip* QuestSounds::Utils::AsyncAudioClipLoader::getClip()
{
    if(audioSource != nullptr && loaded)
    {
        getLogger().debug("AudioClip returned");
        return audioSource->get_clip();
    } else
    {
        getLogger().debug("nullptr returned: is AudioSource null: {}, is loaded: {}", fmt::ptr(audioSource), loaded);
        return nullptr;
    }
}

void QuestSounds::Utils::AsyncAudioClipLoader::set_OriginalClip(AudioClip* OriginalAudioClip)
{
    // Stage 1
    if (OriginalAudioClip != nullptr)
    {
        static auto goName = "OrigAudioClipGO";
        GameObject* audioClipGO = GameObject::New_ctor(goName);
        OriginalAudioSource = audioClipGO->AddComponent<AudioSource*>();
        OriginalAudioSource->set_playOnAwake(false);
        OriginalAudioSource->set_clip(OriginalAudioClip);
        UnityEngine::Object::DontDestroyOnLoad(audioClipGO);
        getLogger().debug("Saved Original AudioClip");
    }
    // Finished
}

UnityEngine::AudioClip* QuestSounds::Utils::AsyncAudioClipLoader::get_OriginalClip()
{
    if (OriginalAudioSource != nullptr)
    {
        getLogger().debug("OrignalAudioClip returned");
        return OriginalAudioSource->get_clip();
    }
    else
    {
        getLogger().debug("nullptr returned: is OriginalAudioSource null: {}", fmt::ptr(OriginalAudioSource));
        return nullptr;
    }
}