#if defined(BS__1_16)
#elif defined(BS_1_13_2)
#error Define BS Version
#endif

#include "main.hpp"
#include "audiocliploader.hpp"
#include "QSoundsConfig.hpp"
#include <dlfcn.h>
using namespace QSoundsConfig;
//using namespace audioClipLoader;

#include "GlobalNamespace/ResultsViewController.hpp"
#include "GlobalNamespace/SongPreviewPlayer.hpp"
#include "GlobalNamespace/NoteCutSoundEffectManager.hpp"
#include "GlobalNamespace/NoteCutSoundEffect.hpp"
#include "GlobalNamespace/BasicUIAudioManager.hpp"
#include "GlobalNamespace/FireworkItemController.hpp"
#include "GlobalNamespace/GameServerLobbyFlowCoordinator.hpp"
#include "GlobalNamespace/MultiplayerModeSelectionFlowCoordinator.hpp"
using namespace GlobalNamespace;


#include "UnityEngine/SceneManagement/Scene.hpp"
//using namespace UnityEngine::SceneManagement;

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"
using namespace QuestUI;

//#include "NewAudioClipLoader.hpp"
#include "QSoundsSdListViewController.hpp"
//#include "QSoundsConfigViewController.hpp"
#include "QSoundsFlowCoordinator.hpp"
using namespace QuestSounds;

#include "custom-types/shared/register.hpp"
using namespace custom_types;

#define RAPIDJSON_HAS_STDSTRING 1
/*
#define HITSOUNDS_PATH_FORMAT "/sdcard/ModData/%s/Mods/QuestSounds/HitSounds/"
#define BADHITSOUNDS_PATH_FORMAT "/sdcard/ModData/%s/Mods/QuestSounds/BadHitSounds/"
#define MENUMUSIC_PATH_FORMAT "/sdcard/ModData/%s/Mods/QuestSounds/MenuMusicTracks/"
*/


ModInfo modInfo;

Configuration& getConfig() {
    static Configuration config(modInfo);
    //config.Load();
    return config;
}


Logger& getLogger() {
    static auto logger = new Logger(modInfo, LoggerOptions(false, true));
    return *logger;
}

//Config_t QSoundsConfig::Config;

//std::string soundPath = string_format(SOUND_PATH_FORMAT, Modloader::getApplicationId().c_str());
/*
std::string HitSoundsPath = string_format(HITSOUNDS_PATH_FORMAT, Modloader::getApplicationId().c_str());
std::string BadHitSoundsPath = string_format(BADHITSOUNDS_PATH_FORMAT, Modloader::getApplicationId().c_str());
std::string MenuMusicPath = string_format(MENUMUSIC_PATH_FORMAT, Modloader::getApplicationId().c_str());
*/

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
    /*
    if (!direxists(HitSoundsPath.c_str()))
    {
        int makePath = mkpath(HitSoundsPath.data());
        if (makePath == -1)
        {
            getLogger().debug("Failed to make HitSound Folder path!");
        }
    }
    if (!direxists(BadHitSoundsPath.c_str()))
    {
        int makePath = mkpath(HitsoundsPath.data());
        if (makePath == -1)
        {
            getLogger().debug("Failed to make HitSound Folder path!");
        }
            if (!direxists(MenuMusicPath.c_str()))
    {
        int makePath = mkpath(MenuMusicPath.data());
        if (makePath == -1)
        {
            getLogger().debug("Failed to make HitSound Folder path!");
        }
    }
    */
}


audioClipLoader::loader hitSoundLoader,     // hitSound
                        badHitSoundLoader,  // badHitSound
                        menuMusicLoader,    // menuMusic
                        menuClickLoader,
                        fireworkSoundLoader,
                        levelClearedLoader,
                        lobbyAmbienceLoader;    // Added for LobbyMusic
Array<UnityEngine::AudioClip*> *hitSoundArr,    // hitSoundArray
                               *badHitSoundArr, // badHitSoundArray
                               *menuClickArr,
                               *fireworkSoundArr;

