#if defined(BS__1_16)
#elif defined(BS_1_13_2)
#else
#error Define BS Version
#endif

#include "main.hpp"
#include "Utils/AsyncAudiocliploader.hpp"
#include "QSoundsConfig.hpp"
using namespace QSoundsConfig;
#include "AudioClips.hpp"
using namespace QuestSounds::AudioClips;

#include "GlobalNamespace/ResultsViewController.hpp"
#include "GlobalNamespace/SongPreviewPlayer.hpp"
#include "GlobalNamespace/NoteCutSoundEffectManager.hpp"
#include "GlobalNamespace/NoteCutSoundEffect.hpp"
#include "GlobalNamespace/BasicUIAudioManager.hpp"
#include "GlobalNamespace/FireworkItemController.hpp"
#include "GlobalNamespace/GameServerLobbyFlowCoordinator.hpp"
#include "GlobalNamespace/MultiplayerModeSelectionFlowCoordinator.hpp"

#include "GlobalNamespace/FileHelpers.hpp"

#include "GlobalNamespace/LevelCompletionResults.hpp"
#include "GlobalNamespace/SongPreviewPlayer_AudioSourceVolumeController.hpp"
using namespace GlobalNamespace;

#include "System/Action.hpp"

#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/SceneManagement/SceneManager.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/WaitForSeconds.hpp"
#include "UnityEngine/Time.hpp"
using namespace UnityEngine::SceneManagement;

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"
using namespace QuestUI;

#include "ViewControllers/MenuSdListViewController.hpp"
#include "ViewControllers/HitSdListViewController.hpp"
#include "ViewControllers/MenuClickSdListViewController.hpp"
#include "ViewControllers/BadHitSdListViewController.hpp"
#include "ViewControllers/FireworkSdListViewController.hpp"
#include "ViewControllers/LevelClearedSdListViewController.hpp"
#include "ViewControllers/LobbyMusicSdListViewController.hpp"
#include "ViewControllers/ConfigViewController.hpp"
#include "QSoundsFlowCoordinator.hpp"
using namespace QuestSounds;

#include "custom-types/shared/register.hpp"
using namespace custom_types;

#if defined(MAKE_HOOK_OFFSETLESS) && !defined(MAKE_HOOK_MATCH)
#define QS_MAKE_HOOK(name, mPtr, retval, ...) MAKE_HOOK_OFFSETLESS(name, retval, __VA_ARGS__)
#define QS_INSTALL_HOOK(logger, name, methodInfo) INSTALL_HOOK_OFFSETLESS(logger, name, methodInfo)
#elif defined(MAKE_HOOK_MATCH)
#define QS_MAKE_HOOK(name, mPtr, retval, ...) MAKE_HOOK_MATCH(name, mPtr, retval, __VA_ARGS__)
#define QS_INSTALL_HOOK(logger, name, methodInfo) INSTALL_HOOK(logger, name)
#else
#error No Compatible HOOK macro found
#endif

#define RAPIDJSON_HAS_STDSTRING 1

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

void makeFolder() 
{
    //if(!direxists(soundPath.c_str()))
    //{
    //    int makePath = mkpath(soundPath.c_str());
    //    if(makePath == -1)
    //    {
    //        getLogger().debug("Failed to make path!");
    //    }
    //}
    
    if (!direxists(MenuMusicPath.c_str()))
    {
        int makePath = mkpath(MenuMusicPath.c_str());
        if (makePath == -1)
        {
            getLogger().error("Failed to make MenuMusic Folder path!");
        }
    }

    if (!direxists(HitSoundPath.c_str()))
    {
        int makePath = mkpath(HitSoundPath.c_str());
        if (makePath == -1)
        {
            getLogger().error("Failed to make HitSound Folder path!");
        }
    }

    if (!direxists(BadHitSoundPath.c_str()))
    {
        int makePath = mkpath(BadHitSoundPath.c_str());
            if (makePath == -1)
            {
                getLogger().error("Failed to make BadHitSound Folder path!");
            }
    }    

    if (!direxists(MenuClickPath.c_str()))
    {
        int makePath = mkpath(MenuClickPath.c_str());
        if (makePath == -1)
        {
            getLogger().error("Failed to make MenuClick Folder path!");
        }
    }

    if (!direxists(FireworkSoundPath.c_str()))
    {
        int makePath = mkpath(FireworkSoundPath.c_str());
        if (makePath == -1)
        {
            getLogger().error("Failed to make FireworkSound Folder path!");
        }
    }

    if (!direxists(LevelClearedPath.c_str()))
    {
        int makePath = mkpath(LevelClearedPath.c_str());
        if (makePath == -1)
        {
            getLogger().error("Failed to make LevelCleared Folder path!");
        }
    }

    if (!direxists(LevelFailedPath.c_str()))
    {
        int makePath = mkpath(LevelFailedPath.c_str());
        if (makePath == -1)
        {
            getLogger().error("Failed to make LevelFailed Folder path!");
        }
    }

#ifndef BS__1_13_2
    if (!direxists(LobbyMusicPath.c_str()))
    {
        int makePath = mkpath(LobbyMusicPath.c_str());
        if (makePath == -1)
        {
            getLogger().error("Failed to make LevelCleared Folder path!");
        }
    }
#endif
}



