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
#include "GlobalNamespace/NoteController.hpp"
#include "GlobalNamespace/NoteData.hpp"
#include "GlobalNamespace/BasicUIAudioManager.hpp"
#include "GlobalNamespace/FireworkItemController.hpp"
#include "GlobalNamespace/GameServerLobbyFlowCoordinator.hpp"
#include "GlobalNamespace/MultiplayerModeSelectionFlowCoordinator.hpp"
#include "GlobalNamespace/BeatmapObjectManager.hpp"
#include "GlobalNamespace/PauseMenuManager.hpp"

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
#elif defined(MAKE_HOOK_MATCH)
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

    if (!direxists(NoteMissedSoundPath.c_str()))
    {
        int makePath = mkpath(NoteMissedSoundPath.c_str());
        if (makePath == -1)
        {
            getLogger().error("Failed to make NoteMissedSound Folder path!");
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
            getLogger().error("Failed to make LobbyMusic Folder path!");
        }
    }
#endif
}



namespace QuestSounds::AudioClips {
/*audioClipLoader::loader*/
AsyncAudioClipLoader::loader    hitSoundLoader,     // hitSound
                                badHitSoundLoader,  // badHitSound
                                noteMissedSoundLoader,
                                menuMusicLoader,    // menuMusic
                                menuClickLoader,
                                fireworkSoundLoader,
                                levelClearedLoader,
                                levelFailedLoader,  // For LevelFailed sound
#ifndef BS__1_13_2
                                lobbyAmbienceLoader;    // Added for LobbyMusic
#endif
Array<UnityEngine::AudioClip*> * hitSoundArr,    // hitSoundArray
                               * badHitSoundArr, // badHitSoundArray
                               * menuClickArr,
                               * fireworkSoundArr;


Array<UnityEngine::AudioClip*>* origMenuClickArr;

    void loadAudioClips()
    {
        hitSoundLoader.filePath = Config.hitSound_filepath;
        badHitSoundLoader.filePath = Config.badHitSound_filepath;
        noteMissedSoundLoader.filePath = Config.noteMissedSound_filepath;
        menuMusicLoader.filePath = Config.menuMusic_filepath;
        menuClickLoader.filePath = Config.menuClick_filepath;
        fireworkSoundLoader.filePath = Config.firework_filepath;
        levelClearedLoader.filePath = Config.levelCleared_filepath;
        levelFailedLoader.filePath = Config.levelFailed_filepath;
        lobbyAmbienceLoader.filePath = Config.lobbyAmbience_filepath; // Added for LobbyMusic

        menuMusicLoader.load();
        menuClickLoader.load();
        if (Config.hitSound_Active) hitSoundLoader.load();
        if (Config.badHitSound_Active) badHitSoundLoader.load();
        if (Config.noteMissedSound_Active) noteMissedSoundLoader.load();
        if (Config.firework_Active) fireworkSoundLoader.load();
        if (Config.levelCleared_Active) levelClearedLoader.load();
        if (Config.levelFailed_Active) levelFailedLoader.load();
        if (Config.lobbyAmbience_Active) lobbyAmbienceLoader.load();    // Added for LobbyMusic
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
        //return ArrayW<UnityEngine::AudioClip*>({ tempClip });
    }
}

MAKE_HOOK_MATCH(ResultsViewController_DidActivate, &ResultsViewController::DidActivate, void, ResultsViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (firstActivation && addedToHierarchy && !levelClearedLoader.OriginalAudioSource) {
        levelClearedLoader.set_OriginalClip(self->dyn__levelClearedAudioClip());
    }
    if (self->dyn__levelCompletionResults()->dyn_levelEndStateType() == LevelCompletionResults::LevelEndStateType::Failed) {
        self->dyn__songPreviewPlayer()->StopAllCoroutines();
        if (levelFailedLoader.loaded && addedToHierarchy && QSoundsConfig::Config.levelFailed_Active) {
            UnityEngine::AudioClip* FailedSound = levelFailedLoader.getClip();
            float length = FailedSound->get_length();
            getLogger().debug("Duration of LevelFailed Sound: %f", length);
            if (length > 7.0f) {
                getLogger().info("Long LevelFailedSound");
                self->dyn__songPreviewPlayer()->CrossfadeTo(FailedSound, -4.0f, 0.0f, length, nullptr);
            }
            else {
                getLogger().info("Short LevelFailedSound");
                self->dyn__songPreviewPlayer()->FadeOut(0.1f);
                self->dyn__songPreviewPlayer()->dyn__fadeSpeed() = self->dyn__songPreviewPlayer()->dyn__fadeInSpeed();
                getLogger().debug("volume: %f", self->dyn__songPreviewPlayer()->dyn__volume());
                getLogger().debug("AmbientVolume: %f", self->dyn__songPreviewPlayer()->dyn__ambientVolumeScale());
                getLogger().debug("Set Volume: %f", self->dyn__songPreviewPlayer()->dyn__volume() * self->dyn__songPreviewPlayer()->dyn__ambientVolumeScale());

                levelFailedLoader.audioSource->set_volume(self->dyn__songPreviewPlayer()->dyn__volume() * self->dyn__songPreviewPlayer()->dyn__ambientVolumeScale());
                self->dyn__songPreviewPlayer()->StartCoroutine(self->dyn__songPreviewPlayer()->CrossFadeAfterDelayCoroutine(length - 1.2f));
                levelFailedLoader.audioSource->Play();
            }
        }
    }
    else {
        if (levelClearedLoader.loaded && addedToHierarchy && QSoundsConfig::Config.levelCleared_Active)
        {
            UnityEngine::AudioClip* audioClip = levelClearedLoader.getClip();
            self->dyn__levelClearedAudioClip() = audioClip;
        }
        else {
            self->dyn__levelClearedAudioClip() = levelClearedLoader.get_OriginalClip();
        }
    }
    ResultsViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
}

