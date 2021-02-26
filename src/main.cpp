#include "main.hpp"
#include "audiocliploader.hpp"
#include <dlfcn.h>

#include "GlobalNamespace/ResultsViewController.hpp"
#include "GlobalNamespace/SongPreviewPlayer.hpp"
#include "GlobalNamespace/NoteCutSoundEffectManager.hpp"
#include "GlobalNamespace/NoteCutSoundEffect.hpp"
#include "GlobalNamespace/BasicUIAudioManager.hpp"
#include "GlobalNamespace/FireworkItemController.hpp"
using namespace GlobalNamespace;

#include "UnityEngine/SceneManagement/Scene.hpp"
using namespace UnityEngine;

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"
using namespace QuestUI;

//#include "NewAudioClipLoader.hpp"
#include "QSoundsViewController.hpp"
using namespace QuestSounds;

#include "custom-types/shared/register.hpp"
using namespace custom_types;

#define RAPIDJSON_HAS_STDSTRING 1
#define SOUND_PATH_FORMAT "/sdcard/ModData/%s/Mods/QuestSounds/"

ModInfo modInfo;

Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

Logger& getLogger() {
    static auto logger = new Logger(modInfo, LoggerOptions(false, true));
    return *logger;
}

std::string soundPath = string_format(SOUND_PATH_FORMAT, Modloader::getApplicationId().c_str());
void makeFolder() 
{
    
    if(!direxists(soundPath.c_str()))
    {
        int makePath = mkpath(soundPath.data());
        if(makePath == -1)
        {
            getLogger().debug("Failed to make path!");
        }
    }
}

//Config stuff
static struct Config_t 
{
    bool hitSound_Active = true;
    bool badHitSound_Active = true;
    bool menuMusic_Active = true;
    bool menuClick_Active = true;
    bool firework_Active = true;
    bool levelCleared_Active = true;
    std::string hitSound_filepath = soundPath + "HitSound.ogg";
    std::string badHitSound_filepath = soundPath + "BadHitSound.ogg";
    std::string menuMusic_filepath = soundPath + "MenuMusic.ogg";
    std::string menuClick_filepath = soundPath + "MenuClick.ogg";
    std::string firework_filepath = soundPath + "Firework.ogg";
    std::string levelCleared_filepath = soundPath + "LevelCleared.ogg";
} Config;

void AddChildSound(ConfigValue& parent, std::string_view soundName, bool active, std::string filePath, ConfigDocument::AllocatorType& allocator)
{
    ConfigValue value(rapidjson::kObjectType);
    value.AddMember("activated", active, allocator);
    std::string data(filePath);
    value.AddMember("filepath", data, allocator);
    parent.AddMember((ConfigValue::StringRefType)soundName.data(), value, allocator);
}

// Edited ParseVector made by Darknight1050
bool ParseSound(bool& active, std::string& filepath, ConfigValue& parent, std::string_view soundName)
{
    if(!parent.HasMember(soundName.data()) || !parent[soundName.data()].IsObject()) return false;
    ConfigValue value = parent[soundName.data()].GetObject();
    if(!(value.HasMember("activated") && value["activated"].IsBool() &&
         value.HasMember("filepath") && value["filepath"].IsString() )) return false;
    active = value["activated"].GetBool();
    filepath = value["filepath"].GetString();
    return true;
}

void SaveConfig()
{
    getConfig().config.RemoveAllMembers();
    getConfig().config.SetObject();
    auto& allocator = getConfig().config.GetAllocator();

    ConfigValue soundsValue(rapidjson::kObjectType);
    AddChildSound(soundsValue, "HitSound", Config.hitSound_Active, Config.hitSound_filepath, allocator);
    AddChildSound(soundsValue, "BadHitSound", Config.badHitSound_Active, Config.badHitSound_filepath, allocator);  
    AddChildSound(soundsValue, "MenuMusic", Config.menuMusic_Active, Config.menuMusic_filepath, allocator);  
    AddChildSound(soundsValue, "MenuClick", Config.menuClick_Active, Config.menuClick_filepath, allocator);  
    AddChildSound(soundsValue, "Firework", Config.firework_Active, Config.firework_filepath, allocator);  
    AddChildSound(soundsValue, "LevelCleared", Config.levelCleared_Active, Config.levelCleared_filepath, allocator); 
    getConfig().config.AddMember("Sounds 1.0.0", soundsValue, allocator); 
    getConfig().Write();

}

