#include "QSoundsConfig.hpp"
#include "Utils/AsyncAudiocliploader.hpp"
#include "main.hpp"
#include "ObjectInstances.hpp"

#include "System/Collections/IEnumerator.hpp"
#include "custom-types/shared/coroutine.hpp"

#include <dirent.h>
#include <regex>

#include <chrono>

//using namespace audioClipLoader;

#include "UnityEngine/Networking/DownloadHandlerAudioClip.hpp"
#include "UnityEngine/Networking/UnityWebRequestMultimedia.hpp"
using namespace UnityEngine::Networking;

#include "System/Threading/Tasks/TaskStatus.hpp"
#include "System/Threading/Tasks/Task.hpp"
#include "System/Threading/Tasks/Task_1.hpp"
#include "System/Action_1.hpp"
using namespace System;

#include "UnityEngine/AsyncOperation.hpp"
#include "UnityEngine/GameObject.hpp"
using namespace UnityEngine;

#include "GlobalNamespace/SharedCoroutineStarter.hpp"

custom_types::Helpers::Coroutine AsyncAudioClipLoader::loader::GetAudioClip(System::Action_1<UnityEngine::AsyncOperation*>* actionUWRM, AsyncAudioClipLoader::loader* self, int audioType, Il2CppString* filePathStr/*, Logger& logger*/) {
    audioClipRequest = UnityEngine::Networking::UnityWebRequestMultimedia::GetAudioClip(filePathStr, audioType);
    {
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(audioClipAsync = audioClipRequest->SendWebRequest());
        audioClipAsync->set_allowSceneActivation(false);

        if (audioClipRequest->get_isHttpError() || audioClipRequest->get_isNetworkError()) {            
            co_return;
        }
        else {
            //Stage 2
            audioClipAsync->add_completed(actionUWRM);
            co_return;
        }
    }
}

int getAudioType(std::string path) {
    if (path.ends_with(".ogg")) {
        getLogger().debug("File is OGGVORBIS");
        return 0xE;
    }
    else if (path.ends_with(".mp3") || path.ends_with(".mp2")) {
        getLogger().debug("File is MPEG");
        return 0xD;
    }
    else if (path.ends_with(".m4a")) {
        getLogger().debug("File is AAC");
        return 1;
    }
    else if (path.ends_with(".wav")) {
        getLogger().debug("File is WAV");
        return 0x14;
    }
    else if (path.ends_with(".aiff") || path.ends_with(".aif")) {
        getLogger().debug("File is AIFF");
        return 2;
    }
    return 0;
}

System::Threading::Tasks::Task_1<UnityEngine::AudioClip*>* audioClipTask;
//bool NeedReload = false;

bool AsyncAudioClipLoader::loader::load()
{
    loaded = false;
    //Stage 0 
    static const char* FileExtensions[] =
    { "mp3","wav","aiff","aif"};
    bool fileError;
    getLogger().info("Starting Stage 0");
    getLogger().info("FilePath to check is %s", filePath.c_str());
    Il2CppString* filePathStr;
    if (filePath.starts_with("https://") || filePath.starts_with("http://")) {
        getLogger().info("Filepath is URL, skipping file checks and try loading it");
        fileError = true;
    }
    else {
        // Checks if the given File in the config exists and
        fileError = fileexists(filePath);
        getLogger().info("File error is %d", fileError);
        if (!fileError)
        {
            getLogger().error("File %s not found", filePath.c_str());
            getLogger().error("Stage 0 Failed");
            return false;
        }
        else { getLogger().info("Stage 0 Done with %s", filePath.c_str()); }
    }

    //Stage 1
    if (!(filePath == QSoundsConfig::Config.hitSound_filepath) && filePath.ends_with(".ogg")) {
        getLogger().info("Stage 1: Running MediaAsyncLoader for FilePath %s", filePath.c_str());
        UsesUWR = false;
        filePathStr = il2cpp_utils::newcsstr(filePath);
        audioClipTask = GlobalNamespace::MediaAsyncLoader::LoadAudioClipAsync(filePathStr, System::Threading::CancellationToken::get_None());
        //              ^
        //      Known crash there
        ////Stage 2
        auto actionMAL = il2cpp_utils::MakeDelegate<System::Action_1<System::Threading::Tasks::Task*>*>(classof(System::Action_1<System::Threading::Tasks::Task*>*), this, audioClipCompleted);
        reinterpret_cast<System::Threading::Tasks::Task*>(audioClipTask)->ContinueWith(actionMAL);
    }
    else {
        getLogger().info("Stage 1: Running UnityWebRequestMultimedia for FilePath %s", filePath.c_str());
        UsesUWR = true;
        if (filePath.starts_with("https://") || filePath.starts_with("http://")) {
            filePathStr = il2cpp_utils::newcsstr(filePath);
        }
        else {
            filePathStr = il2cpp_utils::newcsstr("file:///" + filePath);
        }
        audioType = getAudioType(filePath);

        auto actionUWRM = il2cpp_utils::MakeDelegate<System::Action_1<UnityEngine::AsyncOperation*>*>(classof(System::Action_1<UnityEngine::AsyncOperation*>*), this, audioClipCompleted);
        //                ^
        // Got a crash there somehow
        auto start = std::chrono::high_resolution_clock::now();
        //GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(AsyncAudioClipLoader::loader::GetAudioClip(actionUWRM, this, audioType, filePathStr))));
        GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(AsyncAudioClipLoader::loader::GetAudioClip(actionUWRM, this, audioType, filePathStr))));
        //                                                       ^                                                                  ^
        //                                            TODO: Why did it crash there                                                or here
        auto end = std::chrono::high_resolution_clock::now();
        double time_taken = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        time_taken *= 1e-9;
        getLogger().debug("UnityWebRequestMultimedia: Time taken %f", time_taken);
    }
    getLogger().info("Stage 1 done with filepath %s", filePath.c_str());
    return true;
}

