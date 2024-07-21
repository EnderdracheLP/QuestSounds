#include "main.hpp"
#include "logging.hpp"
#include "Utils/AsyncAudioClipLoader.hpp"
#include "Config.hpp"
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
using namespace GlobalNamespace;

#include "System/Action.hpp"

#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/SceneManagement/SceneManager.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/WaitForSeconds.hpp"
#include "UnityEngine/Time.hpp"
using namespace UnityEngine::SceneManagement;

// TODO BSML

// #include "ViewControllers/MenuSdListViewController.hpp"
// #include "ViewControllers/HitSdListViewController.hpp"
// #include "ViewControllers/MenuClickSdListViewController.hpp"
// #include "ViewControllers/BadHitSdListViewController.hpp"
// #include "ViewControllers/FireworkSdListViewController.hpp"
// #include "ViewControllers/LevelClearedSdListViewController.hpp"
// #include "ViewControllers/LobbyMusicSdListViewController.hpp"
// #include "ViewControllers/ConfigViewController.hpp"
// #include "QSoundsFlowCoordinator.hpp"
using namespace QuestSounds;

// #include "custom-types/shared/register.hpp"
// using namespace custom_types;

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

MAKE_HOOK_MATCH(ResultsViewController_DidActivate, &ResultsViewController::DidActivate, void, ResultsViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (firstActivation && addedToHierarchy && !levelClearedLoader.OriginalAudioSource) {
        levelClearedLoader.set_OriginalClip(self->_levelClearedAudioClip);
    }
    if (self->_levelCompletionResults->levelEndStateType == LevelCompletionResults::LevelEndStateType::Failed) {
        self->_songPreviewPlayer->StopAllCoroutines();
        if (levelFailedLoader.loaded && addedToHierarchy && Config.Sounds.LevelFailed.Active) {
            UnityEngine::AudioClip* FailedSound = levelFailedLoader.getClip();
            float length = FailedSound->get_length();
            getLogger().debug("Duration of LevelFailed Sound: {}", length);
            if (length > 7.0f) {
                getLogger().info("Long LevelFailedSound");
                self->_songPreviewPlayer->CrossfadeTo(FailedSound, -4.0f, 0.0f, length, nullptr);
            }
            else {
                getLogger().info("Short LevelFailedSound");
                self->_songPreviewPlayer->FadeOut(0.1f);
                self->_songPreviewPlayer->_fadeSpeed = self->_songPreviewPlayer->_fadeInSpeed;
                getLogger().debug("volume: {}", self->_songPreviewPlayer->_volume);
                getLogger().debug("AmbientVolume: {}", self->_songPreviewPlayer->_ambientVolumeScale);
                getLogger().debug("Set Volume: {}", self->_songPreviewPlayer->_volume * self->_songPreviewPlayer->_ambientVolumeScale);

                levelFailedLoader.audioSource->set_volume(self->_songPreviewPlayer->_volume * self->_songPreviewPlayer->_ambientVolumeScale);
                self->_songPreviewPlayer->StartCoroutine(self->_songPreviewPlayer->CrossFadeAfterDelayCoroutine(length - 1.2f));
                levelFailedLoader.audioSource->Play();
            }
        }
    }
    else {
        if (levelClearedLoader.loaded && addedToHierarchy && Config.Sounds.LevelCleared.Active)
        {
            UnityEngine::AudioClip* audioClip = levelClearedLoader.getClip();
            self->_levelClearedAudioClip = audioClip;
        }
        else {
            self->_levelClearedAudioClip = levelClearedLoader.get_OriginalClip();
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
        menuMusicLoader.set_OriginalClip(self->_defaultAudioClip);
    }

    if (menuMusicLoader.loaded && Config.Sounds.MenuMusic.Active)
    {
        getLogger().debug("Overriding MenuMusic Audio");
        UnityEngine::AudioClip* audioClip = menuMusicLoader.getClip();
        if (audioClip != nullptr)
            self->_defaultAudioClip = audioClip;
    }
    else {
        getLogger().debug("Loading MenuMusic Audio normally: isLoaded='{}' isActive='{}'", menuMusicLoader.loaded, Config.Sounds.MenuMusic.Active);
        self->_defaultAudioClip = menuMusicLoader.get_OriginalClip();
    }
    SongPreviewPlayer_OnEnable(self);
}

MAKE_HOOK_MATCH(GameServerLobbyFlowCoordinator_DidActivate, &GameServerLobbyFlowCoordinator::DidActivate, void, GameServerLobbyFlowCoordinator* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    getLogger().debug("GameServerLobbyFlowCoordinator_DidActivate");
    if (!lobbyAmbienceLoader.OriginalAudioSource) lobbyAmbienceLoader.set_OriginalClip(self->_ambienceAudioClip);

    if (lobbyAmbienceLoader.loaded && Config.Sounds.LobbyMusic.Active)
    {
        getLogger().debug("Overwriting LobbyAmbience Audio");
        self->_ambienceAudioClip = lobbyAmbienceLoader.getClip();
    }
    else {
        self->_ambienceAudioClip = lobbyAmbienceLoader.get_OriginalClip();
    }
    GameServerLobbyFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
}

MAKE_HOOK_MATCH(GameServerLobbyFlowCoordinator_DidDeactivate, &GameServerLobbyFlowCoordinator::DidDeactivate, void, GameServerLobbyFlowCoordinator* self, bool removedFromHierarchy, bool screenSystemDisabling)
{
    getLogger().debug("GameServerLobbyFlowCoordinator_DidActivate");
    if (menuMusicLoader.loaded && Config.Sounds.MenuMusic.Active && removedFromHierarchy)
    {
        getLogger().debug("Switching LobbyMusic to MenuMusic Audio");
        self->_ambienceAudioClip = menuMusicLoader.getClip();
    }
    else {
        self->_ambienceAudioClip = menuMusicLoader.get_OriginalClip();
    }
    GameServerLobbyFlowCoordinator_DidDeactivate(self, removedFromHierarchy, screenSystemDisabling);
}

MAKE_HOOK_MATCH(MultiplayerModeSelectionFlowCoordinator_DidActivate, &MultiplayerModeSelectionFlowCoordinator::DidActivate, void, MultiplayerModeSelectionFlowCoordinator* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    getLogger().debug("MultiplayerModeSelectionFlowCoordinator_DidActivate");
    if (menuMusicLoader.loaded && Config.Sounds.MenuMusic.Active)
    {
        getLogger().debug("Switching LobbyMusic to MenuMusic Audio");
        self->_ambienceAudioClip = menuMusicLoader.getClip();
    }
    else {
        self->_ambienceAudioClip = menuMusicLoader.get_OriginalClip();
    }
    MultiplayerModeSelectionFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling); // This has to be ran last, otherwise it will not work correctly
}