bool LoadConfig()
{
    getConfig().Load();

    if(getConfig().config.HasMember("Sounds 1.0.0") && getConfig().config["Sounds 1.0.0"].IsObject())
    {
        ConfigValue soundsValue = getConfig().config["Sounds 1.0.0"].GetObject();
        if(!ParseSound(Config.hitSound_Active, Config.hitSound_filepath, soundsValue, "HitSound")) return false;
        if(!ParseSound(Config.badHitSound_Active, Config.badHitSound_filepath, soundsValue, "BadHitSound")) return false;
        if(!ParseSound(Config.menuMusic_Active, Config.menuMusic_filepath, soundsValue, "MenuMusic")) return false;
        if(!ParseSound(Config.menuClick_Active, Config.menuClick_filepath, soundsValue, "MenuClick")) return false;
        if(!ParseSound(Config.firework_Active, Config.firework_filepath, soundsValue, "Firework")) return false;
        if(!ParseSound(Config.levelCleared_Active, Config.levelCleared_filepath, soundsValue, "LevelCleared")) return false;
    } else return false;
    
    return true;
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

//MAKE_HOOK_OFFSETLESS(ResultsViewController_DidActivate, void, Il2CppObject* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
//{
//    if(levelClearedLoader.loaded && addedToHierarchy)
//    {
//        Il2CppObject* audioClip = levelClearedLoader.getClip();
//        CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "_levelClearedAudioClip", audioClip));
//    } 
//    ResultsViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
//}

MAKE_HOOK_OFFSETLESS(ResultsViewController_DidActivate, void, ResultsViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    ResultsViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    if (levelClearedLoader.loaded && addedToHierarchy)
    {
        ResultsViewController* audioClip = levelClearedLoader.getClip();
        self->levelClearedAudioClip = audioClip;
        //CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "_levelClearedAudioClip", audioClip));
    }
}

//MAKE_HOOK_OFFSETLESS(SongPreviewPlayer_OnEnable, void, Il2CppObject* self)
//{
//    
//    getLogger().info("is it true: %i", menuMusicLoader.loaded);
//    if(menuMusicLoader.loaded)
//    {
//        Il2CppObject* audioClip = menuMusicLoader.getClip();
//        if(audioClip != nullptr)
//            CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "_defaultAudioClip", audioClip));
//    }
//    SongPreviewPlayer_OnEnable(self);
//
//}

MAKE_HOOK_OFFSETLESS(SongPreviewPlayer_OnEnable, void, SongPreviewPlayer* self) {
    SongPreviewPlayer_OnEnable(self);
    getLogger().info("is it true: %i", menuMusicLoader.loaded);

    if (menuMusicLoader.loaded)
    {
        SongPreviewPlayer* audioClip = menuMusicLoader.getClip();
        if (audioClip != nullptr)
            self->defaultAudioClip = audioClip;
            //CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "_defaultAudioClip", audioClip));
    }
}

MAKE_HOOK_OFFSETLESS(NoteCutSoundEffectManager_Start, void, NoteCutSoundEffectManager* self) {
    NoteCutSoundEffectManager_Start(self);
    if(hitSoundLoader.loaded)
    {
        hitSoundArr = createAudioClipArray(hitSoundLoader);
        self->longCutEffectsAudioClips = hitSoundArr;
        self->shortCutEffectsAudioClips = hitSoundArr;
        //CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "_longCutEffectsAudioClips", hitSoundArr));
        //CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "_shortCutEffectsAudioClips", hitSoundArr));
    }
}

MAKE_HOOK_OFFSETLESS(NoteCutSoundEffect_Awake, void, NoteCutSoundEffect* self) {
    NoteCutSoundEffect_Awake(self);
    if(badHitSoundLoader.loaded)
    {
        badHitSoundArr = createAudioClipArray(badHitSoundLoader);
        //CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "_badCutSoundEffectAudioClips", badHitSoundArr));
        self->badCutSoundEffectAudioClips = badHitSoundArr;
    }

}

MAKE_HOOK_OFFSETLESS(BasicUIAudioManager_Start, void, BasicUIAudioManager* self) {
    BasicUIAudioManager_Start(self);
    if(menuClickLoader.loaded)
    {
        menuClickArr = createAudioClipArray(menuClickLoader);
        self->clickSounds = menuClickArr;
        //CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "_clickSounds", menuClickArr));
    }
}

MAKE_HOOK_OFFSETLESS(FireworkItemController_Awake, void, FireworkItemController* self) {
    FireworkItemController_Awake(self);
    if(fireworkSoundLoader.loaded)
    {
        fireworkSoundArr = createAudioClipArray(fireworkSoundLoader);
        self->explosionClips = fireworkSoundArr;
        //CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "_explosionClips", fireworkSoundArr));
    }
}

//MAKE_HOOK_OFFSETLESS(SceneManager_ActiveSceneChanged, void, Scene previousActiveScene, Scene nextActiveScene) {
//    SceneManager_ActiveSceneChanged(previousActiveScene, nextActiveScene);
//    Il2CppString* activeSceneName = CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppString*>(il2cpp_utils::GetClassFromName("UnityEngine.SceneManagement", "Scene"), "GetNameInternal", nextActiveScene.m_Handle));
//    std::string activeSceneStr  = to_utf8(csstrtostr(activeSceneName));
//    getLogger().info("Scene found: %s",  activeSceneStr.data());
//    
//    std::string shaderWarmup = "ShaderWarmup";
//    if(activeSceneStr == shaderWarmup) loadAudioClips();
//
//
//}

MAKE_HOOK_OFFSETLESS(SceneManager_Internal_ActiveSceneChanged, void, UnityEngine::SceneManagement::Scene prevScene, UnityEngine::SceneManagement::Scene nextScene) {
    SceneManager_Internal_ActiveSceneChanged(prevScene, nextScene);
    if (nextScene.IsValid()) {
        std::string sceneName = to_utf8(csstrtostr(nextScene.get_name()));
        getLogger().info("Scene found: %s", sceneName.data());

        std::string shaderWarmup = "ShaderWarmup";
            if(sceneName == shaderWarmup) loadAudioClips();
        //if (sceneName.find("MenuViewControllers") != std::string::npos) { //Don't use find
        //}
    }
}

extern "C" void setup(ModInfo &info)
{
    info.id = "QuestSounds";
    info.version = VERSION;
    modInfo = info;
    getConfig();
    getLogger().info("Completed setup!");
    getLogger().info("Modloader name: %s", Modloader::getInfo().name.c_str());
}  

extern "C" void load()
{
    il2cpp_functions::Init();
    QuestUI::Init();

    Logger& hkLog = getLogger();

    if(!LoadConfig()) SaveConfig();
    makeFolder();
    getLogger().debug("Installing QuestSounds!");
    auto* SM_ActiveSceneChanged =   il2cpp_utils::FindMethodUnsafe("UnityEngine.SceneManagement", "SceneManager", "Internal_ActiveSceneChanged", 2);
    auto* RVC_DidActivate =         il2cpp_utils::FindMethodUnsafe("", "ResultsViewController", "DidActivate", 3);
    auto* SPP_OnEnable =            il2cpp_utils::FindMethodUnsafe("", "SongPreviewPlayer", "OnEnable", 0);
    auto* NCSEM_Start =             il2cpp_utils::FindMethodUnsafe("", "NoteCutSoundEffectManager", "Start", 0);
    auto* BUIAM_Start =             il2cpp_utils::FindMethodUnsafe("", "BasicUIAudioManager", "Start", 0);
    auto* NCSE_Awake =              il2cpp_utils::FindMethodUnsafe("", "NoteCutSoundEffect", "Awake", 0);
    auto* FIC_Awake =               il2cpp_utils::FindMethodUnsafe("", "FireworkItemController", "Awake", 0);
    INSTALL_HOOK_OFFSETLESS(hkLog, SceneManager_ActiveSceneChanged, SM_ActiveSceneChanged);
    INSTALL_HOOK_OFFSETLESS(hkLog, SongPreviewPlayer_OnEnable, SPP_OnEnable);
    INSTALL_HOOK_OFFSETLESS(hkLog, NoteCutSoundEffectManager_Start, NCSEM_Start);
    INSTALL_HOOK_OFFSETLESS(hkLog, NoteCutSoundEffect_Awake, NCSE_Awake);
    INSTALL_HOOK_OFFSETLESS(hkLog, FireworkItemController_Awake, FIC_Awake);
    INSTALL_HOOK_OFFSETLESS(hkLog, BasicUIAudioManager_Start, BUIAM_Start);
    INSTALL_HOOK_OFFSETLESS(hkLog, ResultsViewController_DidActivate, RVC_DidActivate);
    getLogger().debug("Installed QuestSounds!");
}