void loadAudioClips()
{
    hitSoundLoader.filePath = Config.hitSound_filepath;
    badHitSoundLoader.filePath = Config.badHitSound_filepath;
    menuMusicLoader.filePath = Config.menuMusic_filepath;
    menuClickLoader.filePath = Config.menuClick_filepath;
    fireworkSoundLoader.filePath = Config.firework_filepath;
    levelClearedLoader.filePath =  Config.levelCleared_filepath;
    lobbyAmbienceLoader.filePath = Config.lobbyAmbience_filepath; // Added for LobbyMusic
    if(Config.hitSound_Active) hitSoundLoader.load();
    if(Config.badHitSound_Active) badHitSoundLoader.load();
    if(Config.menuMusic_Active) menuMusicLoader.load();
    if(Config.menuClick_Active) menuClickLoader.load();
    if(Config.firework_Active) fireworkSoundLoader.load();
    if(Config.levelCleared_Active) levelClearedLoader.load();
    if(Config.lobbyAmbience_Active) lobbyAmbienceLoader.load();    // Added for LobbyMusic
}

// Creates an Array, of AudioClips
Array<UnityEngine::AudioClip*>* createAudioClipArray(audioClipLoader::loader clipLoader)
{
    UnityEngine::AudioClip* tempClip = clipLoader.getClip();
    auto* temporaryArray = Array<UnityEngine::AudioClip*>::New(tempClip);
    temporaryArray->values[0] = tempClip;
    return temporaryArray;
}

MAKE_HOOK_OFFSETLESS(ResultsViewController_DidActivate, void, ResultsViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (levelClearedLoader.loaded && addedToHierarchy)
    {   
        UnityEngine::AudioClip* audioClip = levelClearedLoader.getClip();
        self->levelClearedAudioClip = audioClip;
    }
    ResultsViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
}

MAKE_HOOK_OFFSETLESS(SongPreviewPlayer_OnEnable, void, SongPreviewPlayer* self) {
    getLogger().info("is it true: %i", menuMusicLoader.loaded);

    if (menuMusicLoader.loaded)
    {
        UnityEngine::AudioClip* audioClip = menuMusicLoader.getClip();
        if (audioClip != nullptr)
            self->defaultAudioClip = audioClip;
    }
    SongPreviewPlayer_OnEnable(self);
}

MAKE_HOOK_OFFSETLESS(GameServerLobbyFlowCoordinator_DidActivate, void, GameServerLobbyFlowCoordinator* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    getLogger().debug("GameServerLobbyFlowCoordinator_DidActivate");

    getLogger().info("LobbyMusic is it true: %i", lobbyAmbienceLoader.loaded);
    if (lobbyAmbienceLoader.loaded && addedToHierarchy)
    {
        getLogger().debug("Overwriting LobbyAmbience Audio");
        UnityEngine::AudioClip* audioClip = lobbyAmbienceLoader.getClip();
        self->ambienceAudioClip = audioClip;
    }
    GameServerLobbyFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
}

MAKE_HOOK_OFFSETLESS(GameServerLobbyFlowCoordinator_DidDeactivate, void, GameServerLobbyFlowCoordinator* self, bool removedFromHierarchy, bool screenSystemDisabling)
{
    getLogger().debug("GameServerLobbyFlowCoordinator_DidDeactivate");

    getLogger().info("LobbyMusic is it true: %i", lobbyAmbienceLoader.loaded);
    if (menuMusicLoader.loaded && removedFromHierarchy)
    {
        getLogger().debug("Switching LobbyMusic to MenuMusic Audio");
        UnityEngine::AudioClip* audioClip = menuMusicLoader.getClip();
        self->ambienceAudioClip = audioClip;
    }
    GameServerLobbyFlowCoordinator_DidDeactivate(self, removedFromHierarchy, screenSystemDisabling);
}
// */

