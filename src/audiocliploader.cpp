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
    bool fileError = fileexists(filePath.c_str());
    bool error = (audioClipAsync != nullptr || audioSource != nullptr || !fileError);
    if(error)
    {
        if(!fileError)
        {
            getLogger().error("File Error");
        } else 
        {
            getLogger().info("File Loaded");
        }
        getLogger().info("Stage 0 Done");
        return false;
    }

    //Stage 1
    Il2CppString* filePathStr = il2cpp_utils::createcsstr("file:///" + filePath);
    //std::string filePathStr = "file:///" + filePath;

    //reinterpret_cast<type*>(instance)
    audioType = getAudioType(filePath);
    audioClipRequest = UnityEngine::Networking::UnityWebRequestMultimedia::GetAudioClip(filePathStr, audioType);
    //audioClipRequest = CRASH_UNLESS(il2cpp_utils::RunMethod("UnityEngine.Networking", "UnityWebRequestMultimedia", "GetAudioClip", filePathStr, audioType));
    audioClipAsync = audioClipRequest->SendWebRequest();
    //audioClipAsync = CRASH_UNLESS(il2cpp_utils::RunMethod(audioClipRequest, "SendWebRequest"));
    
    //Stage 2
    //const MethodInfo* addCompletedMethod = CRASH_UNLESS(il2cpp_utils::FindMethodUnsafe(audioClipAsync, "add_completed", 1));
    //auto action = CRASH_UNLESS(il2cpp_utils::MakeDelegate(addCompletedMethod, 0, (Il2CppObject*)this, audioClipCompleted));
    auto action = il2cpp_utils::MakeDelegate<System::Action_1<UnityEngine::AsyncOperation*>*>(classof(System::Action_1<UnityEngine::AsyncOperation*>*), this, audioClipCompleted);
    
    audioClipAsync->add_completed(action);
    //CRASH_UNLESS(il2cpp_utils::RunMethod(audioClipAsync, addCompletedMethod, action));
    getLogger().debug("Stage 2 done");
    return true;
}

void audioClipLoader::loader::audioClipCompleted(loader* obj, Il2CppObject* asyncOp)
{
    // Stage 1
    UnityEngine::AudioClip* temporaryClip = UnityEngine::Networking::DownloadHandlerAudioClip::GetContent(obj->audioClipRequest);
    //Il2CppObject* temporaryClip = CRASH_UNLESS(il2cpp_utils::RunMethod("UnityEngine.Networking", "DownloadHandlerAudioClip", "GetContent", obj->audioClipRequest));
    if(temporaryClip != nullptr)
    {  
        static auto goName = il2cpp_utils::createcsstr("AudioClipGO", il2cpp_utils::StringType::Manual);
        GameObject* audioClipGO = GameObject::New_ctor(goName);
        //auto audioClipGO = UnityEngine::GameObject::New_ctor(il2cpp_utils::createcsstr("AudioClipGO"));
        //Il2CppString* goName = il2cpp_utils::createcsstr("AudioClipGO");
        //Il2CppObject* audioClipGO = CRASH_UNLESS(il2cpp_utils::NewUnsafe(il2cpp_utils::GetClassFromName("UnityEngine", "GameObject"), goName));
        obj->audioSource = audioClipGO->AddComponent<AudioSource*>();
        //obj->audioSource = CRASH_UNLESS(il2cpp_utils::RunMethod(audioClipGO, "AddComponent", il2cpp_utils::GetSystemType("UnityEngine", "AudioSource")));
        obj->audioSource->set_playOnAwake(false);
        //CRASH_UNLESS(il2cpp_utils::SetPropertyValue(obj->audioSource, "playOnAwake", false));
        obj->audioSource->set_clip(temporaryClip);
        //CRASH_UNLESS(il2cpp_utils::SetPropertyValue(obj->audioSource, "clip", temporaryClip));
        //CRASH_UNLESS(il2cpp_utils::RunMethod(audioClipGO, "DontDestroyOnLoad", audioClipGO));
        UnityEngine::Object::DontDestroyOnLoad(audioClipGO);
        obj->loaded = true;
        getLogger().debug("Stage 1 done");
    }
    // Finished
}

UnityEngine::AudioClip* audioClipLoader::loader::getClip()
{
    if(audioSource != nullptr && loaded)
    {
        //return CRASH_UNLESS(il2cpp_utils::GetPropertyValue(audioSource, "clip"));
        return audioSource->get_clip();
    } else
    {
        return nullptr;
    }
}