namespace QuestSounds::AudioClips {
/*audioClipLoader::loader*/
AsyncAudioClipLoader::loader    hitSoundLoader,     // hitSound
                                badHitSoundLoader,  // badHitSound
                                menuMusicLoader,    // menuMusic
                                menuClickLoader,
                                fireworkSoundLoader,
                                levelClearedLoader,
                                levelFailedLoader,  // For LevelFailed sound
#ifndef BS__1_13_2
                                lobbyAmbienceLoader;    // Added for LobbyMusic
#endif
Array<UnityEngine::AudioClip*> *hitSoundArr,    // hitSoundArray
                               *badHitSoundArr, // badHitSoundArray
                               *menuClickArr,
                               *fireworkSoundArr;


Array<UnityEngine::AudioClip*> *origMenuClickArr;


    void loadAudioClips()
    {
        hitSoundLoader.filePath = Config.hitSound_filepath;
        badHitSoundLoader.filePath = Config.badHitSound_filepath;
        menuMusicLoader.filePath = Config.menuMusic_filepath;
        menuClickLoader.filePath = Config.menuClick_filepath;
        fireworkSoundLoader.filePath = Config.firework_filepath;
        levelClearedLoader.filePath = Config.levelCleared_filepath;
        levelFailedLoader.filePath = Config.levelFailed_filepath;
#ifndef BS__1_13_2
        lobbyAmbienceLoader.filePath = Config.lobbyAmbience_filepath; // Added for LobbyMusic
#endif

        menuMusicLoader.load();
        menuClickLoader.load();
        if (Config.hitSound_Active) hitSoundLoader.load();
        if (Config.badHitSound_Active) badHitSoundLoader.load();
        if (Config.firework_Active) fireworkSoundLoader.load();
        if (Config.levelCleared_Active) levelClearedLoader.load();
        if (Config.levelFailed_Active) levelFailedLoader.load();
#ifndef BS__1_13_2
        if (Config.lobbyAmbience_Active) lobbyAmbienceLoader.load();    // Added for LobbyMusic
#endif
    }
    // Creates an Array, of AudioClips
    Array<UnityEngine::AudioClip*>* createAudioClipArray(AsyncAudioClipLoader::loader clipLoader, bool GetOriginalClip)
    {
        UnityEngine::AudioClip* tempClip;
        if (!GetOriginalClip) tempClip = clipLoader.getClip();
        else tempClip = clipLoader.get_OriginalClip();
        auto* temporaryArray = Array<UnityEngine::AudioClip*>::New(tempClip);
        //temporaryArray->values[0] = tempClip;
        return temporaryArray;
    }
}

QS_MAKE_HOOK(ResultsViewController_DidActivate, &ResultsViewController::DidActivate, void, ResultsViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (firstActivation && addedToHierarchy && !levelClearedLoader.OriginalAudioSource) {
        levelClearedLoader.set_OriginalClip(self->levelClearedAudioClip);
    }
    if (self->levelCompletionResults->levelEndStateType == LevelCompletionResults::LevelEndStateType::Failed) {
        self->songPreviewPlayer->StopAllCoroutines();
        if (levelFailedLoader.loaded && addedToHierarchy && QSoundsConfig::Config.levelFailed_Active) {
            UnityEngine::AudioClip* FailedSound = levelFailedLoader.getClip();
            float length = FailedSound->get_length();
            getLogger().debug("Duration of LevelFailed Sound: %f", length);
            if (length > 8.0f) {
                getLogger().debug("Long LevelFailedSound");
                self->songPreviewPlayer->CrossfadeTo(FailedSound, -4.0f, 0.0f, length);
            }
            else {
                getLogger().debug("Short LevelFailedSound");
                self->songPreviewPlayer->FadeOut(0.1f);
                self->songPreviewPlayer->fadeSpeed = self->songPreviewPlayer->fadeInSpeed;
                self->songPreviewPlayer->StartCoroutine(self->songPreviewPlayer->CrossFadeAfterDelayCoroutine(length - 1.2f));
                levelFailedLoader.audioSource->Play();
            }
        }
    }
    else {
        if (levelClearedLoader.loaded && addedToHierarchy && QSoundsConfig::Config.levelCleared_Active)
        {
            UnityEngine::AudioClip* audioClip = levelClearedLoader.getClip();
            self->levelClearedAudioClip = audioClip;
        }
        else {
            self->levelClearedAudioClip = levelClearedLoader.get_OriginalClip();
        }
    }
    ResultsViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
}

