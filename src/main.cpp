#define NO_CODEGEN_USE
#include "audiocliploader.hpp"
#include "main.hpp"
#include <dlfcn.h>
#define RAPIDJSON_HAS_STDSTRING 1
#define SOUND_PATH_FORMAT "/sdcard/ModData/%s/Mods/QuestSounds/"



static ModInfo modInfo;

static Configuration& getConfig() {
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
    //bool lobbyAmbience_Active = true;
    std::string hitSound_filepath = soundPath + "HitSound.ogg";
    std::string badHitSound_filepath = soundPath + "BadHitSound.ogg";
    std::string menuMusic_filepath = soundPath + "MenuMusic.ogg";
    std::string menuClick_filepath = soundPath + "MenuClick.ogg";
    std::string firework_filepath = soundPath + "Firework.ogg";
    std::string levelCleared_filepath = soundPath + "LevelCleared.ogg";
    //std::string lobbyAmbience_filepath = soundPath + "LobbyMusic.ogg";
} Config;

void AddChildSound(ConfigValue& parent, std::string_view soundName, bool active, std::string filePath, ConfigDocument::AllocatorType& allocator)
{
    getLogger().debug("Adding %s to config", filePath.c_str());
    ConfigValue value(rapidjson::kObjectType);
    value.AddMember("activated", active, allocator);
    std::string data(filePath);
    value.AddMember("filepath", data, allocator);
    parent.AddMember((ConfigValue::StringRefType)soundName.data(), value, allocator);
}

// Edited ParseVector made by Darknight1050
bool ParseSound(bool& active, std::string& filepath, ConfigValue& parent, std::string_view soundName)
{
    getLogger().debug("Parsing %s", filepath.c_str());
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
    //AddChildSound(soundsValue, "LobbyMusic", Config.lobbyAmbience_Active, Config.lobbyAmbience_filepath, allocator);
    getConfig().config.AddMember("SoundsConfig", soundsValue, allocator); 
    getConfig().Write();
}

bool LoadConfig()
{
    getConfig().Load();

    if(getConfig().config.HasMember("SoundsConfig") && getConfig().config["SoundsConfig"].IsObject())
    {
        ConfigValue soundsValue = getConfig().config["SoundsConfig"].GetObject();
        if(!ParseSound(Config.hitSound_Active, Config.hitSound_filepath, soundsValue, "HitSound")) return false;
        if(!ParseSound(Config.badHitSound_Active, Config.badHitSound_filepath, soundsValue, "BadHitSound")) return false;
        if(!ParseSound(Config.menuMusic_Active, Config.menuMusic_filepath, soundsValue, "MenuMusic")) return false;
        if(!ParseSound(Config.menuClick_Active, Config.menuClick_filepath, soundsValue, "MenuClick")) return false;
        if(!ParseSound(Config.firework_Active, Config.firework_filepath, soundsValue, "Firework")) return false;
        if(!ParseSound(Config.levelCleared_Active, Config.levelCleared_filepath, soundsValue, "LevelCleared")) return false;
        //if(!ParseSound(Config.lobbyAmbience_Active, Config.lobbyAmbience_filepath, soundsValue, "LobbyMusic")) return false;
    } else return false;
    
    return true;
}

audioClipLoader::loader hitSoundLoader; // hitSound
audioClipLoader::loader badHitSoundLoader; // badHitSound
audioClipLoader::loader menuMusicLoader;    // menuMusic
audioClipLoader::loader menuClickLoader;
audioClipLoader::loader fireworkSoundLoader;
audioClipLoader::loader levelClearedLoader;
//audioClipLoader::loader lobbyAmbienceLoader;    // Added for LobbyMusic
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
    //lobbyAmbienceLoader.filePath = Config.lobbyAmbience_filepath; // Added for LobbyMusic
    if(Config.hitSound_Active) hitSoundLoader.load();
    if(Config.badHitSound_Active) badHitSoundLoader.load();
    if(Config.menuMusic_Active) menuMusicLoader.load();
    if(Config.menuClick_Active) menuClickLoader.load();
    if(Config.firework_Active) fireworkSoundLoader.load();
    if(Config.levelCleared_Active) levelClearedLoader.load();
    //if(Config.lobbyAmbience_Active) lobbyAmbienceLoader.load();    // Added for LobbyMusic
}

Il2CppArray* createAudioClipArray(audioClipLoader::loader clipLoader)
{
    Il2CppObject* tempClip = clipLoader.getClip();
    Il2CppArray* temporaryArray = (il2cpp_functions::array_new(il2cpp_utils::GetClassFromName("UnityEngine", "AudioClip"), 1));
    il2cpp_array_set(temporaryArray, Il2CppObject*, 0, tempClip);
    return temporaryArray;
}