void AsyncAudioClipLoader::loader::audioClipCompleted(loader* obj, Il2CppObject* asyncOp)
{
    // Stage 1
    UnityEngine::AudioClip* temporaryClip = nullptr;
    if (!obj->UsesUWR) temporaryClip = obj->audioClipTask->get_ResultOnSuccess();
    else if (obj->UsesUWR) temporaryClip = UnityEngine::Networking::DownloadHandlerAudioClip::GetContent(obj->audioClipRequest); // TODO: This method takes too long
    if (temporaryClip == nullptr && !obj->UsesUWR) {
        Il2CppString* filePathStr = il2cpp_utils::newcsstr("file://" + obj->filePath);
        obj->audioClipTask = GlobalNamespace::MediaAsyncLoader::LoadAudioClipAsync(filePathStr, System::Threading::CancellationToken::get_None());
        ////Stage 2
        auto actionMAL = il2cpp_utils::MakeDelegate<System::Action_1<System::Threading::Tasks::Task*>*>(classof(System::Action_1<System::Threading::Tasks::Task*>*), obj, audioClipCompleted);
        reinterpret_cast<System::Threading::Tasks::Task*>(obj->audioClipTask)->ContinueWith(actionMAL);
    }

    if(temporaryClip != nullptr)
    {  
        static auto goName = il2cpp_utils::createcsstr("AudioClipGO", il2cpp_utils::StringType::Manual);
        GameObject* audioClipGO = GameObject::New_ctor(goName);
        obj->audioSource = audioClipGO->AddComponent<AudioSource*>();
        obj->audioSource->set_playOnAwake(false);
        obj->audioSource->set_clip(temporaryClip);
        UnityEngine::Object::DontDestroyOnLoad(audioClipGO);
        obj->audioSource->set_volume(0.6f);
        obj->loaded = true;
        getLogger().info("Stage 2 done with temporaryClip");
    }
    else {
        getLogger().error("Stage 2 failed with temporaryClip being nullptr %p", temporaryClip);
        if (!obj->UsesUWR) getLogger().error("Task Status was %d", (int)obj->audioClipTask->get_Status());
        return;
    } // Finished
}

UnityEngine::AudioClip* AsyncAudioClipLoader::loader::getClip()
{
    if(audioSource != nullptr && loaded)
    {
        getLogger().debug("AudioClip returned");
        return audioSource->get_clip();
    } else
    {
        getLogger().debug("nullptr returned: is AudioSource null: %p, is loaded: %d", audioSource, loaded);
        return nullptr;
    }
}

void AsyncAudioClipLoader::loader::set_OriginalClip(AudioClip* OriginalAudioClip)
{
    // Stage 1
    if (OriginalAudioClip != nullptr)
    {
        static auto goName = il2cpp_utils::createcsstr("OrigAudioClipGO", il2cpp_utils::StringType::Manual);
        GameObject* audioClipGO = GameObject::New_ctor(goName);
        OriginalAudioSource = audioClipGO->AddComponent<AudioSource*>();
        OriginalAudioSource->set_playOnAwake(false);
        OriginalAudioSource->set_clip(OriginalAudioClip);
        UnityEngine::Object::DontDestroyOnLoad(audioClipGO);
        getLogger().debug("Saved Original AudioClip");
    }
    // Finished
}

UnityEngine::AudioClip* AsyncAudioClipLoader::loader::get_OriginalClip()
{
    if (OriginalAudioSource != nullptr)
    {
        getLogger().debug("OrignalAudioClip returned");
        return OriginalAudioSource->get_clip();
    }
    else
    {
        getLogger().debug("nullptr returned: is OriginalAudioSource null: %ld", (long int)OriginalAudioSource);
        return nullptr;
    }
}