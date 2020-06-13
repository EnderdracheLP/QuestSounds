#include "../include/audiocliploader.hpp"

bool audioClipLoader::loader::load()
{
    //TODO figure out how to get logging working here!
    //Stage 0 
    bool fileError = fileexists(filePath.c_str());
    bool error = (audioClipAsync != nullptr || loadedClip != nullptr || !fileexists);
    if(error)
    {
        if(!fileError)
        {
            // log file doesn't exist error!
        } else 
        {
            // Log another error!
        }
        return false;
    }

    //Stage 1
    Il2CppString* filePathStr = il2cpp_utils::createcsstr(filePath);
    audioClipRequest = CRASH_UNLESS(il2cpp_utils::RunMethod("UnityEngine.Networking", "UnityWebRequestMultimedia", "GetAudioClip", filePath, audioType));
    audioClipAsync = CRASH_UNLESS(il2cpp_utils::RunMethod(webRequest, "SendWebRequest"));
    
    //Stage 2
    const MethodInfo* addCompletedMethod = CRASH_UNLESS(il2cpp_utils::FindMethodUnsafe(audioClipAsync, "add_completed", 1));
    auto action = CRASH_UNLESS(il2cpp_utils::MakeAction(addCompletedMethod, 0, this, audioClipCompleted));
    CRASH_UNLESS(il2cpp_utils::RunMethod(audioClipAsync, completedMethod, action));
    return true;
}

void audioClipLoader::loader::audioClipCompleted(loader* obj, Il2CppObject* asyncOp)
{
    // Stage 1
    obj->loadedClip = CRASH_UNLESS(il2cpp_utils::RunMethod("UnityEngine.Networking", "DownloadHandlerAudioClip", "GetContent", obj->audioClipRequest));
    if(obj->loadedClip != nullptr)
    {
        obj->loaded = true;
    }
    // Finished
}


