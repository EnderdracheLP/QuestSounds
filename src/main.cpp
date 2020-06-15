#include <dlfcn.h>
#include "../extern/beatsaber-hook/shared/utils/utils.h"
#include "../extern/beatsaber-hook/shared/utils/logging.hpp"
#include "../extern/beatsaber-hook/include/modloader.hpp"
#include "../extern/beatsaber-hook/shared/utils/typedefs.h"
#include "../extern/beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "../extern/beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "../include/soundconfig.hpp"
#include "../extern/beatsaber-hook/shared/config/config-utils.hpp"
#include "../include/audiocliploader.hpp"



static ModInfo modInfo;

Configuration& getConfig() {
    static Configuration config(modInfo);
    return config;
}

const Logger& getLogger() {
  static const Logger& logger(modInfo);
  return logger;
}



audioClipLoader::loader hitSoundLoader; // hitSound
audioClipLoader::loader badHitSoundLoader; // badHitSound
audioClipLoader::loader menuMusicLoader;    // menuMusic
audioClipLoader::loader menuClickLoader;
audioClipLoader::loader fireworkSoundLoader;
audioClipLoader::loader levelClearedLoader;
Il2CppArray* hitSoundArr; // hitSoundArray
Il2CppArray* badHitSoundArr; // badHitSoundArray
Il2CppArray* menuClickArr;
Il2CppArray* fireworkSoundArr; 

void loadAudioClips()
{
    hitSoundLoader.filePath = Config.hitSound_filepath;
    badHitSoundLoader.filePath = Config.badHitSound_filepath;
    menuMusicLoader.filePath = Config.menuMusic_filepath;
    menuClickLoader.filePath = Config.menuClick_filepath;
    fireworkSoundLoader.filePath = Config.firework_filepath;
    levelClearedLoader.filePath =  Config.levelCleared_filepath;
    if(Config.hitSound_Active) hitSoundLoader.load();
    if(Config.badHitSound_Active) badHitSoundLoader.load();
    if(Config.menuMusic_Active) menuMusicLoader.load();
    if(Config.menuClick_Active) menuClickLoader.load();
    if(Config.firework_Active) fireworkSoundLoader.load();
    if(Config.levelCleared_Active) levelClearedLoader.load();
}

Il2CppArray* createAudioClipArray(audioClipLoader::loader clipLoader)
{
    Il2CppObject* tempClip = clipLoader.getClip();
    Il2CppArray* temporaryArray = (il2cpp_functions::array_new(il2cpp_utils::GetClassFromName("UnityEngine", "AudioClip"), 1));
    il2cpp_array_set(temporaryArray, Il2CppObject*, 0, tempClip);
    return temporaryArray;
}

MAKE_HOOK_OFFSETLESS(ResultsViewController_DidActivate, void, Il2CppObject* self, bool firstActivation, int activationType)
{
    if(levelClearedLoader.loaded && activationType == 0)
    {
        Il2CppObject* audioClip = levelClearedLoader.getClip();
        CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "_levelClearedAudioClip", audioClip));
    } 
    ResultsViewController_DidActivate(self, firstActivation, activationType);
}

MAKE_HOOK_OFFSETLESS(SongPreviewPlayer_OnEnable, void, Il2CppObject* self)
{
    
    getLogger().info("is it true: %i", menuMusicLoader.loaded);
    if(menuMusicLoader.loaded)
    {
        Il2CppObject* audioClip = menuMusicLoader.getClip();
        if(audioClip != nullptr)
            CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "_defaultAudioClip", audioClip));
    }
    SongPreviewPlayer_OnEnable(self);

}

MAKE_HOOK_OFFSETLESS(NoteCutSoundEffectManager_Start, void, Il2CppObject* self)
{
    if(hitSoundLoader.loaded)
    {
        hitSoundArr = createAudioClipArray(hitSoundLoader);
        CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "_longCutEffectsAudioClips", hitSoundArr));
        CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "_shortCutEffectsAudioClips", hitSoundArr));
    }
    NoteCutSoundEffectManager_Start(self);
}