QS_MAKE_HOOK(SongPreviewPlayer_OnEnable, &SongPreviewPlayer::OnEnable, void, SongPreviewPlayer* self) {
    getLogger().info("is it true: %i", menuMusicLoader.loaded);

    if (!menuMusicLoader.OriginalAudioSource) {
        menuMusicLoader.set_OriginalClip(self->defaultAudioClip);
    }

    if (menuMusicLoader.loaded && QSoundsConfig::Config.menuMusic_Active)
    {
        UnityEngine::AudioClip* audioClip = menuMusicLoader.getClip();
        if (audioClip != nullptr)
            self->defaultAudioClip = audioClip;
    }
    else {
        self->defaultAudioClip = menuMusicLoader.get_OriginalClip();
    }
    SongPreviewPlayer_OnEnable(self);
}

#ifndef BS__1_13_2
QS_MAKE_HOOK(GameServerLobbyFlowCoordinator_DidActivate, &GameServerLobbyFlowCoordinator::DidActivate, void, GameServerLobbyFlowCoordinator* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    getLogger().debug("GameServerLobbyFlowCoordinator_DidActivate");
    if (!lobbyAmbienceLoader.OriginalAudioSource) lobbyAmbienceLoader.set_OriginalClip(self->ambienceAudioClip);

    if (lobbyAmbienceLoader.loaded && QSoundsConfig::Config.lobbyAmbience_Active)
    {
        getLogger().debug("Overwriting LobbyAmbience Audio");
        self->ambienceAudioClip = lobbyAmbienceLoader.getClip();
    }
    else {
        self->ambienceAudioClip = lobbyAmbienceLoader.get_OriginalClip();
    }
    GameServerLobbyFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
}

QS_MAKE_HOOK(GameServerLobbyFlowCoordinator_DidDeactivate, &GameServerLobbyFlowCoordinator::DidDeactivate, void, GameServerLobbyFlowCoordinator* self, bool removedFromHierarchy, bool screenSystemDisabling)
{
    getLogger().debug("GameServerLobbyFlowCoordinator_DidActivate");
    if (menuMusicLoader.loaded && QSoundsConfig::Config.menuMusic_Active && removedFromHierarchy)
    {
        getLogger().debug("Switching LobbyMusic to MenuMusic Audio");
        self->ambienceAudioClip = menuMusicLoader.getClip();
    }
    else {
        self->ambienceAudioClip = menuMusicLoader.get_OriginalClip();
    }
    GameServerLobbyFlowCoordinator_DidDeactivate(self, removedFromHierarchy, screenSystemDisabling);
}

QS_MAKE_HOOK(MultiplayerModeSelectionFlowCoordinator_DidActivate, &MultiplayerModeSelectionFlowCoordinator::DidActivate, void, MultiplayerModeSelectionFlowCoordinator* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    getLogger().debug("MultiplayerModeSelectionFlowCoordinator_DidActivate");
    if (menuMusicLoader.loaded && QSoundsConfig::Config.menuMusic_Active)
    {
        getLogger().debug("Switching LobbyMusic to MenuMusic Audio");
        self->ambienceAudioClip = menuMusicLoader.getClip();
    }
    else {
        self->ambienceAudioClip = menuMusicLoader.get_OriginalClip();
    }
    MultiplayerModeSelectionFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling); // This has to be ran last, otherwise it will not work correctly
}

QS_MAKE_HOOK(MultiplayerModeSelectionFlowCoordinator_DidDeactivate, &MultiplayerModeSelectionFlowCoordinator::DidDeactivate, void, MultiplayerModeSelectionFlowCoordinator* self, bool removedFromHierarchy, bool screenSystemDisabling)
{
    getLogger().debug("MultiplayerModeSelectionFlowCoordinator_DidDeactivate");
    if (menuMusicLoader.loaded && QSoundsConfig::Config.menuMusic_Active && removedFromHierarchy)
    {
        self->ambienceAudioClip = menuMusicLoader.getClip();
    }
    else {
        self->ambienceAudioClip = menuMusicLoader.get_OriginalClip();
    }

    MultiplayerModeSelectionFlowCoordinator_DidDeactivate(self, removedFromHierarchy, screenSystemDisabling);
}
#endif

