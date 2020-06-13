#include <dlfcn.h>
#include "../extern/beatsaber-hook/shared/utils/utils.h"
#include "../extern/beatsaber-hook/shared/utils/logging.hpp"
#include "../extern/beatsaber-hook/include/modloader.hpp"
#include "../extern/beatsaber-hook/shared/utils/typedefs.h"
#include "../extern/beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "../extern/beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "../include/audiocliploader.hpp"

static const Logger *logger;
audioClipLoader::loader hitSoundLoader0; // hitsound
audioClipLoader::loader hitSoundLoader1; // badHitsound
audioClipLoader::loader musicLoader0;    // menuMusic

MAKE_HOOK_OFFSETLESS(SceneManager_SetActiveScene, bool, Scene scene)
{
    bool activeScene = SceneManager_SetActiveScene(scene);
    Il2CppString* activeSceneName = RET_0_UNLESS(il2cpp_utils::RunMethod<Il2CppString*>(il2cpp_utils::GetClassFromName("UnityEngine.SceneManagement", "Scene"), "GetNameInternal", scene.m_Handle));
    if(activeSceneName == nullptr)
        return activeScene;
    
    std::string activeSceneStr  = to_utf8(csstrtostr(activeSceneName));
    std::string shaderWarmup = "ShaderWarmup";
    logger->info("Scene found: %s",  activeSceneStr.data());
    if(activeSceneStr == shaderWarmup)
    {
        //Load all the audioclips
        hitSoundLoader0.filePath = "sdcard/Android/data/com.beatgames.beatsaber/files/sounds/HitSound.ogg";
        hitSoundLoader1.filePath = "sdcard/Android/data/com.beatgames.beatsaber/files/sounds/BadHitSound.ogg";
        musicLoader0.filePath = "sdcard/Android/data/com.beatgames.beatsaber/files/sounds/MenuMusic.ogg";
        hitSoundLoader0.load();
        hitSoundLoader1.load();
        musicLoader0.load();
    }
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
    logger->debug("Installed QuestSounds!");
}
