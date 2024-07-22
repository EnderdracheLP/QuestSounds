#include "main.hpp"
#include "_config.h"
#include "hooking.hpp"
#include "logging.hpp"
#include "Utils/AsyncAudioClipLoader.hpp"
#include "Config.hpp"
#include "AudioClips.hpp"
using namespace QuestSounds::AudioClips;

#include "GlobalNamespace/SongPreviewPlayer.hpp"
#include "GlobalNamespace/BasicUIAudioManager.hpp"

#include "GlobalNamespace/FileHelpers.hpp"
using namespace GlobalNamespace;

#include "System/Action.hpp"

#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/SceneManagement/SceneManager.hpp"
using namespace UnityEngine::SceneManagement;

#include "bsml/shared/BSML.hpp"

// TODO BSML

#include "UI/QuestSoundsFlowCoordinator.hpp"
using namespace QuestSounds;

#define RAPIDJSON_HAS_STDSTRING 1

inline modloader::ModInfo modInfo = {MOD_ID, VERSION, 0};

// TODO: Proper paper logging
Paper::ConstLoggerContext<12UL>& getLogger() {
    static auto fastContext = Paper::Logger::WithContext<MOD_ID>();
    return fastContext;
}

void makeFolder() 
{    
    if (!direxists(Config.Sounds.MenuMusic.FolderPath.c_str()))
    {
        int makePath = mkpath(Config.Sounds.MenuMusic.FolderPath.c_str());
        if (makePath == -1)
        {
            getLogger().error("Failed to make MenuMusic Folder path!");
        }
    }

    if (!direxists(Config.Sounds.HitSound.FolderPath.c_str()))
    {
        int makePath = mkpath(Config.Sounds.HitSound.FolderPath.c_str());
        if (makePath == -1)
        {
            getLogger().error("Failed to make HitSound Folder path!");
        }
    }

    if (!direxists(Config.Sounds.BadHitSound.FolderPath.c_str()))
    {
        int makePath = mkpath(Config.Sounds.BadHitSound.FolderPath.c_str());
            if (makePath == -1)
            {
                getLogger().error("Failed to make BadHitSound Folder path!");
            }
    }

    if (!direxists(Config.Sounds.NoteMissedSound.FolderPath.c_str()))
    {
        int makePath = mkpath(Config.Sounds.NoteMissedSound.FolderPath.c_str());
        if (makePath == -1)
        {
            getLogger().error("Failed to make NoteMissedSound Folder path!");
        }
    }

    if (!direxists(Config.Sounds.MenuClick.FolderPath.c_str()))
    {
        int makePath = mkpath(Config.Sounds.MenuClick.FolderPath.c_str());
        if (makePath == -1)
        {
            getLogger().error("Failed to make MenuClick Folder path!");
        }
    }

    if (!direxists(Config.Sounds.Firework.FolderPath.c_str()))
    {
        int makePath = mkpath(Config.Sounds.Firework.FolderPath.c_str());
        if (makePath == -1)
        {
            getLogger().error("Failed to make FireworkSound Folder path!");
        }
    }

    if (!direxists(Config.Sounds.LevelCleared.FolderPath.c_str()))
    {
        int makePath = mkpath(Config.Sounds.LevelCleared.FolderPath.c_str());
        if (makePath == -1)
        {
            getLogger().error("Failed to make LevelCleared Folder path!");
        }
    }

    if (!direxists(Config.Sounds.LevelFailed.FolderPath.c_str()))
    {
        int makePath = mkpath(Config.Sounds.LevelFailed.FolderPath.c_str());
        if (makePath == -1)
        {
            getLogger().error("Failed to make LevelFailed Folder path!");
        }
    }

    if (!direxists(Config.Sounds.LobbyMusic.FolderPath.c_str()))
    {
        int makePath = mkpath(Config.Sounds.LobbyMusic.FolderPath.c_str());
        if (makePath == -1)
        {
            getLogger().error("Failed to make LobbyMusic Folder path!");
        }
    }
}