QS_MAKE_HOOK(FileHelpers_GetEscapedURLForFilePath, &FileHelpers::GetEscapedURLForFilePath, Il2CppString*, Il2CppString* filePath) {
    return il2cpp_utils::newcsstr(std::u16string(u"file://") + std::u16string(csstrtostr(filePath)));
}


QS_MAKE_HOOK(NoteCutSoundEffectManager_Start, &NoteCutSoundEffectManager::Start, void, NoteCutSoundEffectManager* self) {
    if(hitSoundLoader.loaded && QSoundsConfig::Config.hitSound_Active)
    {
        hitSoundArr = createAudioClipArray(hitSoundLoader);
        self->longCutEffectsAudioClips = hitSoundArr;
        self->shortCutEffectsAudioClips = hitSoundArr;
        getLogger().debug("NoteCutSoundEffectManager_Start: Loaded hitSoundArray");
    }
    else {
        getLogger().debug("NoteCutSoundEffectManager_Start: Loading normally");
    }
    getLogger().debug("audioSamplesBeatAlignOffset was: %f", self->audioSamplesBeatAlignOffset);
    self->audioSamplesBeatAlignOffset = QSoundsConfig::Config.hitSound_beatOffSet;
    getLogger().debug("audioSamplesBeatAlignOffset changed to: %f", self->audioSamplesBeatAlignOffset);
    NoteCutSoundEffectManager_Start(self);
    getLogger().debug("Beatalign offset is: %f", self->beatAlignOffset);
}

QS_MAKE_HOOK(NoteCutSoundEffect_Awake, &NoteCutSoundEffect::Awake, void, NoteCutSoundEffect* self) {
    if(badHitSoundLoader.loaded && QSoundsConfig::Config.badHitSound_Active)
    {
        badHitSoundArr = createAudioClipArray(badHitSoundLoader);
        self->badCutSoundEffectAudioClips = badHitSoundArr;
    }
    NoteCutSoundEffect_Awake(self);
}

QS_MAKE_HOOK(BasicUIAudioManager_Start, &BasicUIAudioManager::Start, void, BasicUIAudioManager* self) {
    if (!menuClickLoader.OriginalAudioSource) menuClickLoader.set_OriginalClip(self->clickSounds->values[0]);

    if(menuClickLoader.loaded && QSoundsConfig::Config.menuClick_Active)
    {
        menuClickArr = createAudioClipArray(menuClickLoader);
        self->clickSounds = menuClickArr;
    }
    BasicUIAudioManager_Start(self);
}

//QS_MAKE_HOOK(FireworkItemController_Awake, &FireworkItemController::Awake, void, FireworkItemController* self) {
//    if (!origFireworkSoundArr) origFireworkSoundArr = self->explosionClips;
//    
//    if(fireworkSoundLoader.loaded && QSoundsConfig::Config.firework_Active)
//    {
//        getLogger().debug("Setting custom Fireworks Sounds");
//        fireworkSoundArr = createAudioClipArray(fireworkSoundLoader);
//        self->explosionClips = fireworkSoundArr;
//    }
//    else {
//        getLogger().debug("Return default Fireworks Sounds");
//        self->explosionClips = origFireworkSoundArr;
//    }
//    FireworkItemController_Awake(self);
//}

// Replacing the function here, as replacing the AudioClips proves to be difficult
QS_MAKE_HOOK(FireworkItemController_PlayExplosionSound, &FireworkItemController::PlayExplosionSound, void, FireworkItemController* self) {
    if (fireworkSoundLoader.loaded && QSoundsConfig::Config.firework_Active) {
        self->audioSource->set_clip(fireworkSoundLoader.getClip());
        float pitch = 1.2f + (((float)rand()) / (float)RAND_MAX) * (0.8f - 1.2f);
        self->audioSource->set_pitch(pitch);
        self->audioSource->Play();
    }
    else FireworkItemController_PlayExplosionSound(self);
}