MAKE_HOOK_OFFSETLESS(ResultsViewController_DidActivate, void, Il2CppObject* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    if(levelClearedLoader.loaded && addedToHierarchy)
    {
        getLogger().debug("Overwriting LevelCleared Audio");
        Il2CppObject* audioClip = levelClearedLoader.getClip();
        CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "_levelClearedAudioClip", audioClip));
    } 
    ResultsViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
}

MAKE_HOOK_OFFSETLESS(SongPreviewPlayer_OnEnable, void, Il2CppObject* self)
{
    
    getLogger().info("MenuMusic is it true: %i", menuMusicLoader.loaded);
    if(menuMusicLoader.loaded)
    {
        getLogger().debug("Overwriting Menu Audio");
        Il2CppObject* audioClip = menuMusicLoader.getClip();
        if(audioClip != nullptr)
            CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "_defaultAudioClip", audioClip));
    }
    SongPreviewPlayer_OnEnable(self);
}

//MAKE_HOOK_OFFSETLESS(GameServerLobbyFlowCoordinator_DidActivate, void, Il2CppObject* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
//{
//    getLogger().debug("GameServerLobbyFlowCoordinator_DidActivate");
//
//    getLogger().info("LobbyMusic is it true: %i", lobbyAmbienceLoader.loaded);
//    if (lobbyAmbienceLoader.loaded && addedToHierarchy)
//    {
//        getLogger().debug("Overwriting LobbyAmbience Audio");
//        Il2CppObject* audioClip = lobbyAmbienceLoader.getClip();
//        getLogger().debug("LobbyAmbience getClip succesful, setting clip next");
//        CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "_ambienceAudioClip", audioClip));
//        getLogger().debug("LobbyAmbience SetFieldValue succesful");
//        // _ambienceAudioClip
//    }
//    GameServerLobbyFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
//}
//
//MAKE_HOOK_OFFSETLESS(GameServerLobbyFlowCoordinator_DidDeactivate, void, Il2CppObject* self, bool removedFromHierarchy, bool screenSystemDisabling)
//{
//    getLogger().debug("GameServerLobbyFlowCoordinator_DidDeactivate");
//
//    getLogger().info("LobbyMusic is it true: %i", lobbyAmbienceLoader.loaded);
//    if (menuMusicLoader.loaded && removedFromHierarchy)
//    {
//        getLogger().debug("Switching LobbyMusic to MenuMusic Audio");
//        Il2CppObject* audioClip = menuMusicLoader.getClip();
//        getLogger().debug("MenuMusic getClip succesful, setting clip next");
//        CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "_ambienceAudioClip", audioClip));
//        getLogger().debug("LobbyAmbience SetFieldValue succesful");
//        // _ambienceAudioClip
//    }
//    GameServerLobbyFlowCoordinator_DidDeactivate(self, removedFromHierarchy, screenSystemDisabling);
//}
//// */
//
//// /*
//MAKE_HOOK_OFFSETLESS(MultiplayerModeSelectionFlowCoordinator_DidActivate, void, Il2CppObject* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
//{
//    getLogger().debug("MultiplayerModeSelectionFlowCoordinator_DidActivate");
//
//    getLogger().info("LobbyMusic is it true: %i", menuMusicLoader.loaded);
//    if (menuMusicLoader.loaded && addedToHierarchy)
//    {
//        getLogger().debug("Switching LobbyMusic to MenuMusic Audio");
//        Il2CppObject* audioClip = menuMusicLoader.getClip();
//        getLogger().debug("MenuMusic getClip succesful, setting clip next");
//        CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "_ambienceAudioClip", audioClip));
//        getLogger().debug("LobbyAmbience SetFieldValue succesful");
//        // _ambienceAudioClip
//    }
//    MultiplayerModeSelectionFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling); // This has to be ran last, otherwise it will not work correctly
//}
//// /*
//MAKE_HOOK_OFFSETLESS(MultiplayerModeSelectionFlowCoordinator_DidDeactivate, void, Il2CppObject* self, bool removedFromHierarchy, bool screenSystemDisabling)
//{
//    getLogger().debug("MultiplayerModeSelectionFlowCoordinator_DidDeactivate");
//
//    getLogger().info("LobbyMusic is it true: %i", lobbyAmbienceLoader.loaded);
//    if (menuMusicLoader.loaded && removedFromHierarchy)
//    {
//        getLogger().debug("Switching LobbyMusic to MenuMusic Audio");
//        Il2CppObject* audioClip = menuMusicLoader.getClip();
//        getLogger().debug("MenuMusic getClip succesful, setting clip next");
//        CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "_ambienceAudioClip", audioClip));
//        getLogger().debug("LobbyAmbience SetFieldValue succesful");
//        // _ambienceAudioClip
//    }
//    MultiplayerModeSelectionFlowCoordinator_DidDeactivate(self, removedFromHierarchy, screenSystemDisabling);  
//}
////*/

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

// This is needed because typedefs were removed in current BS-Hook
struct Scene {
    int m_Handle;
};
DEFINE_IL2CPP_ARG_TYPE(Scene, "UnityEngine.SceneManagement", "Scene");

