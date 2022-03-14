//#include "audiocliploader.hpp"
//#include "AsyncAudiocliploader.hpp"
#include "QSoundsConfig.hpp"

namespace QSoundsConfig {

    //std::string soundPath = string_format(SOUND_PATH_FORMAT, Modloader::getApplicationId().c_str());

    Config_t Config;

    bool LegacyConfig = false;

    void AddChildSound(ConfigValue& parent, std::string_view soundName, bool active, std::string filePath, ConfigDocument::AllocatorType& allocator)
    {
        ConfigValue value(rapidjson::kObjectType);
        value.AddMember("activated", active, allocator);
        std::string data(filePath);
        value.AddMember("filepath", data, allocator);
        parent.AddMember((ConfigValue::StringRefType)soundName.data(), value, allocator);
    }


    void AddChildSound(ConfigValue& parent, std::string_view soundName, bool active, std::string filePath, float audioVolumeOffset, ConfigDocument::AllocatorType& allocator)
    {
        ConfigValue value(rapidjson::kObjectType);
        value.AddMember("activated", active, allocator);
        std::string data(filePath);
        value.AddMember("filepath", data, allocator);
        value.AddMember("audioVolumeOffset", audioVolumeOffset, allocator);
        parent.AddMember((ConfigValue::StringRefType)soundName.data(), value, allocator);
    }

    void AddChildSound(ConfigValue& parent, std::string_view soundName, bool active, std::string filePath, float beatOffSet, float audioVolumeOffset, ConfigDocument::AllocatorType& allocator)
    {
        ConfigValue value(rapidjson::kObjectType);
        value.AddMember("activated", active, allocator);
        std::string data(filePath);
        value.AddMember("filepath", data, allocator);
        value.AddMember("beatOffSet", beatOffSet, allocator);
        value.AddMember("audioVolumeOffset", audioVolumeOffset, allocator);
        parent.AddMember((ConfigValue::StringRefType)soundName.data(), value, allocator);
    }


    // Edited ParseVector made by Darknight1050
    namespace Legacy {
        bool TryParseSound(bool& active, std::string& filepath, float& beatOffSet, ConfigValue& parent, std::string_view soundName)
        {
            if (!parent.HasMember(soundName.data()) || !parent[soundName.data()].IsObject()) {
                getLogger().error("ERROR: Parsing, %s, parent HasMember bool: %d, Member IsObject bool: %d", soundName.data(), parent.HasMember(soundName.data()), parent[soundName.data()].IsObject());
                return false;
            }
            ConfigValue value = parent[soundName.data()].GetObject();
            bool success = true;
            if (value.HasMember("activated") && value["activated"].IsBool()) {
                active = value["activated"].GetBool();
            }
            else {
                getLogger().warning("WARNING: Member missing.\nWhile Parsing %s, does activated exist '%s' and is it a float '%s'",
                    soundName.data(),
                    value.HasMember("activated") ? "true" : "false", value.HasMember("activated") ? value["activated"].IsBool() ? "true" : "false" : "unknown");
                success = false;
            }
            if (value.HasMember("beatOffSet") && value["beatOffSet"].IsFloat()) {
                beatOffSet = value["beatOffSet"].GetFloat();
            }
            else {
                getLogger().warning("WARNING: Member missing.\nWhile Parsing %s, does beatOffSet exist '%s' and is it a float '%s'",
                    soundName.data(),
                    value.HasMember("beatOffSet") ? "true" : "false", value.HasMember("beatOffSet") ? value["beatOffSet"].IsFloat() ? "true" : "false" : "unknown");
                success = false;
            }
            if (!(value.HasMember("filepath") && value["filepath"].IsString())) {
                getLogger().error("ERROR: important members missing skipping sound.\nWhile Parsing: '%s', does filepath exist: '%s' is it a string: '%s'",
                    soundName.data(),
                    value.HasMember("filepath") ? "true" : "false", value.HasMember("filepath") ? value["filepath"].IsString() ? "true" : "false" : "unknown");
                return false;
            }
            filepath = value["filepath"].GetString();
            return success;
        }
    }

