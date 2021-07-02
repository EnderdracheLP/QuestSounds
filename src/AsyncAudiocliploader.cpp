#include "QSoundsConfig.hpp"
#include "AsyncAudiocliploader.hpp"
#include "main.hpp"
#include "ObjectInstances.hpp"

#include "System/Collections/IEnumerator.hpp"
#include "custom-types/shared/coroutine.hpp"

#include <dirent.h>
#include <regex>

#include <chrono>

//using namespace audioClipLoader;

#include "UnityEngine\Networking\DownloadHandlerAudioClip.hpp"
#include "UnityEngine\Networking\UnityWebRequestMultimedia.hpp"
using namespace UnityEngine::Networking;

#include "System/Threading/Tasks/Task.hpp"
#include "System/Threading/Tasks/Task_1.hpp"
#include "System/Action_1.hpp"
using namespace System;

#include "UnityEngine/AsyncOperation.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/MonoBehaviour.hpp" // Added incase it's somehow needed
using namespace UnityEngine;

#include "GlobalNamespace/SharedCoroutineStarter.hpp"

//custom_types::Helpers::Coroutine AsyncAudioClipLoader::loader::GetAudioClip(System::Action_1<UnityEngine::AsyncOperation*>* actionUWRM, AsyncAudioClipLoader::loader* self, int audioType, Il2CppString* filePathStr) {
custom_types::Helpers::Coroutine AsyncAudioClipLoader::loader::GetAudioClip(AsyncAudioClipLoader::loader* self, int audioType, Il2CppString* filePathStr/*, Logger& logger*/) {
    //logger.debug("Coroutine start");
    self->audioClipRequest = UnityEngine::Networking::UnityWebRequestMultimedia::GetAudioClip(filePathStr, audioType);
    {
        //logger.debug("audioClipRequest set");

        co_yield reinterpret_cast<System::Collections::IEnumerator*>(self->audioClipAsync = self->audioClipRequest->SendWebRequest());

        if (self->audioClipRequest->get_isHttpError() || self->audioClipRequest->get_isNetworkError()) {

            //logger.error("Web Request Error: %s", to_utf8(csstrtostr(self->audioClipRequest->get_error())).c_str());
            
            co_return;
        }
        else {
            //Stage 2
            //logger.info("Coroutine add_completed method");
            //self->audioClipAsync->add_completed(actionUWRM);
            AudioClip* temporaryClip = nullptr;
            co_yield reinterpret_cast<System::Collections::IEnumerator*>(temporaryClip = DownloadHandlerAudioClip::GetContent(self->audioClipRequest)); // TODO: This method takes too long

            static auto goName = il2cpp_utils::createcsstr("AudioClipGO", il2cpp_utils::StringType::Manual);
            GameObject* audioClipGO = GameObject::New_ctor(goName);
            self->audioSource = audioClipGO->AddComponent<AudioSource*>();
            self->audioSource->set_playOnAwake(false);
            self->audioSource->set_clip(temporaryClip);
            UnityEngine::Object::DontDestroyOnLoad(audioClipGO);
            self->loaded = true;

            co_return;
        }
        //co_return;

    }
}


    //int getAudioType(std::string path) {
    //if (path.ends_with(".ogg")) {
    //    getLogger().debug("File is ogg");
    //    return 0xE;
    //} else if (path.ends_with(".wav")) {
    //    getLogger().debug("File is wav");
    //    return 0x14;
    //} else if (path.ends_with(".mp3")) {
    //    getLogger().debug("File is mp3");
    //    return 0xD;
    //}
    //return 0;
    //}

