#include "main.hpp"
#include "audiocliploader.hpp"

#include "UnityEngine\Networking\DownloadHandlerAudioClip.hpp"
#include "UnityEngine\Networking\UnityWebRequestMultimedia.hpp"
using namespace UnityEngine::Networking;

#include "UnityEngine/GameObject.hpp"
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


    audioType = getAudioType(filePath);
    audioClipRequest = filePathStr->GetAudioClip(audioType);
    //audioClipRequest = CRASH_UNLESS(il2cpp_utils::RunMethod("UnityEngine.Networking", "UnityWebRequestMultimedia", "GetAudioClip", filePathStr, audioType));
    audioClipAsync = audioClipRequest->SendWebRequest();
    //audioClipAsync = CRASH_UNLESS(il2cpp_utils::RunMethod(audioClipRequest, "SendWebRequest"));
    
    //Stage 2
    //const MethodInfo* addCompletedMethod = CRASH_UNLESS(il2cpp_utils::FindMethodUnsafe(audioClipAsync, "add_completed", 1));
    const MethodInfo* addCompletedMethod = CRASH_UNLESS(il2cpp_utils::FindMethodUnsafe(audioClipAsync, "add_completed", 1));
    auto action = CRASH_UNLESS(il2cpp_utils::MakeDelegate(addCompletedMethod, 0, (Il2CppObject*)this, audioClipCompleted));
    action->addCompletedMethod;
    //CRASH_UNLESS(il2cpp_utils::RunMethod(audioClipAsync, addCompletedMethod, action));
    getLogger().debug("Stage 2 done");
    return true;
}

void audioClipLoader::loader::audioClipCompleted(loader* obj, Il2CppObject* asyncOp)
{
    // Stage 1
    temporaryClip = obj->audioClipRequest->GetContent();
    //Il2CppObject* temporaryClip = CRASH_UNLESS(il2cpp_utils::RunMethod("UnityEngine.Networking", "DownloadHandlerAudioClip", "GetContent", obj->audioClipRequest));
    if(temporaryClip != nullptr)
    {  
        auto audioClipGO = UnityEngine::GameObject::New_ctor(il2cpp_utils::createcsstr("AudioClipGO"));
        //Il2CppString* goName = il2cpp_utils::createcsstr("AudioClipGO");
        //Il2CppObject* audioClipGO = CRASH_UNLESS(il2cpp_utils::NewUnsafe(il2cpp_utils::GetClassFromName("UnityEngine", "GameObject"), goName));
        obj->audioSource = CRASH_UNLESS(il2cpp_utils::RunMethod(audioClipGO, "AddComponent", il2cpp_utils::GetSystemType("UnityEngine", "AudioSource")));
        CRASH_UNLESS(il2cpp_utils::SetPropertyValue(obj->audioSource, "playOnAwake", false));
        CRASH_UNLESS(il2cpp_utils::SetPropertyValue(obj->audioSource, "clip", temporaryClip));
        CRASH_UNLESS(il2cpp_utils::RunMethod(audioClipGO, "DontDestroyOnLoad", audioClipGO));
        obj->loaded = true;
        getLogger().debug("Stage 1 done");
    }
    // Finished
}

Il2CppObject* audioClipLoader::loader::getClip()
{
    if(audioSource != nullptr && loaded)
    {
        return CRASH_UNLESS(il2cpp_utils::GetPropertyValue(audioSource, "clip"));
    } else
    {
        return nullptr;
    }
}


