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
     /*
    std::string CheckFilePath(std::string PathToCheck) {
        std::string result;
            // Checks if the given File in the config exists
            bool fileError = fileexists(PathToCheck);
            // If that file doesn't exist check if the same file with an .mp3 file extension exists
            if (!fileError) {
                fileError = fileexists(PathToCheck.replace(PathToCheck.length() - 3, 3, "mp3"));
                if (fileError) {
                    result = PathToCheck.replace(PathToCheck.length() - 3, 3, "mp3");
                    getLogger().warning("Could not find file set in Config, but found file with mp3 extension: %s", result.c_str());
                    return result;
                }
                else {
                    getLogger().error("Could not find file with mp3 extension, checking for (1), (2) in filename");
                    fileError = fileexists(PathToCheck.insert(PathToCheck.length() - 4, " (1)"));
                    if (fileError) {
                        result = PathToCheck.insert(PathToCheck.length() - 4, " (1)");
                        return result;
                    }
                    else {
                        PathToCheck = PathToCheck.replace(PathToCheck.length() - 3, 3, "mp3");
                        if (fileError) {
                            result = PathToCheck.insert(PathToCheck.length() - 4, " (1)")
                        }
                    }
                }
            }
            return PathToCheck
        } // */

bool audioClipLoader::loader::load()
{
    //Stage 0 
    getLogger().info("Starting Stage 0");
    getLogger().info("FilePath to check try 1 is %s", filePath.c_str());
    Il2CppString* filePathStr;
    std::string CheckPath = filePath;
    if (filePath.starts_with("https://") || filePath.starts_with("http://")) {
        getLogger().info("Filepath is URL, skipping file checks and try loading it");
        filePathStr = il2cpp_utils::createcsstr(filePath);
    } else {
        // Checks if the given File in the config exists
        bool fileError = fileexists(filePath);
        // If that file doesn't exist check if the same file with an .mp3 file extension exists
        if (!fileError) {
            fileError = fileexists(CheckPath.replace(CheckPath.length() - 3, 3, "mp3"));
            getLogger().info("FilePath to check try 2 is %s", CheckPath.c_str());
            if (fileError) {
                filePath = CheckPath;
                getLogger().warning("Could not find file set in Config, but found file with mp3 extension: %s", filePath.c_str());
            }
            else {
                getLogger().error("Could not find file with mp3 extension, checking for (1) in filename");
                fileError = fileexists(CheckPath.insert(CheckPath.length() - 4, " (1)"));
                getLogger().info("FilePath to check try 3 is %s", CheckPath.c_str());
                if (fileError) {
                    filePath = CheckPath;
                    getLogger().warning("Found mp3 file with (1) in name: %s", filePath.c_str());
                }
                else {
                    fileError = fileexists(CheckPath.replace(CheckPath.length() - 3, 3, "ogg"));
                    getLogger().info("FilePath to check try 4 is %s", CheckPath.c_str());
                    if (fileError) {
                        filePath = CheckPath;
                        getLogger().warning("Found file with (1) in name: %s", filePath.c_str());
                    }
                    else { getLogger().error("All checks failed, no soundfile found"); }
                }
            }
        }
        getLogger().info("File error is %d", fileError);
        //bool fileError = true;
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
                getLogger().error("fileError file %s not found", filePath.c_str());
            }
            getLogger().error("Stage 0 Failed");
            return false;
        }
        filePathStr = il2cpp_utils::createcsstr("file:///" + filePath);
    }
    getLogger().info("Stage 0 Done with %s", filePath.c_str());

    //Stage 1
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