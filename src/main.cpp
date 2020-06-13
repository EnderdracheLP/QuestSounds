#include <dlfcn.h>
#include "../extern/beatsaber-hook/shared/utils/utils.h"
#include "../extern/beatsaber-hook/shared/utils/logging.hpp"
#include "../extern/beatsaber-hook/include/modloader.hpp"
#include "../extern/beatsaber-hook/shared/utils/typedefs.h"
#include "../extern/beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "../extern/beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "../include/audiocliploader.hpp"

static const Logger *logger;
bool menuScene = false;
audioClipLoader::loader hitSoundLoader0; // hitSound
audioClipLoader::loader hitSoundLoader1; // badHitSound
audioClipLoader::loader musicLoader0;    // menuMusic
Il2CppArray* hitSounds0; // hitSoundArray
Array<Il2CppObject*>* hitSounds1; // badHitSoundArray
bool audioSet;
Il2CppObject *GetFirstObjectOfType(Il2CppClass *klass)
{
    Il2CppReflectionType* klassType = il2cpp_utils::GetSystemType(klass);
    Array<Il2CppObject*>* objects = CRASH_UNLESS(il2cpp_utils::RunMethod<Array<Il2CppObject*>*>(il2cpp_utils::GetClassFromName("UnityEngine", "Resources"), "FindObjectsOfTypeAll", klassType));
    if(objects == nullptr) return nullptr;
    return objects->values[0];
}

MAKE_HOOK_OFFSETLESS(SongPreviewPlayer_OnEnable, void, Il2CppObject* self)
{
    
    //Array<Il2CppObject*>* audioSources = CRASH_UNLESS(il2cpp_utils::GetFieldValue<Array<Il2CppObject*>*>(songPreview, "_audioSources"));
    //CRASH_UNLESS(il2cpp_utils::RunMethod(audioSources->values[activeChannel], "Stop"));
    logger->info("is it true: %i", musicLoader0.loaded);
    if(musicLoader0.loaded)
    {
        Il2CppObject* audioClip = musicLoader0.getClip();
        if(audioClip != nullptr)
            CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "_defaultAudioClip", audioClip));
    }
    SongPreviewPlayer_OnEnable(self);

}

MAKE_HOOK_OFFSETLESS(NoteCutSoundEffectManager_Start, void, Il2CppObject* self)
{
    if(hitSoundLoader0.loaded)
    {
        Il2CppObject* audioClip = hitSoundLoader0.getClip();
        hitSounds0 = (il2cpp_functions::array_new(il2cpp_utils::GetClassFromName("UnityEngine", "AudioClip"), 1));
        il2cpp_array_set(hitSounds0, Il2CppObject*, 0, audioClip);
        CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "_longCutEffectsAudioClips", hitSounds0));
        CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "_shortCutEffectsAudioClips", hitSounds0));
    }
    NoteCutSoundEffectManager_Start(self);
}

MAKE_HOOK_OFFSETLESS(SceneManager_SetActiveScene, void, Scene previousActiveScene, Scene scene)
{
    Il2CppString* activeSceneName = CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppString*>(il2cpp_utils::GetClassFromName("UnityEngine.SceneManagement", "Scene"), "GetNameInternal", scene.m_Handle));
    std::string activeSceneStr  = to_utf8(csstrtostr(activeSceneName));
    std::string shaderWarmup = "ShaderWarmup";
    std::string menuScene = "MenuViewController";
    logger->info("Scene found: %s",  activeSceneStr.data());
    if(activeSceneStr == shaderWarmup)
    {
        //Load all the audioclips
        hitSoundLoader0.filePath = "sdcard/Android/data/com.beatgames.beatsaber/files/sounds/HitSound.ogg";
        //hitSoundLoader1.filePath = "sdcard/Android/data/com.beatgames.beatsaber/files/sounds/BadHitSound.ogg";
        musicLoader0.filePath = "sdcard/Android/data/com.beatgames.beatsaber/files/sounds/MenuMusic.ogg";
        hitSoundLoader0.load();
        //hitSoundLoader1.load();
        musicLoader0.load();
        //hitSounds1 = reinterpret_cast<Array<Il2CppObject*>*>(il2cpp_functions::array_new(il2cpp_utils::GetClassFromName("UnityEngine", "AudioClip"), 1));
    }

    SceneManager_SetActiveScene(previousActiveScene, scene);
}

extern "C" void setup(ModInfo &info)
{
    info.id = "QuestSounds";
    info.version = "0.1.0";
    static std::unique_ptr<const Logger> ptr(new Logger(info));
    logger = ptr.get();
    logger->info("Completed setup!");
    logger->info("Modloader name: %s", Modloader::getInfo().name.c_str());
}

// This function is called when the mod is loaded for the first time, immediately after il2cpp_init.
extern "C" void load()
{
    logger->debug("Installing QuestSounds!");
    INSTALL_HOOK_OFFSETLESS(SceneManager_SetActiveScene, il2cpp_utils::FindMethodUnsafe("UnityEngine.SceneManagement", "SceneManager", "Internal_ActiveSceneChanged", 2));
    INSTALL_HOOK_OFFSETLESS(SongPreviewPlayer_OnEnable, il2cpp_utils::FindMethodUnsafe("", "SongPreviewPlayer", "OnEnable", 0));
    INSTALL_HOOK_OFFSETLESS(NoteCutSoundEffectManager_Start, il2cpp_utils::FindMethodUnsafe("", "NoteCutSoundEffectManager", "Start", 0));

    logger->debug("Installed QuestSounds!");
}