namespace QuestSounds::AudioClips {
/*audioClipLoader::loader*/
QuestSounds::Utils::AsyncAudioClipLoader    hitSoundLoader,     // hitSound
                                            badHitSoundLoader,  // badHitSound
                                            noteMissedSoundLoader,
                                            menuMusicLoader,    // menuMusic
                                            menuClickLoader,
                                            fireworkSoundLoader,
                                            levelClearedLoader,
                                            levelFailedLoader,  // For LevelFailed sound
                                            lobbyAmbienceLoader;    // Added for LobbyMusic
::ArrayW<::UnityW<::UnityEngine::AudioClip>>    hitSoundArr,    // hitSoundArray
                                                badHitSoundArr, // badHitSoundArray
                                                menuClickArr,
                                                fireworkSoundArr;


::ArrayW<::UnityW<::UnityEngine::AudioClip>> origMenuClickArr;

    void loadAudioClips()
    {
        hitSoundLoader.filePath = Config.Sounds.HitSound.FilePath;
        badHitSoundLoader.filePath = Config.Sounds.BadHitSound.FilePath;
        noteMissedSoundLoader.filePath = Config.Sounds.NoteMissedSound.FilePath;
        menuMusicLoader.filePath = Config.Sounds.MenuMusic.FilePath;
        menuClickLoader.filePath = Config.Sounds.MenuClick.FilePath;
        fireworkSoundLoader.filePath = Config.Sounds.Firework.FilePath;
        levelClearedLoader.filePath = Config.Sounds.LevelCleared.FilePath;
        levelFailedLoader.filePath = Config.Sounds.LevelFailed.FilePath;
        lobbyAmbienceLoader.filePath = Config.Sounds.LobbyMusic.FilePath; // Added for LobbyMusic

        menuMusicLoader.load();
        menuClickLoader.load();
        if (Config.Sounds.HitSound.Active) hitSoundLoader.load();
        if (Config.Sounds.BadHitSound.Active) badHitSoundLoader.load();
        if (Config.Sounds.NoteMissedSound.Active) noteMissedSoundLoader.load();
        if (Config.Sounds.Firework.Active) fireworkSoundLoader.load();
        if (Config.Sounds.LevelCleared.Active) levelClearedLoader.load();
        if (Config.Sounds.LevelFailed.Active) levelFailedLoader.load();
        if (Config.Sounds.LobbyMusic.Active) lobbyAmbienceLoader.load();    // Added for LobbyMusic
    }
    // Creates an Array, of AudioClips
    ::ArrayW<::UnityW<::UnityEngine::AudioClip>> createAudioClipArray(QuestSounds::Utils::AsyncAudioClipLoader clipLoader, bool GetOriginalClip)
    {
        UnityEngine::AudioClip* tempClip;
        if (!GetOriginalClip) tempClip = clipLoader.getClip();
        else tempClip = clipLoader.get_OriginalClip();
        auto temporaryArray = ArrayW<::UnityW<UnityEngine::AudioClip>>({UnityW<UnityEngine::AudioClip>(tempClip)});
        return temporaryArray;
    }
}

#ifdef DEBUG
MAKE_HOOK_MATCH(FileHelpers_GetEscapedURLForFilePath, &FileHelpers::GetEscapedURLForFilePath, StringW, StringW filePath) {
    return StringW("file://" + filePath);
}
#endif


MAKE_HOOK_MATCH(BasicUIAudioManager_Start, &BasicUIAudioManager::Start, void, BasicUIAudioManager* self) {
    if (!menuClickLoader.OriginalAudioSource) menuClickLoader.set_OriginalClip(self->_clickSounds[0]);

    if(menuClickLoader.loaded && Config.Sounds.MenuClick.Active)
    {
        menuClickArr = createAudioClipArray(menuClickLoader);
        self->_clickSounds = menuClickArr;
    }
    BasicUIAudioManager_Start(self);
}

MAKE_HOOK_MATCH(SceneManager_Internal_ActiveSceneChanged, &SceneManager::Internal_ActiveSceneChanged, void, Scene previousActiveScene, Scene newActiveScene) {
    SceneManager_Internal_ActiveSceneChanged(previousActiveScene, newActiveScene);
    if (newActiveScene.IsValid()) {
        std::string sceneName = newActiveScene.get_name();
        getLogger().info("Scene found: {}", sceneName.data());

        std::string questInit = "QuestInit";
        if(sceneName == questInit) {
            try { 
                QuestSounds::AudioClips::loadAudioClips();
            }
            catch (const std::exception& e) {
                getLogger().error("Failed to load AudioClips: {}", e.what());
            }
        }
    }
}