MAKE_HOOK_OFFSETLESS(SceneManager_ActiveSceneChanged, void, Scene previousActiveScene, Scene nextActiveScene)
{
    Il2CppString* activeSceneName = CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppString*>(il2cpp_utils::GetClassFromName("UnityEngine.SceneManagement", "Scene"), "GetNameInternal", nextActiveScene.m_Handle));
    std::string activeSceneStr = to_utf8(csstrtostr(activeSceneName));
    getLogger().info("Scene found: %s", activeSceneStr.data());

    std::string shaderWarmup = "ShaderWarmup";
    if (activeSceneStr == shaderWarmup) loadAudioClips();

    SceneManager_ActiveSceneChanged(previousActiveScene, nextActiveScene);
}

extern "C" void setup(ModInfo &info)
{
    info.id = ID;
    info.version = VERSION;
    modInfo = info;

    getLogger().info("Loading Config");
    getConfig();
    getLogger().info("Completed setup!");
    getLogger().info("Modloader name: %s", Modloader::getInfo().name.c_str());
}  

extern "C" void load()
{
    Logger& hkLog = getLogger();
    if(!LoadConfig()) SaveConfig();
    makeFolder();
    if (Modloader::getAllConstructed()) {
        getLogger().info("Modloader Finished");
    }
    getLogger().info("Installing QuestSounds!");
    auto* SM_ActiveSceneChanged =   il2cpp_utils::FindMethodUnsafe("UnityEngine.SceneManagement", "SceneManager", "Internal_ActiveSceneChanged", 2);
    auto* RVC_DidActivate =         il2cpp_utils::FindMethodUnsafe("", "ResultsViewController", "DidActivate", 3);
    auto* SPP_OnEnable =            il2cpp_utils::FindMethodUnsafe("", "SongPreviewPlayer", "OnEnable", 0);
    auto* NCSEM_Start =             il2cpp_utils::FindMethodUnsafe("", "NoteCutSoundEffectManager", "Start", 0);
    auto* BUIAM_Start =             il2cpp_utils::FindMethodUnsafe("", "BasicUIAudioManager", "Start", 0);
    auto* NCSE_Awake =              il2cpp_utils::FindMethodUnsafe("", "NoteCutSoundEffect", "Awake", 0);
    auto* FIC_Awake =               il2cpp_utils::FindMethodUnsafe("", "FireworkItemController", "Awake", 0);
    //auto* MMSFC_DidActivate =       il2cpp_utils::FindMethodUnsafe("", "MultiplayerModeSelectionFlowCoordinator", "DidActivate", 3);
    //auto* MMSFC_DidDeactivate =     il2cpp_utils::FindMethodUnsafe("", "MultiplayerModeSelectionFlowCoordinator", "DidDeactivate", 2);
    //auto* GSLFC_DidActivate =       il2cpp_utils::FindMethodUnsafe("", "GameServerLobbyFlowCoordinator", "DidActivate", 3);
    //auto* GSLFC_DidDeactivate =     il2cpp_utils::FindMethodUnsafe("", "GameServerLobbyFlowCoordinator", "DidDeactivate", 2);
    INSTALL_HOOK_OFFSETLESS(hkLog, SceneManager_ActiveSceneChanged, SM_ActiveSceneChanged);
    INSTALL_HOOK_OFFSETLESS(hkLog, SongPreviewPlayer_OnEnable, SPP_OnEnable);
    INSTALL_HOOK_OFFSETLESS(hkLog, NoteCutSoundEffectManager_Start, NCSEM_Start);
    INSTALL_HOOK_OFFSETLESS(hkLog, NoteCutSoundEffect_Awake, NCSE_Awake);
    INSTALL_HOOK_OFFSETLESS(hkLog, FireworkItemController_Awake, FIC_Awake);
    INSTALL_HOOK_OFFSETLESS(hkLog, BasicUIAudioManager_Start, BUIAM_Start);
    INSTALL_HOOK_OFFSETLESS(hkLog, ResultsViewController_DidActivate, RVC_DidActivate);
    //INSTALL_HOOK_OFFSETLESS(hkLog, MultiplayerModeSelectionFlowCoordinator_DidActivate, MMSFC_DidActivate);     // Added for switching out MP Lobby Music
    //INSTALL_HOOK_OFFSETLESS(hkLog, MultiplayerModeSelectionFlowCoordinator_DidDeactivate, MMSFC_DidDeactivate);  // Added for switching out MP Lobby Music
    //INSTALL_HOOK_OFFSETLESS(hkLog, GameServerLobbyFlowCoordinator_DidActivate, GSLFC_DidActivate);              // Added for switching out MP Lobby Music
    //INSTALL_HOOK_OFFSETLESS(hkLog, GameServerLobbyFlowCoordinator_DidDeactivate, GSLFC_DidDeactivate);          // Added for switching out MP Lobby Music
    getLogger().info("Installed QuestSounds!");
}