MAKE_HOOK_MATCH(ResultsViewController_RestartButtonPressed, &ResultsViewController::RestartButtonPressed, void, ResultsViewController* self) {
    if (levelFailedLoader.loaded && levelFailedLoader.audioSource->get_isPlaying()) {
        levelFailedLoader.audioSource->Stop();
    }
    ResultsViewController_RestartButtonPressed(self);
}

MAKE_HOOK_MATCH(SongPreviewPlayer_OnEnable, &SongPreviewPlayer::OnEnable, void, SongPreviewPlayer* self) {
    getLogger().info("is it true: %i", menuMusicLoader.loaded);

    if (!menuMusicLoader.OriginalAudioSource) {
        menuMusicLoader.set_OriginalClip(self->dyn__defaultAudioClip());
    }

    if (menuMusicLoader.loaded && QSoundsConfig::Config.menuMusic_Active)
    {
        UnityEngine::AudioClip* audioClip = menuMusicLoader.getClip();
        if (audioClip != nullptr)
            self->dyn__defaultAudioClip() = audioClip;
    }
    else {
        self->dyn__defaultAudioClip() = menuMusicLoader.get_OriginalClip();
    }
    SongPreviewPlayer_OnEnable(self);
}

#ifndef BS__1_13_2
MAKE_HOOK_MATCH(GameServerLobbyFlowCoordinator_DidActivate, &GameServerLobbyFlowCoordinator::DidActivate, void, GameServerLobbyFlowCoordinator* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    getLogger().debug("GameServerLobbyFlowCoordinator_DidActivate");
    if (!lobbyAmbienceLoader.OriginalAudioSource) lobbyAmbienceLoader.set_OriginalClip(self->dyn__ambienceAudioClip());

    if (lobbyAmbienceLoader.loaded && QSoundsConfig::Config.lobbyAmbience_Active)
    {
        getLogger().debug("Overwriting LobbyAmbience Audio");
        self->dyn__ambienceAudioClip() = lobbyAmbienceLoader.getClip();
    }
    else {
        self->dyn__ambienceAudioClip() = lobbyAmbienceLoader.get_OriginalClip();
    }
    GameServerLobbyFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
}

MAKE_HOOK_MATCH(GameServerLobbyFlowCoordinator_DidDeactivate, &GameServerLobbyFlowCoordinator::DidDeactivate, void, GameServerLobbyFlowCoordinator* self, bool removedFromHierarchy, bool screenSystemDisabling)
{
    getLogger().debug("GameServerLobbyFlowCoordinator_DidActivate");
    if (menuMusicLoader.loaded && QSoundsConfig::Config.menuMusic_Active && removedFromHierarchy)
    {
        getLogger().debug("Switching LobbyMusic to MenuMusic Audio");
        self->dyn__ambienceAudioClip() = menuMusicLoader.getClip();
    }
    else {
        self->dyn__ambienceAudioClip() = menuMusicLoader.get_OriginalClip();
    }
    GameServerLobbyFlowCoordinator_DidDeactivate(self, removedFromHierarchy, screenSystemDisabling);
}

MAKE_HOOK_MATCH(MultiplayerModeSelectionFlowCoordinator_DidActivate, &MultiplayerModeSelectionFlowCoordinator::DidActivate, void, MultiplayerModeSelectionFlowCoordinator* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    getLogger().debug("MultiplayerModeSelectionFlowCoordinator_DidActivate");
    if (menuMusicLoader.loaded && QSoundsConfig::Config.menuMusic_Active)
    {
        getLogger().debug("Switching LobbyMusic to MenuMusic Audio");
        self->dyn__ambienceAudioClip() = menuMusicLoader.getClip();
    }
    else {
        self->dyn__ambienceAudioClip() = menuMusicLoader.get_OriginalClip();
    }
    MultiplayerModeSelectionFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling); // This has to be ran last, otherwise it will not work correctly
}