    bool TryParseSound(bool& active, std::string& filepath, float& beatOffSet, float& audioVolumeOffset, ConfigValue& parent, std::string_view soundName)
    {
        if (!parent.HasMember(soundName.data()) || !parent[soundName.data()].IsObject()) {
            getLogger().error("ERROR: Parsing, %s, parent HasMember bool: %d, Member IsObject bool: %d", soundName.data(), parent.HasMember(soundName.data()), parent[soundName.data()].IsObject());
            return false;
        }
        ConfigValue value = parent[soundName.data()].GetObject();
        bool success = true;
        if (value.HasMember("activated") && value["activated"].IsBool()) {
            active = value["activated"].GetBool();
        }
        else {
            getLogger().warning("WARNING: Member missing.\nWhile Parsing %s, does activated exist '%s' and is it a float '%s'",
                soundName.data(),
                value.HasMember("activated") ? "true" : "false", value.HasMember("activated") ? value["activated"].IsBool() ? "true" : "false" : "unknown");
            success = false;
        }
        if (value.HasMember("beatOffSet") && value["beatOffSet"].IsFloat()) {
            beatOffSet = value["beatOffSet"].GetFloat();
        }
        else {
            getLogger().warning("WARNING: Member missing.\nWhile Parsing %s, does beatOffSet exist '%s' and is it a float '%s'", 
                soundName.data(),
                value.HasMember("beatOffSet") ? "true" : "false", value.HasMember("beatOffSet") ? value["beatOffSet"].IsFloat() ? "true" : "false" : "unknown");
            success = false;
        }
        if (value.HasMember("audioVolumeOffset") && value["audioVolumeOffset"].IsFloat()) {
            audioVolumeOffset = value["audioVolumeOffset"].GetFloat();
        }
        else {
            getLogger().warning("WARNING: Member missing.\nWhile Parsing %s, does audioVolumeOffset exist '%s' and is it a float '%s'",
                soundName.data(),
                value.HasMember("audioVolumeOffset") ? "true" : "false", value.HasMember("audioVolumeOffset") ? value["audioVolumeOffset"].IsFloat() ? "true" : "false" : "unknown");
            success = false;
        }
        if (!(value.HasMember("filepath") && value["filepath"].IsString())) {
            getLogger().error("ERROR: important members missing skipping sound.\nWhile Parsing: '%s', does filepath exist: '%s' is it a string: '%s'",
                soundName.data(),
                value.HasMember("filepath") ? "true" : "false", value.HasMember("filepath") ? value["filepath"].IsString() ? "true" : "false" : "unknown");
            return false;
        }
        filepath = value["filepath"].GetString();
        return success;
    }

    bool TryParseSound(bool& active, std::string& filepath, float& audioVolumeOffset, ConfigValue& parent, std::string_view soundName)
    {
        if (!parent.HasMember(soundName.data()) || !parent[soundName.data()].IsObject()) {
            getLogger().error("ERROR: Parsing, %s, parent HasMember bool: %d, Member IsObject bool: %d", soundName.data(), parent.HasMember(soundName.data()), parent[soundName.data()].IsObject());
            return false;
        }
        ConfigValue value = parent[soundName.data()].GetObject();
        bool success = true;
        if (value.HasMember("activated") && value["activated"].IsBool()) {
            active = value["activated"].GetBool();
        }
        else {
            getLogger().warning("WARNING: Member missing.\nWhile Parsing %s, does activated exist '%s' and is it a bool '%s'",
                soundName.data(),
                value.HasMember("activated") ? "true" : "false", value.HasMember("activated") ? value["activated"].IsBool() ? "true" : "false" : "unknown");
            success = false;
        }
        if (value.HasMember("audioVolumeOffset") && value["audioVolumeOffset"].IsFloat()) {
            audioVolumeOffset = value["audioVolumeOffset"].GetFloat();
        }
        else {
            getLogger().warning("WARNING: Member missing.\nWhile Parsing %s, does audioVolumeOffset exist '%s' and is it a float '%s'",
                soundName.data(),
                value.HasMember("audioVolumeOffset") ? "true" : "false", value.HasMember("audioVolumeOffset") ? value["audioVolumeOffset"].IsFloat() ? "true" : "false" : "unknown");
            success = false;
        }
        if (!(value.HasMember("filepath") && value["filepath"].IsString())) {
            getLogger().error("ERROR: important members missing skipping sound.\nWhile Parsing: '%s', does filepath exist: '%s' is it a string: '%s'",
                soundName.data(),
                value.HasMember("filepath") ? "true" : "false", value.HasMember("filepath") ? value["filepath"].IsString() ? "true" : "false" : "unknown");
            return false;
        }
        filepath = value["filepath"].GetString();
        return success;
    }