// /*
MAKE_HOOK_OFFSETLESS(MultiplayerModeSelectionFlowCoordinator_DidActivate, void, MultiplayerModeSelectionFlowCoordinator* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    getLogger().debug("MultiplayerModeSelectionFlowCoordinator_DidActivate");

    getLogger().info("LobbyMusic is it true: %i", menuMusicLoader.loaded);
    if (menuMusicLoader.loaded && addedToHierarchy)
    {
        getLogger().debug("Switching LobbyMusic to MenuMusic Audio");
        UnityEngine::AudioClip* audioClip = menuMusicLoader.getClip();
        self->ambienceAudioClip = audioClip;
    }
    MultiplayerModeSelectionFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling); // This has to be ran last, otherwise it will not work correctly
}
// /*
MAKE_HOOK_OFFSETLESS(MultiplayerModeSelectionFlowCoordinator_DidDeactivate, void, MultiplayerModeSelectionFlowCoordinator* self, bool removedFromHierarchy, bool screenSystemDisabling)
{
    getLogger().debug("MultiplayerModeSelectionFlowCoordinator_DidDeactivate");

    getLogger().info("LobbyMusic is it true: %i", lobbyAmbienceLoader.loaded);
    if (menuMusicLoader.loaded && removedFromHierarchy)
    {
        UnityEngine::AudioClip* audioClip = menuMusicLoader.getClip();
        self->ambienceAudioClip = audioClip;
    }
    MultiplayerModeSelectionFlowCoordinator_DidDeactivate(self, removedFromHierarchy, screenSystemDisabling);
}
//*/

MAKE_HOOK_OFFSETLESS(NoteCutSoundEffectManager_Start, void, NoteCutSoundEffectManager* self) {
    if(hitSoundLoader.loaded)
    {
        hitSoundArr = createAudioClipArray(hitSoundLoader);
        self->longCutEffectsAudioClips = hitSoundArr;
        self->shortCutEffectsAudioClips = hitSoundArr;
    }
    NoteCutSoundEffectManager_Start(self);
}

MAKE_HOOK_OFFSETLESS(NoteCutSoundEffect_Awake, void, NoteCutSoundEffect* self) {
    if(badHitSoundLoader.loaded)
    {
        badHitSoundArr = createAudioClipArray(badHitSoundLoader);
        self->badCutSoundEffectAudioClips = badHitSoundArr;
    }
    NoteCutSoundEffect_Awake(self);
}

MAKE_HOOK_OFFSETLESS(BasicUIAudioManager_Start, void, BasicUIAudioManager* self) {
    if(menuClickLoader.loaded)
    {
        menuClickArr = createAudioClipArray(menuClickLoader);
        self->clickSounds = menuClickArr;
    }
    BasicUIAudioManager_Start(self);
}

MAKE_HOOK_OFFSETLESS(FireworkItemController_Awake, void, FireworkItemController* self) {
    if(fireworkSoundLoader.loaded)
    {
        fireworkSoundArr = createAudioClipArray(fireworkSoundLoader);
        self->explosionClips = fireworkSoundArr;
    }
    FireworkItemController_Awake(self);
}

MAKE_HOOK_OFFSETLESS(SceneManager_Internal_ActiveSceneChanged, void, UnityEngine::SceneManagement::Scene prevScene, UnityEngine::SceneManagement::Scene nextScene) {
    if (nextScene.IsValid()) {
        std::string sceneName = to_utf8(csstrtostr(nextScene.get_name()));
        getLogger().info("Scene found: %s", sceneName.data());

        std::string shaderWarmup = "ShaderWarmup";
            if(sceneName == shaderWarmup) loadAudioClips();
    }
    SceneManager_Internal_ActiveSceneChanged(prevScene, nextScene);
}