MAKE_HOOK_MATCH(MultiplayerModeSelectionFlowCoordinator_DidDeactivate, &MultiplayerModeSelectionFlowCoordinator::DidDeactivate, void, MultiplayerModeSelectionFlowCoordinator* self, bool removedFromHierarchy, bool screenSystemDisabling)
{
    getLogger().debug("MultiplayerModeSelectionFlowCoordinator_DidDeactivate");
    if (menuMusicLoader.loaded && Config.Sounds.MenuMusic.Active && removedFromHierarchy)
    {
        self->_ambienceAudioClip = menuMusicLoader.getClip();
    }
    else {
        self->_ambienceAudioClip = menuMusicLoader.get_OriginalClip();
    }

    MultiplayerModeSelectionFlowCoordinator_DidDeactivate(self, removedFromHierarchy, screenSystemDisabling);
}

#ifdef DEBUG
MAKE_HOOK_MATCH(FileHelpers_GetEscapedURLForFilePath, &FileHelpers::GetEscapedURLForFilePath, StringW, StringW filePath) {
    return StringW("file://" + filePath);
}
#endif

MAKE_HOOK_MATCH(NoteCutSoundEffectManager_Start, &NoteCutSoundEffectManager::Start, void, NoteCutSoundEffectManager* self) {
    if(hitSoundLoader.loaded && Config.Sounds.HitSound.Active)
    {
        hitSoundArr = createAudioClipArray(hitSoundLoader);
        self->_longCutEffectsAudioClips = hitSoundArr;
        self->_shortCutEffectsAudioClips = hitSoundArr;
        getLogger().debug("NoteCutSoundEffectManager_Start: Loaded hitSoundArray");
    }
    else {
        getLogger().debug("NoteCutSoundEffectManager_Start: Loading normally");
    }
    getLogger().debug("audioSamplesBeatAlignOffset was: {}", self->_audioSamplesBeatAlignOffset);
    if (Config.Sounds.HitSound.BeatOffset.has_value())
        self->_audioSamplesBeatAlignOffset = Config.Sounds.HitSound.BeatOffset.value();
    getLogger().debug("audioSamplesBeatAlignOffset changed to: {}", self->_audioSamplesBeatAlignOffset);
    NoteCutSoundEffectManager_Start(self);
    getLogger().debug("Beatalign offset is: {}", self->_beatAlignOffset);
}