std::string& GetConfigPath() {
    static std::string configPath = Configuration::getConfigFilePath(modInfo);
    return configPath;
}

QS_EXPORT_FUNC void setup(CModInfo *info) {
    info->id = MOD_ID;
    info->version = VERSION;


    Paper::Logger::RegisterFileContextId(getLogger().tag);

    getLogger().info("Modloader: {}", modloader::get_modloader_path().c_str());

    auto& configPath = GetConfigPath();
    getLogger().debug("Config Path is: {}", configPath);

    getLogger().info("Loading Config from file {}", configPath);
    if(!fileexists(configPath)) {
        getLogger().info("Config file not found, creating default config");
        if(!WriteToFile(configPath, Config, true))
            getLogger().error("Failed to write config file!");
    }

    try {
        ReadFromFile(configPath, Config);
    } catch(const std::exception& e) {
        getLogger().error("Failed to read config: {}", e.what());
    }

    getLogger().info("Completed setup!");
}  

QS_EXPORT_FUNC void late_load()
{
    il2cpp_functions::Init();
    BSML::Init();
    custom_types::Register::AutoRegister();
    BSML::Register::RegisterMainMenu<QuestSounds::UI::QuestSoundsFlowCoordinator*>("QuestSounds", "Let's you customize sounds and music");

    auto& hkLog = getLogger();

    makeFolder();
    getLogger().debug("Installing QuestSounds!");
    QuestSounds::Hooking::InstallHooks();
    INSTALL_HOOK(hkLog, SceneManager_Internal_ActiveSceneChanged);
    // INSTALL_HOOK(hkLog, PauseMenuManager_MenuButtonPressed); --> NoteSoundHooks.cpp
    // INSTALL_HOOK(hkLog, SongPreviewPlayer_OnEnable); //      --> BackgroundMusicHooks.cpp
    // INSTALL_HOOK(hkLog, NoteCutSoundEffectManager_Start);    --> NoteSoundHooks.cpp
    // INSTALL_HOOK(hkLog, NoteCutSoundEffect_Awake);           --> NoteSoundHooks.cpp
    // INSTALL_HOOK(hkLog, BeatmapObjectManager_HandleNoteWasMissed);
    INSTALL_HOOK(hkLog, BasicUIAudioManager_Start);
    // INSTALL_HOOK(hkLog, ResultsViewController_DidActivate);          --> LevelResultSoundHooks.cpp
    // INSTALL_HOOK(hkLog, ResultsViewController_RestartButtonPressed); --> LevelResultSoundHooks.cpp
    // INSTALL_HOOK(hkLog, FireworkItemController_PlayExplosionSound);  --> LevelResultSoundHooks.cpp
    // INSTALL_HOOK(hkLog, MultiplayerModeSelectionFlowCoordinator_DidActivate);     // Added for switching out MP Lobby Music --> BackgroundMusicHooks.cpp
    // INSTALL_HOOK(hkLog, MultiplayerModeSelectionFlowCoordinator_DidDeactivate);  // Added for switching out MP Lobby Music  --> BackgroundMusicHooks.cpp
    // INSTALL_HOOK(hkLog, GameServerLobbyFlowCoordinator_DidActivate);              // Added for switching out MP Lobby Music --> BackgroundMusicHooks.cpp
    // INSTALL_HOOK(hkLog, GameServerLobbyFlowCoordinator_DidDeactivate);          // Added for switching out MP Lobby Music   --> BackgroundMusicHooks.cpp
    // #ifdef DEBUG
    // auto ModList = Modloader::getMods();
    // if (ModList.find("SongLoader") == ModList.end()) {
    //     getLogger().info("SongLoader missing, installing FilePath check");
    //     INSTALL_HOOK(hkLog, FileHelpers_GetEscapedURLForFilePath);
    // }
    // #endif
    getLogger().debug("Installed QuestSounds!");
}