int getAudioType(std::string path) {
    //if (path.ends_with(".ogg")) {
    //    getLogger().debug("File is ogg");
    //    return 0xE;
    //}
    /*else*/ if (path.ends_with(".wav")) {
        getLogger().debug("File is wav");
        return 0x14;
    }
    else if (path.ends_with(".mp3")) {
        getLogger().debug("File is mp3");
        return 0xD;
    }
    else if (path.ends_with(".aiff") || path.ends_with(".aif")) {
        getLogger().debug("File is aiff");
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
    //if (filePath.starts_with(QSoundsConfig::HitSoundPath) || filePath.starts_with(QSoundsConfig::BadHitSoundPath) || !filePath.ends_with(".ogg")) {
        getLogger().debug("Running UnityWebRequestMultimedia for FilePath %s", filePath.c_str());
        if (filePath.starts_with("https://") || filePath.starts_with("http://")) {
            filePathStr = il2cpp_utils::newcsstr(filePath);
        }
        else {
            filePathStr = il2cpp_utils::newcsstr("file:///" + filePath);
        }
        //audioClipRequest = UnityEngine::Networking::UnityWebRequestMultimedia::GetAudioClip(filePathStr, audioType);
        //audioClipAsync = audioClipRequest->SendWebRequest();
        //                                 ^ 
        // TODO: Figure out why it crashes here sometimes
        audioType = getAudioType(filePath);

        auto actionUWRM = il2cpp_utils::MakeDelegate<System::Action_1<UnityEngine::AsyncOperation*>*>(classof(System::Action_1<UnityEngine::AsyncOperation*>*), this, audioClipCompleted);
        auto start = std::chrono::high_resolution_clock::now();
        //GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(AsyncAudioClipLoader::loader::GetAudioClip(actionUWRM, this, audioType, filePathStr))));
        GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(AsyncAudioClipLoader::loader::GetAudioClip(this, audioType, filePathStr/*, getLogger()*/))));
        auto end = std::chrono::high_resolution_clock::now();
        double time_taken = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        time_taken *= 1e-9;
        getLogger().debug("UnityWebRequestMultimedia: Time taken %ld", time_taken);

    //}
    //else {
    //    getLogger().debug("Running MediaAsyncLoader for FilePath");
    //    filePathStr = il2cpp_utils::newcsstr(filePath);
    //    audioClipTask = GlobalNamespace::MediaAsyncLoader::LoadAudioClipAsync(filePathStr, System::Threading::CancellationToken::get_None());
    //    //              ^
    //    // Why is that sometimes a nullptr when first loading
    //    ////Stage 2
    //    auto actionMAL = il2cpp_utils::MakeDelegate<System::Action_1<System::Threading::Tasks::Task*>*>(classof(System::Action_1<System::Threading::Tasks::Task*>*), this, audioClipCompleted);
    //    reinterpret_cast<System::Threading::Tasks::Task*>(audioClipTask)->ContinueWith(actionMAL);
    //}
    getLogger().debug("Stage 2 done with filepath %s", filePath.c_str());
    return true;
}

void AsyncAudioClipLoader::loader::audioClipCompleted(loader* obj, Il2CppObject* asyncOp)
{
    auto start = std::chrono::high_resolution_clock::now();

    // Stage 1
    //getLogger().debug("isOGG %d", obj->isOGG);
    UnityEngine::AudioClip* temporaryClip = obj->audioClipTask->get_ResultOnSuccess();
    //if (obj->isOGG) temporaryClip = obj->audioClipTask->get_ResultOnSuccess();
    //else temporaryClip = UnityEngine::Networking::DownloadHandlerAudioClip::GetContent(obj->audioClipRequest); // TODO: This method takes too long

    if(temporaryClip != nullptr)
    {  
        static auto goName = il2cpp_utils::createcsstr("AudioClipGO", il2cpp_utils::StringType::Manual);
        GameObject* audioClipGO = GameObject::New_ctor(goName);
        obj->audioSource = audioClipGO->AddComponent<AudioSource*>();
        obj->audioSource->set_playOnAwake(false);
        obj->audioSource->set_clip(temporaryClip);
        UnityEngine::Object::DontDestroyOnLoad(audioClipGO);
        obj->loaded = true;
        getLogger().debug("Stage 1 done with temporaryClip");
    }
    auto end_final = std::chrono::high_resolution_clock::now();
    double time_taken_Total = std::chrono::duration_cast<std::chrono::nanoseconds>(end_final - start).count();
    time_taken_Total *= 1e-9;
    getLogger().debug("Time taken for audioClipCompleted %lf", time_taken_Total);


    // Finished
}

UnityEngine::AudioClip* AsyncAudioClipLoader::loader::getClip()
{
    if(audioSource != nullptr && loaded)
    {
        getLogger().debug("AudioClip returned");
        return audioSource->get_clip();
    } else
    {
        getLogger().debug("nullptr returned: is AudioSource null: %d, is loaded: %d", audioSource, loaded);
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
        getLogger().debug("nullptr returned: is OriginalAudioSource null: %d", OriginalAudioSource);
        return nullptr;
    }
}