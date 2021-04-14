#include "audiocliploader.hpp"
#include "main.hpp"
//using namespace audioClipLoader;

#include "UnityEngine\Networking\DownloadHandlerAudioClip.hpp"
#include "UnityEngine\Networking\UnityWebRequestMultimedia.hpp"
using namespace UnityEngine::Networking;

#include "System/Action_1.hpp"
using namespace System;

#include "UnityEngine/AsyncOperation.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/MonoBehaviour.hpp" // Added incase it's somehow needed
using namespace UnityEngine;

    int getAudioType(std::string path) {
    if (path.ends_with(".ogg")) {
        getLogger().debug("File is ogg");
        return 0xE;
    } else if (path.ends_with(".wav")) {
        getLogger().debug("File is wav");
        return 0x14;
    } else if (path.ends_with(".mp3")) {
        getLogger().debug("File is mp3");
        return 0xD;
    }
    return 0;
    }

bool audioClipLoader::loader::load()
{
    //Stage 0 
    getLogger().info("Starting Stage 0");
    getLogger().info("FilePath to check is %s", filePath.c_str());
    Il2CppString* filePathStr;
    if (filePath.starts_with("https://") || filePath.starts_with("http://")) {
        getLogger().info("Filepath is URL, skipping file checks and try loading it");
        filePathStr = il2cpp_utils::createcsstr(filePath);
    }
    else {
        bool fileError = fileexists(filePath.c_str());
        getLogger().info("File error is %d", fileError);
        bool error = (audioClipAsync != nullptr || audioSource != nullptr || !fileError);
        getLogger().info("error is %d", error);
        if (error)
        {
            if (!fileError) // If fileError is null or false?
            {
                getLogger().error("File %s not found", filePath.c_str());
            }
            else
            {
                getLogger().error("Something went wrong when trying to load", filePath.c_str());
            }
            getLogger().error("Stage 0 Failed");
            return false;
        }
        else { getLogger().info("Stage 0 Done with %s", filePath.c_str()); }
        filePathStr = il2cpp_utils::createcsstr("file:///" + filePath);
    }

    //Stage 1

    audioType = getAudioType(filePath);
    audioClipRequest = UnityEngine::Networking::UnityWebRequestMultimedia::GetAudioClip(filePathStr, audioType);
    audioClipAsync = audioClipRequest->SendWebRequest();
    
    //Stage 2
    auto action = il2cpp_utils::MakeDelegate<System::Action_1<UnityEngine::AsyncOperation*>*>(classof(System::Action_1<UnityEngine::AsyncOperation*>*), this, audioClipCompleted);
    
    audioClipAsync->add_completed(action);
    getLogger().debug("Stage 2 done with filepath %s", filePath.c_str());
    //getLogger().debug("Stage 2 done with filepath");
    return true;
}

void audioClipLoader::loader::audioClipCompleted(loader* obj, Il2CppObject* asyncOp)
{
    // Stage 1
    UnityEngine::AudioClip* temporaryClip = UnityEngine::Networking::DownloadHandlerAudioClip::GetContent(obj->audioClipRequest);
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
    // Finished
}

UnityEngine::AudioClip* audioClipLoader::loader::getClip()
{
    if(audioSource != nullptr && loaded)
    {
        return audioSource->get_clip();
    } else
    {
        return nullptr;
    }
}