MAKE_HOOK_MATCH(MultiplayerModeSelectionFlowCoordinator_DidDeactivate, &MultiplayerModeSelectionFlowCoordinator::DidDeactivate, void, MultiplayerModeSelectionFlowCoordinator* self, bool removedFromHierarchy, bool screenSystemDisabling)
{
    getLogger().debug("MultiplayerModeSelectionFlowCoordinator_DidDeactivate");
    if (menuMusicLoader.loaded && QSoundsConfig::Config.menuMusic_Active && removedFromHierarchy)
    {
        self->dyn__ambienceAudioClip() = menuMusicLoader.getClip();
    }
    else {
        self->dyn__ambienceAudioClip() = menuMusicLoader.get_OriginalClip();
    }

    MultiplayerModeSelectionFlowCoordinator_DidDeactivate(self, removedFromHierarchy, screenSystemDisabling);
}
#endif

MAKE_HOOK_MATCH(FileHelpers_GetEscapedURLForFilePath, &FileHelpers::GetEscapedURLForFilePath, StringW, StringW filePath) {
    return il2cpp_utils::newcsstr(std::u16string(u"file://") + std::u16string(csstrtostr(filePath)));
}

MAKE_HOOK_MATCH(NoteCutSoundEffectManager_Start, &NoteCutSoundEffectManager::Start, void, NoteCutSoundEffectManager* self) {
    if(hitSoundLoader.loaded && QSoundsConfig::Config.hitSound_Active)
    {
        hitSoundArr = createAudioClipArray(hitSoundLoader);
        self->dyn__longCutEffectsAudioClips() = hitSoundArr;
        self->dyn__shortCutEffectsAudioClips() = hitSoundArr;
        getLogger().debug("NoteCutSoundEffectManager_Start: Loaded hitSoundArray");
    }
    else {
        getLogger().debug("NoteCutSoundEffectManager_Start: Loading normally");
    }
    getLogger().debug("audioSamplesBeatAlignOffset was: %f", self->dyn__audioSamplesBeatAlignOffset());
    self->dyn__audioSamplesBeatAlignOffset() = QSoundsConfig::Config.hitSound_beatOffSet;
    getLogger().debug("audioSamplesBeatAlignOffset changed to: %f", self->dyn__audioSamplesBeatAlignOffset());
    NoteCutSoundEffectManager_Start(self);
    getLogger().debug("Beatalign offset is: %f", self->dyn__beatAlignOffset());
}

MAKE_HOOK_MATCH(NoteCutSoundEffect_Awake, &NoteCutSoundEffect::Awake, void, NoteCutSoundEffect* self) {
    if (hitSoundLoader.loaded && QSoundsConfig::Config.hitSound_Active) {
        self->dyn__goodCutVolume() += QSoundsConfig::Config.hitSound_audioVolumeOffset;
    }

    if(badHitSoundLoader.loaded && QSoundsConfig::Config.badHitSound_Active)
    {
        badHitSoundArr = createAudioClipArray(badHitSoundLoader);
        self->dyn__badCutSoundEffectAudioClips() = badHitSoundArr;
        self->dyn__badCutVolume() += QSoundsConfig::Config.badHitSound_audioVolumeOffset;
    }
    NoteCutSoundEffect_Awake(self);
}

MAKE_HOOK_MATCH(BeatmapObjectManager_HandleNoteWasMissed, &BeatmapObjectManager::HandleNoteControllerNoteWasMissed, void, BeatmapObjectManager* self, NoteController* noteController) {
    BeatmapObjectManager_HandleNoteWasMissed(self, noteController);
    if (noteMissedSoundLoader.loaded && 
        QSoundsConfig::Config.noteMissedSound_Active &&
        noteController->get_noteData()->get_scoringType() != NoteData::ScoringType::Ignore &&
        noteController->get_noteData()->get_gameplayType() != NoteData::GameplayType::Bomb) {
        //getLogger().debug("Playing note missed sound");
        noteMissedSoundLoader.audioSource->set_volume(0.5f + QSoundsConfig::Config.noteMissedSound_audioVolumeOffset);
        noteMissedSoundLoader.audioSource->Play();
        //noteMissedSoundLoader.audioSource->PlayOneShot(noteMissedSoundLoader.audioSource->get_clip(), 1.0f + QSoundsConfig::Config.noteMissedSound_audioVolumeOffset);
    }
}