extern "C" void setup(ModInfo &info)
{
    info.id = ID;
    info.version = VERSION;
    modInfo = info;

    getLogger().info("Modloader name: %s", Modloader::getInfo().name.c_str());
    getLogger().debug("Config Path is: %s", getConfig().getConfigFilePath(modInfo).c_str());
    getLogger().info("Loading Config");
    getConfig();
    getLogger().info("Completed setup!");
}  

extern "C" void load()
{
    il2cpp_functions::Init();
    QuestUI::Init();

    Logger& hkLog = getLogger();

    //custom_types::Register::RegisterType<QuestSounds::QSoundsFlowCoordinator>();
    custom_types::Register::RegisterTypes<QuestSounds::QSoundsFlowCoordinator, /*QuestSounds::QSoundsConfigViewController,*/ QuestSounds::QSoundsSdListViewController>();
    QuestUI::Register::RegisterModSettingsFlowCoordinator<QuestSounds::QSoundsFlowCoordinator*>(modInfo);
    //custom_types::Register::RegisterType<QuestSounds::QSoundsViewController>();
    //QuestUI::Register::RegisterModSettingsViewController<QuestSounds::QSoundsViewController*>(modInfo);

    if(!LoadConfig()) SaveConfig();
    makeFolder();

    getLogger().debug("Installing QuestSounds!");
    INSTALL_HOOK_OFFSETLESS(hkLog, SceneManager_Internal_ActiveSceneChanged, il2cpp_utils::FindMethodUnsafe("UnityEngine.SceneManagement", "SceneManager", "Internal_ActiveSceneChanged", 2));
    INSTALL_HOOK_OFFSETLESS(hkLog, SongPreviewPlayer_OnEnable, il2cpp_utils::FindMethodUnsafe("", "SongPreviewPlayer", "OnEnable", 0));
    INSTALL_HOOK_OFFSETLESS(hkLog, NoteCutSoundEffectManager_Start, il2cpp_utils::FindMethodUnsafe("", "NoteCutSoundEffectManager", "Start", 0));
    INSTALL_HOOK_OFFSETLESS(hkLog, NoteCutSoundEffect_Awake, il2cpp_utils::FindMethodUnsafe("", "NoteCutSoundEffect", "Awake", 0));
    INSTALL_HOOK_OFFSETLESS(hkLog, FireworkItemController_Awake, il2cpp_utils::FindMethodUnsafe("", "FireworkItemController", "Awake", 0));
    INSTALL_HOOK_OFFSETLESS(hkLog, BasicUIAudioManager_Start, il2cpp_utils::FindMethodUnsafe("", "BasicUIAudioManager", "Start", 0));
    INSTALL_HOOK_OFFSETLESS(hkLog, ResultsViewController_DidActivate, il2cpp_utils::FindMethodUnsafe("", "ResultsViewController", "DidActivate", 3));
    INSTALL_HOOK_OFFSETLESS(hkLog, MultiplayerModeSelectionFlowCoordinator_DidActivate, il2cpp_utils::FindMethodUnsafe("", "MultiplayerModeSelectionFlowCoordinator", "DidActivate", 3));     // Added for switching out MP Lobby Music
    INSTALL_HOOK_OFFSETLESS(hkLog, MultiplayerModeSelectionFlowCoordinator_DidDeactivate, il2cpp_utils::FindMethodUnsafe("", "MultiplayerModeSelectionFlowCoordinator", "DidDeactivate", 2));  // Added for switching out MP Lobby Music
    INSTALL_HOOK_OFFSETLESS(hkLog, GameServerLobbyFlowCoordinator_DidActivate, il2cpp_utils::FindMethodUnsafe("", "GameServerLobbyFlowCoordinator", "DidActivate", 3));              // Added for switching out MP Lobby Music
    INSTALL_HOOK_OFFSETLESS(hkLog, GameServerLobbyFlowCoordinator_DidDeactivate, il2cpp_utils::FindMethodUnsafe("", "GameServerLobbyFlowCoordinator", "DidDeactivate", 2));          // Added for switching out MP Lobby Music
    getLogger().debug("Installed QuestSounds!");
}