    bool TryParseSound(bool& active, std::string& filepath, ConfigValue& parent, std::string_view soundName)
    {
        if (!parent.HasMember(soundName.data()) || !parent[soundName.data()].IsObject()) {
            getLogger().error("ERROR: Parsing, %s, parent HasMember bool: %d, Member IsObject bool: %d", soundName.data(), parent.HasMember(soundName.data()), parent[soundName.data()].IsObject());
            return false;
        }
        ConfigValue value = parent[soundName.data()].GetObject();
        bool success = true;
        if (value.HasMember("activated") && value["activated"].IsBool()) {
            active = value["activated"].GetBool();
        }
        else {
            getLogger().warning("WARNING: Member missing.\nWhile Parsing %s, does activated exist '%s' and is it a bool '%s'",
                soundName.data(),
                value.HasMember("activated") ? "true" : "false", value.HasMember("activated") ? value["activated"].IsBool() ? "true" : "false" : "unknown");
            success = false;
        }
        if (!(value.HasMember("filepath") && value["filepath"].IsString())) {
            getLogger().error("ERROR: important members missing skipping sound.\nWhile Parsing: '%s', does filepath exist: '%s' is it a string: '%s'",
                soundName.data(),
                value.HasMember("filepath") ? "true" : "false", value.HasMember("filepath") ? value["filepath"].IsString() ? "true" : "false" : "unknown");
            return false;
        }
        filepath = value["filepath"].GetString();
        return success;
    }


    void SaveConfig()
    {
        getLogger().debug("Save config run");
        getConfig().config.RemoveAllMembers();
        getConfig().config.SetObject();
        auto& allocator = getConfig().config.GetAllocator();

        ConfigValue soundsValue(rapidjson::kObjectType);
        AddChildSound(soundsValue, "HitSound", Config.hitSound_Active, Config.hitSound_filepath , Config.hitSound_beatOffSet, Config.hitSound_audioVolumeOffset, allocator);
        AddChildSound(soundsValue, "BadHitSound", Config.badHitSound_Active, Config.badHitSound_filepath, Config.badHitSound_audioVolumeOffset, allocator);
        AddChildSound(soundsValue, "NoteMissedSound", Config.noteMissedSound_Active, Config.noteMissedSound_filepath, Config.noteMissedSound_audioVolumeOffset, allocator);
        AddChildSound(soundsValue, "MenuMusic", Config.menuMusic_Active, Config.menuMusic_filepath, allocator);
        AddChildSound(soundsValue, "MenuClick", Config.menuClick_Active, Config.menuClick_filepath, allocator);
        AddChildSound(soundsValue, "Firework", Config.firework_Active, Config.firework_filepath, allocator);
        AddChildSound(soundsValue, "LevelCleared", Config.levelCleared_Active, Config.levelCleared_filepath, allocator);
        AddChildSound(soundsValue, "LevelFailed", Config.levelFailed_Active, Config.levelFailed_filepath, Config.levelFailed_audioVolumeOffset, allocator);
        AddChildSound(soundsValue, "LobbyMusic", Config.lobbyAmbience_Active, Config.lobbyAmbience_filepath, allocator);
        getConfig().config.AddMember(CONFIG_VERSION, soundsValue, allocator);
        getConfig().Write();
        LegacyConfig = false;
    }