MAKE_HOOK_MATCH(NoteCutSoundEffect_Awake, &NoteCutSoundEffect::Awake, void, NoteCutSoundEffect* self) {
    if (hitSoundLoader.loaded && Config.Sounds.HitSound.Active) {
        self->_goodCutVolume += Config.Sounds.HitSound.VolumeOffset.value_or(0.0f);
    }

    if(badHitSoundLoader.loaded && Config.Sounds.BadHitSound.Active)
    {
        badHitSoundArr = createAudioClipArray(badHitSoundLoader);
        self->_badCutSoundEffectAudioClips = badHitSoundArr;
        self->_badCutVolume += Config.Sounds.BadHitSound.VolumeOffset.value_or(0.0f);
    }
    NoteCutSoundEffect_Awake(self);
}

MAKE_HOOK_MATCH(BeatmapObjectManager_HandleNoteWasMissed, &BeatmapObjectManager::HandleNoteControllerNoteWasMissed, void, BeatmapObjectManager* self, NoteController* noteController) {
    BeatmapObjectManager_HandleNoteWasMissed(self, noteController);
    if (noteMissedSoundLoader.loaded && 
        Config.Sounds.NoteMissedSound.Active &&
        noteController->get_noteData()->get_scoringType() != NoteData::ScoringType::Ignore &&
        noteController->get_noteData()->get_gameplayType() != NoteData::GameplayType::Bomb) {
        noteMissedSoundLoader.audioSource->set_volume(0.5f + Config.Sounds.NoteMissedSound.VolumeOffset.value_or(0.0f));
        noteMissedSoundLoader.audioSource->Play();
    }
}

MAKE_HOOK_MATCH(BasicUIAudioManager_Start, &BasicUIAudioManager::Start, void, BasicUIAudioManager* self) {
    if (!menuClickLoader.OriginalAudioSource) menuClickLoader.set_OriginalClip(self->_clickSounds[0]);

    if(menuClickLoader.loaded && Config.Sounds.MenuClick.Active)
    {
        menuClickArr = createAudioClipArray(menuClickLoader);
        self->_clickSounds = menuClickArr;
    }
    BasicUIAudioManager_Start(self);
}

// Replacing the function here, as replacing the AudioClips proves to be difficult
MAKE_HOOK_MATCH(FireworkItemController_PlayExplosionSound, &FireworkItemController::PlayExplosionSound, void, FireworkItemController* self) {
    if (fireworkSoundLoader.loaded && Config.Sounds.Firework.Active) {
        self->_audioSource->set_clip(fireworkSoundLoader.getClip());
        float pitch = 1.2f + (((float)rand()) / (float)RAND_MAX) * (0.8f - 1.2f);
        self->_audioSource->set_pitch(pitch);
        self->_audioSource->Play();
    }
    else FireworkItemController_PlayExplosionSound(self);
}

MAKE_HOOK_MATCH(PauseMenuManager_MenuButtonPressed, &PauseMenuManager::MenuButtonPressed, void, PauseMenuManager* self) {
    if (noteMissedSoundLoader.loaded &&
        Config.Sounds.NoteMissedSound.Active) {
        if (noteMissedSoundLoader.audioSource) noteMissedSoundLoader.audioSource->Stop();
    }
    PauseMenuManager_MenuButtonPressed(self);
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

QS_EXPORT void setup(CModInfo *info) {
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

QS_EXPORT void late_load()
{
    il2cpp_functions::Init();

    auto& hkLog = getLogger();

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
    // #ifdef DEBUG
    // auto ModList = Modloader::getMods();
    // if (ModList.find("SongLoader") == ModList.end()) {
    //     getLogger().info("SongLoader missing, installing FilePath check");
    //     INSTALL_HOOK(hkLog, FileHelpers_GetEscapedURLForFilePath);
    // }
    // #endif
    getLogger().debug("Installed QuestSounds!");
}