MAKE_HOOK_MATCH(BasicUIAudioManager_Start, &BasicUIAudioManager::Start, void, BasicUIAudioManager* self) {
    if (!menuClickLoader.OriginalAudioSource) menuClickLoader.set_OriginalClip(self->dyn__clickSounds()[0]);

    if(menuClickLoader.loaded && QSoundsConfig::Config.menuClick_Active)
    {
        menuClickArr = createAudioClipArray(menuClickLoader);
        self->dyn__clickSounds() = menuClickArr;
    }
    BasicUIAudioManager_Start(self);
}

//MAKE_HOOK_MATCH(FireworkItemController_Awake, &FireworkItemController::Awake, void, FireworkItemController* self) {
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
MAKE_HOOK_MATCH(FireworkItemController_PlayExplosionSound, &FireworkItemController::PlayExplosionSound, void, FireworkItemController* self) {
    if (fireworkSoundLoader.loaded && QSoundsConfig::Config.firework_Active) {
        self->dyn__audioSource()->set_clip(fireworkSoundLoader.getClip());
        float pitch = 1.2f + (((float)rand()) / (float)RAND_MAX) * (0.8f - 1.2f);
        self->dyn__audioSource()->set_pitch(pitch);
        self->dyn__audioSource()->Play();
    }
    else FireworkItemController_PlayExplosionSound(self);
}

MAKE_HOOK_MATCH(PauseMenuManager_MenuButtonPressed, &PauseMenuManager::MenuButtonPressed, void, PauseMenuManager* self) {
    if (noteMissedSoundLoader.loaded &&
        QSoundsConfig::Config.noteMissedSound_Active) {
        if (noteMissedSoundLoader.audioSource) noteMissedSoundLoader.audioSource->Stop();
        //noteMissedSoundLoader.audioSource->PlayOneShot(noteMissedSoundLoader.audioSource->get_clip(), 1.0f + QSoundsConfig::Config.noteMissedSound_audioVolumeOffset);
    }
    PauseMenuManager_MenuButtonPressed(self);
}

MAKE_HOOK_MATCH(SceneManager_Internal_ActiveSceneChanged, &SceneManager::Internal_ActiveSceneChanged, void, Scene previousActiveScene, Scene newActiveScene) {
    SceneManager_Internal_ActiveSceneChanged(previousActiveScene, newActiveScene);
    if (newActiveScene.IsValid()) {
        std::string sceneName = to_utf8(csstrtostr(newActiveScene.get_name()));
        getLogger().info("Scene found: %s", sceneName.data());

        std::string questInit = "QuestInit";
        if(sceneName == questInit) QuestSounds::AudioClips::loadAudioClips();
    }
}

extern "C" void setup(ModInfo &info)
{
    info.id = MOD_ID;
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
    //QuestUI::Register::RegisterModSettingsFlowCoordinator<QuestSounds::QSoundsFlowCoordinator*>(modInfo);
    QuestUI::Register::RegisterMainMenuModSettingsFlowCoordinator<QuestSounds::QSoundsFlowCoordinator*>(modInfo);
    
    if(!LoadConfig()) SaveConfig();
    makeFolder();
    getLogger().debug("Installing QuestSounds!");
    INSTALL_HOOK(hkLog, SceneManager_Internal_ActiveSceneChanged);
    INSTALL_HOOK(hkLog, PauseMenuManager_MenuButtonPressed);
    INSTALL_HOOK(hkLog, SongPreviewPlayer_OnEnable);
    INSTALL_HOOK(hkLog, NoteCutSoundEffectManager_Start);
    INSTALL_HOOK(hkLog, NoteCutSoundEffect_Awake);
    INSTALL_HOOK(hkLog, BeatmapObjectManager_HandleNoteWasMissed);
    INSTALL_HOOK(hkLog, BasicUIAudioManager_Start);
    INSTALL_HOOK(hkLog, ResultsViewController_DidActivate);
    INSTALL_HOOK(hkLog, ResultsViewController_RestartButtonPressed);
    INSTALL_HOOK(hkLog, FireworkItemController_PlayExplosionSound)
    INSTALL_HOOK(hkLog, MultiplayerModeSelectionFlowCoordinator_DidActivate);     // Added for switching out MP Lobby Music
    INSTALL_HOOK(hkLog, MultiplayerModeSelectionFlowCoordinator_DidDeactivate);  // Added for switching out MP Lobby Music
    INSTALL_HOOK(hkLog, GameServerLobbyFlowCoordinator_DidActivate);              // Added for switching out MP Lobby Music
    INSTALL_HOOK(hkLog, GameServerLobbyFlowCoordinator_DidDeactivate);          // Added for switching out MP Lobby Music
    auto ModList = Modloader::getMods();
    if (ModList.find("SongLoader") == ModList.end()) {
        getLogger().info("SongLoader missing, installing FilePath check");
        INSTALL_HOOK(hkLog, FileHelpers_GetEscapedURLForFilePath);
    }
    getLogger().debug("Installed QuestSounds!");
}