    bool LoadConfig()
    {
        getConfig().Load();

        bool parseError = false;
        getLogger().debug("PreCheck: parseError currentValue: %d will be returned as: %d", parseError, !parseError);
        if (getConfig().config.HasMember(CONFIG_VERSION) && getConfig().config[CONFIG_VERSION].IsObject())
        {
            ConfigValue soundsValue = getConfig().config[CONFIG_VERSION].GetObject();
            parseError |= !TryParseSound(Config.hitSound_Active, Config.hitSound_filepath, Config.hitSound_beatOffSet, Config.hitSound_audioVolumeOffset, soundsValue, "HitSound");
            parseError |= !TryParseSound(Config.badHitSound_Active, Config.badHitSound_filepath, Config.badHitSound_audioVolumeOffset, soundsValue, "BadHitSound");
            parseError |= !TryParseSound(Config.noteMissedSound_Active, Config.noteMissedSound_filepath, Config.noteMissedSound_audioVolumeOffset, soundsValue, "NoteMissedSound");
            parseError |= !TryParseSound(Config.menuMusic_Active, Config.menuMusic_filepath, soundsValue, "MenuMusic");
            parseError |= !TryParseSound(Config.menuClick_Active, Config.menuClick_filepath, soundsValue, "MenuClick");
            parseError |= !TryParseSound(Config.firework_Active, Config.firework_filepath, soundsValue, "Firework");
            parseError |= !TryParseSound(Config.levelCleared_Active, Config.levelCleared_filepath, soundsValue, "LevelCleared");
            parseError |= !TryParseSound(Config.levelFailed_Active, Config.levelFailed_filepath, Config.levelFailed_audioVolumeOffset, soundsValue, "LevelFailed");
            parseError |= !TryParseSound(Config.lobbyAmbience_Active, Config.lobbyAmbience_filepath, soundsValue, "LobbyMusic");
            getLogger().debug("PostCheck: parseError currentValue: %d will be returned as: %d", parseError, !parseError);
            getLogger().debug("Config Loaded OK");
            return !parseError;
        }
        else if (getConfig().config.HasMember(CONFIG_VERSION_PRE_1_20) && getConfig().config[CONFIG_VERSION_PRE_1_20].IsObject())
        {
            ConfigValue soundsValue = getConfig().config[CONFIG_VERSION_PRE_1_20].GetObject();
            parseError |= !Legacy::TryParseSound(Config.hitSound_Active, Config.hitSound_filepath, Config.hitSound_beatOffSet, soundsValue, "HitSound");
            parseError |= !TryParseSound(Config.badHitSound_Active, Config.badHitSound_filepath, soundsValue, "BadHitSound");
            parseError |= !TryParseSound(Config.menuMusic_Active, Config.menuMusic_filepath, soundsValue, "MenuMusic");
            parseError |= !TryParseSound(Config.menuClick_Active, Config.menuClick_filepath, soundsValue, "MenuClick");
            parseError |= !TryParseSound(Config.firework_Active, Config.firework_filepath, soundsValue, "Firework");
            parseError |= !TryParseSound(Config.levelCleared_Active, Config.levelCleared_filepath, soundsValue, "LevelCleared");
            parseError |= !TryParseSound(Config.levelFailed_Active, Config.levelFailed_filepath, soundsValue, "LevelFailed");
            parseError |= !TryParseSound(Config.lobbyAmbience_Active, Config.lobbyAmbience_filepath, soundsValue, "LobbyMusic");
        }
        else if (getConfig().config.HasMember(CONFIG_VERSION_LEGACY) && getConfig().config[CONFIG_VERSION_LEGACY].IsObject()) {
            ConfigValue soundsValue = getConfig().config[CONFIG_VERSION_LEGACY].GetObject();
            parseError |= !TryParseSound(Config.hitSound_Active, Config.hitSound_filepath, soundsValue, "HitSound");
            parseError |= !TryParseSound(Config.badHitSound_Active, Config.badHitSound_filepath, soundsValue, "BadHitSound");
            parseError |= !TryParseSound(Config.menuMusic_Active, Config.menuMusic_filepath, soundsValue, "MenuMusic");
            parseError |= !TryParseSound(Config.menuClick_Active, Config.menuClick_filepath, soundsValue, "MenuClick");
            parseError |= !TryParseSound(Config.firework_Active, Config.firework_filepath, soundsValue, "Firework");
            parseError |= !TryParseSound(Config.levelCleared_Active, Config.levelCleared_filepath, soundsValue, "LevelCleared");
            LegacyConfig = true;
        }
        else if (getConfig().config.HasMember(CONFIG_VERSION_PRE_R) && getConfig().config[CONFIG_VERSION_PRE_R].IsObject()) {
            ConfigValue soundsValue = getConfig().config[CONFIG_VERSION_PRE_R].GetObject();
            parseError |= !TryParseSound(Config.hitSound_Active, Config.hitSound_filepath, soundsValue, "HitSound");
            parseError |= !TryParseSound(Config.badHitSound_Active, Config.badHitSound_filepath, soundsValue, "BadHitSound");
            parseError |= !TryParseSound(Config.menuMusic_Active, Config.menuMusic_filepath, soundsValue, "MenuMusic");
            parseError |= !TryParseSound(Config.menuClick_Active, Config.menuClick_filepath, soundsValue, "MenuClick");
            parseError |= !TryParseSound(Config.firework_Active, Config.firework_filepath, soundsValue, "Firework");
            parseError |= !TryParseSound(Config.levelCleared_Active, Config.levelCleared_filepath, soundsValue, "LevelCleared");
            parseError |= !TryParseSound(Config.lobbyAmbience_Active, Config.lobbyAmbience_filepath, soundsValue, "LobbyMusic");
            LegacyConfig = true;
        }
        else {
            getLogger().debug("Config Mismatch");
            return false;
        }
        if (!parseError) {
            getLogger().debug("Config Loaded OK");
        }
        else {
            getLogger().debug("Config Loaded with errors");
        }
        getLogger().debug("PostCheck: parseError currentValue: %d will be returned as: %d", parseError, !parseError);
        return !parseError;
    }
}