QS_MAKE_HOOK(SceneManager_Internal_ActiveSceneChanged, &SceneManager::Internal_ActiveSceneChanged, void, Scene previousActiveScene, Scene newActiveScene) {
    SceneManager_Internal_ActiveSceneChanged(previousActiveScene, newActiveScene);
    if (newActiveScene.IsValid()) {
        std::string sceneName = to_utf8(csstrtostr(newActiveScene.get_name()));
        getLogger().info("Scene found: %s", sceneName.data());

        std::string shaderWarmup = "ShaderWarmup";
            if(sceneName == shaderWarmup) QuestSounds::AudioClips::loadAudioClips();
    }
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
#ifndef REGISTER_FUNCTION
    custom_types::Register::AutoRegister();
#else
    custom_types::Register::RegisterTypes<
        QuestSounds::QSoundsFlowCoordinator,
        QuestSounds::ConfigViewController,
        QuestSounds::MenuSdListViewController,
        QuestSounds::HitSdListViewController,
        QuestSounds::MenuClickSdListViewController,
        QuestSounds::BadHitSdListViewController,
        QuestSounds::FireworkSdListViewController,
        QuestSounds::LevelClearedSdListViewController>();
#ifndef BS__1_13_2
    custom_types::Register::RegisterType<QuestSounds::LobbyMusicSdListViewController>();
#endif
#endif
    QuestUI::Register::RegisterModSettingsFlowCoordinator<QuestSounds::QSoundsFlowCoordinator*>(modInfo);

    if(!LoadConfig()) SaveConfig();
    makeFolder();
    //Modloader::getMods()
    getLogger().debug("Installing QuestSounds!");
    QS_INSTALL_HOOK(hkLog, SceneManager_Internal_ActiveSceneChanged, il2cpp_utils::FindMethodUnsafe("UnityEngine.SceneManagement", "SceneManager", "Internal_ActiveSceneChanged", 2));
    QS_INSTALL_HOOK(hkLog, SongPreviewPlayer_OnEnable, il2cpp_utils::FindMethodUnsafe("", "SongPreviewPlayer", "OnEnable", 0));
    QS_INSTALL_HOOK(hkLog, NoteCutSoundEffectManager_Start, il2cpp_utils::FindMethodUnsafe("", "NoteCutSoundEffectManager", "Start", 0));
    //INSTALL_HOOK_OFFSETLESS(hkLog, NoteCutSoundEffectManager_HandleNoteWasCut, il2cpp_utils::FindMethodUnsafe("", "NoteCutSoundEffectManager", "HandleNoteWasCut", 2));
    QS_INSTALL_HOOK(hkLog, NoteCutSoundEffect_Awake, il2cpp_utils::FindMethodUnsafe("", "NoteCutSoundEffect", "Awake", 0));
    //QS_INSTALL_HOOK(hkLog, FireworkItemController_Awake, il2cpp_utils::FindMethodUnsafe("", "FireworkItemController", "Awake", 0));
    QS_INSTALL_HOOK(hkLog, BasicUIAudioManager_Start, il2cpp_utils::FindMethodUnsafe("", "BasicUIAudioManager", "Start", 0));
    QS_INSTALL_HOOK(hkLog, ResultsViewController_DidActivate, il2cpp_utils::FindMethodUnsafe("", "ResultsViewController", "DidActivate", 3));
    QS_INSTALL_HOOK(hkLog, FireworkItemController_PlayExplosionSound, il2cpp_utils::FindMethodUnsafe("", "FireworkItemController", "PlayExplosionSound", 0))
#ifndef BS__1_13_2
    QS_INSTALL_HOOK(hkLog, MultiplayerModeSelectionFlowCoordinator_DidActivate, il2cpp_utils::FindMethodUnsafe("", "MultiplayerModeSelectionFlowCoordinator", "DidActivate", 3));     // Added for switching out MP Lobby Music
    QS_INSTALL_HOOK(hkLog, MultiplayerModeSelectionFlowCoordinator_DidDeactivate, il2cpp_utils::FindMethodUnsafe("", "MultiplayerModeSelectionFlowCoordinator", "DidDeactivate", 2));  // Added for switching out MP Lobby Music
    QS_INSTALL_HOOK(hkLog, GameServerLobbyFlowCoordinator_DidActivate, il2cpp_utils::FindMethodUnsafe("", "GameServerLobbyFlowCoordinator", "DidActivate", 3));              // Added for switching out MP Lobby Music
    QS_INSTALL_HOOK(hkLog, GameServerLobbyFlowCoordinator_DidDeactivate, il2cpp_utils::FindMethodUnsafe("", "GameServerLobbyFlowCoordinator", "DidDeactivate", 2));          // Added for switching out MP Lobby Music
#endif
    auto ModList = Modloader::getMods();
    if (ModList.find("SongLoader") == ModList.end()) {
        getLogger().info("SongLoader missing, installing FilePath check");
        QS_INSTALL_HOOK(hkLog, FileHelpers_GetEscapedURLForFilePath, il2cpp_utils::FindMethodUnsafe("", "FileHelpers", "GetEscapedURLForFilePath", 1));
    }

    getLogger().debug("Installed QuestSounds!");
}
