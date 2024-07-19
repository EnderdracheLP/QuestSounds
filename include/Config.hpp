#include "scotland2/shared/loader.hpp"
#include "rapidjson-macros/shared/macros.hpp"

#define SOUND_PATH_FORMAT "/sdcard/ModData/{}/Mods/QuestSounds/"
// #define CONFIG_VERSION "SoundsConfig_v3"
// #define CONFIG_VERSION_PRE_1_20 "SoundsConfig_v2"
// #define CONFIG_VERSION_PRE_R "SoundsConfig_v1"
// #define CONFIG_VERSION_LEGACY "Sounds"

#define SOUND_VALUE(name, folder) VALUE_DEFAULT(Sound, name, Sound(soundPath + folder + "/", soundPath + folder + "/" + #name + ".ogg"))

// define an automatically serialized / deserialized instance variable with a custom name in the json file and a default value
#pragma region NAMED_VALUE_CUSTOM(type, name, default, jsonName)
#define NAMED_VALUE_CUSTOM(type, name, def, jsonName) \
type name = def; \
class _JSONValueAdder_##name { \
    _JSONValueAdder_##name() { \
        serializers().emplace_back([](const SelfType* self, rapidjson::Value& jsonObject, rapidjson::Document::AllocatorType& allocator) { \
            rapidjson_macros_auto::Serialize(self->name, jsonName, jsonObject, allocator); \
        }); \
        deserializers().emplace_back([](SelfType* self, rapidjson::Value& jsonValue) { \
            rapidjson_macros_auto::Deserialize(self->name, jsonName, self->def, jsonValue); \
        }); \
    } \
    friend class rapidjson_macros_types::ConstructorRunner<_JSONValueAdder_##name>; \
    static inline rapidjson_macros_types::ConstructorRunner<_JSONValueAdder_##name> _##name##_JSONValueAdderInstance; \
};
#pragma endregion

namespace QuestSounds {
    static const std::string soundPath = fmt::format(SOUND_PATH_FORMAT, modloader::get_application_id().c_str());

    DECLARE_JSON_CLASS(Sound,
        NAMED_VALUE_DEFAULT(bool, Active, true, "activated");
        std::string FolderPath;
        std::string DefaultFilePath;
        NAMED_VALUE_CUSTOM(std::string, FilePath, DefaultFilePath, "filepath");
        NAMED_VALUE_OPTIONAL(float, VolumeOffset, "audioVolumeOffset");
        NAMED_VALUE_OPTIONAL(float, BeatOffset, "beatOffSet");
        Sound(std::string folderPath, std::string defaultFilePath) : FolderPath(folderPath), DefaultFilePath(defaultFilePath) {}
    )

    DECLARE_JSON_CLASS(
        Sounds, 
        SOUND_VALUE(HitSound, "HitSounds");
        SOUND_VALUE(BadHitSound, "BadHitSounds");
        SOUND_VALUE(NoteMissedSound, "NoteMissedSounds");
        SOUND_VALUE(MenuMusic, "MenuMusic");
        SOUND_VALUE(MenuClick, "MenuClicks");
        SOUND_VALUE(Firework, "Fireworks");
        SOUND_VALUE(LevelCleared, "LevelCleared");
        SOUND_VALUE(LevelFailed, "LevelFailed");
        SOUND_VALUE(LobbyMusic, "LobbyMusic");
    )

    DECLARE_JSON_CLASS(
        SoundsConfig,
        NAMED_VALUE(Sounds, Sounds, NAME_OPTS("SoundsConfig_v3", "SoundsConfig_v2", "SoundsConfig_v1", "Sounds"));
    )
    extern SoundsConfig Config;
}



// #define CONFIG_SOUND(name, folderpath, active, ...) {name, SoundConfig(name, folderpath, active, __VA_ARGS__)}

// namespace QuestSounds {
//     struct SoundConfig {
//         SoundConfig() = default;
//         SoundConfig(std::string name, std::string folderpath, bool active, std::optional<float> volumeOffset = std::nullopt, std::optional<float> beatOffset = std::nullopt) : FolderPath(folderpath), FilePath(folderpath + name + ".ogg"), Active(active), VolumeOffset(volumeOffset), BeatOffset(beatOffset) {}

//         bool Active = true;
//         std::string FilePath;
//         std::string FolderPath;
//         std::optional<float> VolumeOffset;
//         std::optional<float> BeatOffset;
//     };

//     static const std::string soundPath = fmt::format(SOUND_PATH_FORMAT, modloader::get_application_id().c_str());

//     struct Config {
//         std::map<std::string, SoundConfig> Sounds = {
//             CONFIG_SOUND("MenuMusic", soundPath + "MenuMusic/", true),
//             CONFIG_SOUND("HitSound", soundPath + "HitSounds/", true, 0, 0.185f),
//             CONFIG_SOUND("BadHitSound", soundPath + "BadHitSounds/", true),
//             CONFIG_SOUND("NoteMissedSound", soundPath + "NoteMissedSounds/", true),
//             CONFIG_SOUND("MenuClickSound", soundPath + "MenuClicks/", true),
//             CONFIG_SOUND("FireworkSound", soundPath + "Fireworks/", true),
//             CONFIG_SOUND("LevelClearedSound", soundPath + "LevelCleared/", true),
//             CONFIG_SOUND("LevelFailedSound", soundPath + "LevelFailed/", true),
//             CONFIG_SOUND("LobbyAmbience", soundPath + "LobbyMusic/", true)
//         };
//         // SoundConfig menuMusic = SoundConfig("MenuMusic", soundPath + "MenuMusic/", true, 0, 0);
//         // SoundConfig hitSound = SoundConfig("HitSound", soundPath + "HitSounds/", true, 0, 0);
//         // SoundConfig badHitSound = SoundConfig("BadHitSound", soundPath + "BadHitSounds/", true, 0, 0);
//         // SoundConfig noteMissedSound = SoundConfig("NoteMissedSound", soundPath + "NoteMissedSounds/", true, 0, 0);
//         // SoundConfig menuClickSound = SoundConfig("MenuClickSound", soundPath + "MenuClicks/", true, 0, 0);
//         // SoundConfig fireworkSound = SoundConfig("FireworkSound", soundPath + "Fireworks/", true, 0, 0);
//         // SoundConfig levelClearedSound = SoundConfig("LevelClearedSound", soundPath + "LevelCleared/", true, 0, 0);
//         // SoundConfig levelFailedSound = SoundConfig("LevelFailedSound", soundPath + "LevelFailed/", true, 0, 0);
//         // SoundConfig lobbyAmbience = SoundConfig("LobbyAmbience", soundPath + "LobbyMusic/", true, 0, 0);
//         bool LegacyConfig = false;
//     };

//     extern Config config;
// }




#undef SOUND_PATH_FORMAT
#undef SOUND_VALUE
// #undef CONFIG_SOUND