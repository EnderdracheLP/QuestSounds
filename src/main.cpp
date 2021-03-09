#include "main.hpp"
#include "audiocliploader.hpp"
#include <dlfcn.h>
//using namespace audioClipLoader;

#include "GlobalNamespace/ResultsViewController.hpp"
#include "GlobalNamespace/SongPreviewPlayer.hpp"
#include "GlobalNamespace/NoteCutSoundEffectManager.hpp"
#include "GlobalNamespace/NoteCutSoundEffect.hpp"
#include "GlobalNamespace/BasicUIAudioManager.hpp"
#include "GlobalNamespace/FireworkItemController.hpp"
using namespace GlobalNamespace;


#include "UnityEngine/SceneManagement/Scene.hpp"
//using namespace UnityEngine::SceneManagement;

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
/*
#define HITSOUNDS_PATH_FORMAT "/sdcard/ModData/%s/Mods/QuestSounds/HitSounds/"
#define BADHITSOUNDS_PATH_FORMAT "/sdcard/ModData/%s/Mods/QuestSounds/BadHitSounds/"
#define MENUMUSIC_PATH_FORMAT "/sdcard/ModData/%s/Mods/QuestSounds/MenuMusicTracks/"
*/


ModInfo modInfo;

Configuration& getConfig() {
    static Configuration config(modInfo);
    return config;
}

Logger& getLogger() {
    static auto logger = new Logger(modInfo, LoggerOptions(false, true));
    return *logger;
}

std::string soundPath = string_format(SOUND_PATH_FORMAT, Modloader::getApplicationId().c_str());
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
    float hitSound_Volume = 1;
    float badHitSound_Volume = 1;
    float menuClick_Volume = 1;
    float menuMusic_Volume = 1;
    float firework_Volume = 1;
    float levelCleared_Volume = 1;
} Config;

void AddChildSound(ConfigValue& parent, std::string_view soundName, bool active, std::string filePath, float Volume, ConfigDocument::AllocatorType& allocator)
{
    ConfigValue value(rapidjson::kObjectType);
    value.AddMember("activated", active, allocator);
    std::string data(filePath);
    value.AddMember("filepath", data, allocator);
    value.AddMember("Volume", Volume, allocator);
    parent.AddMember((ConfigValue::StringRefType)soundName.data(), value, allocator);
}

// Edited ParseVector made by Darknight1050
bool ParseSound(bool& active, std::string& filepath, float& Volume, ConfigValue& parent, std::string_view soundName)
{
    if(!parent.HasMember(soundName.data()) || !parent[soundName.data()].IsObject()) return false;
    ConfigValue value = parent[soundName.data()].GetObject();
    if(!(value.HasMember("activated") && value["activated"].IsBool() &&
         value.HasMember("filepath") && value["filepath"].IsString() &&
         value.HasMember("Volume") && value["Volume"].IsFloat())) return false;
    active = value["activated"].GetBool();
    filepath = value["filepath"].GetString();
    Volume = value["Volume"].GetFloat();
    return true;
}

void SaveConfig()
{
    getConfig().config.RemoveAllMembers();
    getConfig().config.SetObject();
    auto& allocator = getConfig().config.GetAllocator();

    ConfigValue soundsValue(rapidjson::kObjectType);
    AddChildSound(soundsValue, "HitSound", Config.hitSound_Active, Config.hitSound_filepath, Config.hitSound_Volume, allocator);
    AddChildSound(soundsValue, "BadHitSound", Config.badHitSound_Active, Config.badHitSound_filepath, Config.badHitSound_Volume, allocator);  
    AddChildSound(soundsValue, "MenuMusic", Config.menuMusic_Active, Config.menuMusic_filepath, Config.menuMusic_Volume, allocator);
    AddChildSound(soundsValue, "MenuClick", Config.menuClick_Active, Config.menuClick_filepath, Config.menuClick_Volume, allocator);
    AddChildSound(soundsValue, "Firework", Config.firework_Active, Config.firework_filepath, Config.firework_Volume, allocator);
    AddChildSound(soundsValue, "LevelCleared", Config.levelCleared_Active, Config.levelCleared_filepath, Config.levelCleared_Volume, allocator);
    getConfig().config.AddMember("Sounds 1.0.0", soundsValue, allocator); 
    getConfig().Write();
}

bool LoadConfig()
{
    getConfig().Load();

    if(getConfig().config.HasMember("Sounds 1.0.0") && getConfig().config["Sounds 1.0.0"].IsObject())
    {
        ConfigValue soundsValue = getConfig().config["Sounds 1.0.0"].GetObject();
        if(!ParseSound(Config.hitSound_Active, Config.hitSound_filepath, Config.hitSound_Volume, soundsValue, "HitSound")) return false;
        if(!ParseSound(Config.badHitSound_Active, Config.badHitSound_filepath, Config.badHitSound_Volume, soundsValue, "BadHitSound")) return false;
        if(!ParseSound(Config.menuMusic_Active, Config.menuMusic_filepath, Config.menuMusic_Volume, soundsValue, "MenuMusic")) return false;
        if(!ParseSound(Config.menuClick_Active, Config.menuClick_filepath, Config.menuClick_Volume, soundsValue, "MenuClick")) return false;
        if(!ParseSound(Config.firework_Active, Config.firework_filepath, Config.firework_Volume, soundsValue, "Firework")) return false;
        if(!ParseSound(Config.levelCleared_Active, Config.levelCleared_filepath, Config.levelCleared_Volume, soundsValue, "LevelCleared")) return false;
    } else return false;
    
    return true;
}

audioClipLoader::loader hitSoundLoader; // hitSound
audioClipLoader::loader badHitSoundLoader; // badHitSound
audioClipLoader::loader menuMusicLoader;    // menuMusic
audioClipLoader::loader menuClickLoader;
audioClipLoader::loader fireworkSoundLoader;
audioClipLoader::loader levelClearedLoader;
Array<UnityEngine::AudioClip*>* hitSoundArr; // hitSoundArray
Array<UnityEngine::AudioClip*>* badHitSoundArr; // badHitSoundArray
Array<UnityEngine::AudioClip*>* menuClickArr;
Array<UnityEngine::AudioClip*>* fireworkSoundArr;


/*
also lines 147 to 150 is not only bad practice
(because holding pointers to C# objects like that may not consider the possibilities if the object is GC'd)
but also bad because you set them to null
*/

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

    custom_types::Register::RegisterType<QuestSounds::QSoundsViewController>();
    QuestUI::Register::RegisterModSettingsViewController<QuestSounds::QSoundsViewController*>(modInfo);

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
    getLogger().debug("Installed QuestSounds!");
}
