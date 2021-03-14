#include "main.hpp"
#include "audiocliploader.hpp"
#include <unistd.h>

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

    //bool doesfileexist(std::string_view filename) {
    //    int accessStr = access(filename.data(), X_OK);
    //    getLogger().debug("Root Access %d", accessStr);
    //    if (access(filename.data(), X_OK) != -1) {
    //        return true;
    //    } else if (access(filename.data(), W_OK | R_OK) != -1) {
    //        int accessStr = access(filename.data(), W_OK | R_OK);
    //        getLogger().debug("Normal Read Write Access %d", accessStr);
    //        return true;
    //    } else return false;
    //}

bool audioClipLoader::loader::load()
{
    //Stage 0 
    getLogger().info("Starting Stage 0");
    getLogger().info("FilePath to check is %s", filePath.c_str());
    //bool fileError = doesfileexist(filePath);
    bool fileError = fileexists(filePath);
    getLogger().info("File error is %d", fileError);
    //bool fileError = true;
    bool error = (audioClipAsync != nullptr || audioSource != nullptr /*|| !fileError*/);
    getLogger().info("error is %s", error);
    if(error)
    {
        if(!fileError) // If fileError is null or false?
        {
            getLogger().error("Found file but Web Request for file %s failed", filePath.c_str());
            if (audioClipAsync == nullptr) {
                getLogger().error("audioClipAsync is null");
            }
            if (audioSource == nullptr) {
                getLogger().error("audioSource is null");
            }
        } else 
        {
            getLogger().error("fileError file %s not found", filePath.c_str());
        }
        getLogger().error("Stage 0 Failed");
        return false;
    }
    getLogger().info("Stage 0 Done");

    //Stage 1
    Il2CppString* filePathStr = il2cpp_utils::createcsstr("file:///" + filePath);


    audioType = getAudioType(filePath);
    audioClipRequest = CRASH_UNLESS(il2cpp_utils::RunMethod("UnityEngine.Networking", "UnityWebRequestMultimedia", "GetAudioClip", filePathStr, audioType));
    audioClipAsync = CRASH_UNLESS(il2cpp_utils::RunMethod(audioClipRequest, "SendWebRequest"));
    
    //Stage 2
    const MethodInfo* addCompletedMethod = CRASH_UNLESS(il2cpp_utils::FindMethodUnsafe(audioClipAsync, "add_completed", 1));
    auto action = CRASH_UNLESS(il2cpp_utils::MakeDelegate(addCompletedMethod, 0, (Il2CppObject*)this, audioClipCompleted));
    CRASH_UNLESS(il2cpp_utils::RunMethod(audioClipAsync, addCompletedMethod, action));
    getLogger().debug("Stage 2 done");
    return true;
}

void audioClipLoader::loader::audioClipCompleted(loader* obj, Il2CppObject* asyncOp)
{
    // Stage 1
    Il2CppObject* temporaryClip = CRASH_UNLESS(il2cpp_utils::RunMethod("UnityEngine.Networking", "DownloadHandlerAudioClip", "GetContent", obj->audioClipRequest));
    if(temporaryClip != nullptr)
    {  
        Il2CppString* goName = il2cpp_utils::createcsstr("AudioClipGO");
        Il2CppObject* audioClipGO = CRASH_UNLESS(il2cpp_utils::NewUnsafe(il2cpp_utils::GetClassFromName("UnityEngine", "GameObject"), goName));
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
        getLogger().debug("getClip nullptr");
        return nullptr;
    }
}