MAKE_HOOK_OFFSETLESS(NoteCutSoundEffect_Awake, void, Il2CppObject* self)
{
    if(badHitSoundLoader.loaded)
    {
        badHitSoundArr = createAudioClipArray(badHitSoundLoader);
        CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "_badCutSoundEffectAudioClips", badHitSoundArr));
    }
    NoteCutSoundEffect_Awake(self);
}

MAKE_HOOK_OFFSETLESS(BasicUIAudioManager_Start, void, Il2CppObject* self)
{
    if(menuClickLoader.loaded)
    {
        menuClickArr = createAudioClipArray(menuClickLoader);
        CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "_clickSounds", menuClickArr));
    }
    BasicUIAudioManager_Start(self);
}

MAKE_HOOK_OFFSETLESS(FireworkItemController_Awake, void, Il2CppObject* self)
{
    if(fireworkSoundLoader.loaded)
    {
        fireworkSoundArr = createAudioClipArray(fireworkSoundLoader);
        CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "_explosionClips", fireworkSoundArr));
    }
    FireworkItemController_Awake(self);
}

MAKE_HOOK_OFFSETLESS(SceneManager_ActiveSceneChanged, void, Scene previousActiveScene, Scene nextActiveScene)
{
    Il2CppString* activeSceneName = CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppString*>(il2cpp_utils::GetClassFromName("UnityEngine.SceneManagement", "Scene"), "GetNameInternal", nextActiveScene.m_Handle));
    std::string activeSceneStr  = to_utf8(csstrtostr(activeSceneName));
    getLogger().info("Scene found: %s",  activeSceneStr.data());
    
    std::string shaderWarmup = "ShaderWarmup";
    if(activeSceneStr == shaderWarmup) loadAudioClips();

    SceneManager_ActiveSceneChanged(previousActiveScene, nextActiveScene);
}

extern "C" void setup(ModInfo &info)
{
    info.id = "QuestSounds";
    info.version = "0.1.0";
    modInfo = info;
    getConfig();
    getLogger().info("Completed setup!");
    getLogger().info("Modloader name: %s", Modloader::getInfo().name.c_str());
}  


// This function is called when the mod is loaded for the first time, immediately after il2cpp_init.
extern "C" void load()
{
    if(!LoadConfig()) SaveConfig();
    getLogger().debug("Installing QuestSounds!");
    INSTALL_HOOK_OFFSETLESS(SceneManager_ActiveSceneChanged, il2cpp_utils::FindMethodUnsafe("UnityEngine.SceneManagement", "SceneManager", "Internal_ActiveSceneChanged", 2));
    INSTALL_HOOK_OFFSETLESS(SongPreviewPlayer_OnEnable, il2cpp_utils::FindMethodUnsafe("", "SongPreviewPlayer", "OnEnable", 0));
    INSTALL_HOOK_OFFSETLESS(NoteCutSoundEffectManager_Start, il2cpp_utils::FindMethodUnsafe("", "NoteCutSoundEffectManager", "Start", 0));
    INSTALL_HOOK_OFFSETLESS(NoteCutSoundEffect_Awake, il2cpp_utils::FindMethodUnsafe("", "NoteCutSoundEffect", "Awake", 0));
    INSTALL_HOOK_OFFSETLESS(FireworkItemController_Awake, il2cpp_utils::FindMethodUnsafe("", "FireworkItemController", "Awake", 0));
    INSTALL_HOOK_OFFSETLESS(BasicUIAudioManager_Start, il2cpp_utils::FindMethodUnsafe("", "BasicUIAudioManager", "Start", 0));
    INSTALL_HOOK_OFFSETLESS(ResultsViewController_DidActivate, il2cpp_utils::FindMethodUnsafe("", "ResultsViewController", "DidActivate", 2));
    getLogger().debug("Installed QuestSounds!");
}
