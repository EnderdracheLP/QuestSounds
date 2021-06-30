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

    // Edited ParseVector made by Darknight1050
    bool ParseSound(bool& active, std::string& filepath, ConfigValue& parent, std::string_view soundName)
    {
        if (!parent.HasMember(soundName.data()) || !parent[soundName.data()].IsObject()) {
            getLogger().error("Error Parsing, %s, parent member bool: %d, parent object bool: %d", soundName.data(), parent.HasMember(soundName.data()), parent[soundName.data()].IsObject());
            return false;
        }
        ConfigValue value = parent[soundName.data()].GetObject();
        if (!(value.HasMember("activated") && value["activated"].IsBool() &&
            value.HasMember("filepath") && value["filepath"].IsString())) {
            getLogger().error("Error Parsing, %s, bool activated is: %d, string filepath is: %s", soundName.data(), active, value["activated"].GetBool(), std::string(value["filepath"].GetString()).c_str());
            return false;
        }
        active = value["activated"].GetBool();
        filepath = value["filepath"].GetString();
        return true;
    }

    void SaveConfig()
    {
        getLogger().debug("Save config run");
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
        AddChildSound(soundsValue, "LobbyMusic", Config.lobbyAmbience_Active, Config.lobbyAmbience_filepath, allocator);
        getConfig().config.AddMember(CONFIG_VERSION, soundsValue, allocator);
        getConfig().Write();
    }

    bool LoadConfig()
    {
        getConfig().Load();

        if (getConfig().config.HasMember(CONFIG_VERSION) && getConfig().config[CONFIG_VERSION].IsObject())
        {
            ConfigValue soundsValue = getConfig().config[CONFIG_VERSION].GetObject();
            if (!ParseSound(Config.hitSound_Active, Config.hitSound_filepath, soundsValue, "HitSound")) return false;
            if (!ParseSound(Config.badHitSound_Active, Config.badHitSound_filepath, soundsValue, "BadHitSound")) return false;
            if (!ParseSound(Config.menuMusic_Active, Config.menuMusic_filepath, soundsValue, "MenuMusic")) return false;
            if (!ParseSound(Config.menuClick_Active, Config.menuClick_filepath, soundsValue, "MenuClick")) return false;
            if (!ParseSound(Config.firework_Active, Config.firework_filepath, soundsValue, "Firework")) return false;
            if (!ParseSound(Config.levelCleared_Active, Config.levelCleared_filepath, soundsValue, "LevelCleared")) return false;
            if (!ParseSound(Config.lobbyAmbience_Active, Config.lobbyAmbience_filepath, soundsValue, "LobbyMusic")) return false;
        }
        else if (getConfig().config.HasMember("SoundsConfig_v1") && getConfig().config["SoundsConfig_v1"].IsObject()) {
            ConfigValue soundsValue = getConfig().config["SoundsConfig_v1"].GetObject();
            if (!ParseSound(Config.hitSound_Active, Config.hitSound_filepath, soundsValue, "HitSound")) return false;
            if (!ParseSound(Config.badHitSound_Active, Config.badHitSound_filepath, soundsValue, "BadHitSound")) return false;
            if (!ParseSound(Config.menuMusic_Active, Config.menuMusic_filepath, soundsValue, "MenuMusic")) return false;
            if (!ParseSound(Config.menuClick_Active, Config.menuClick_filepath, soundsValue, "MenuClick")) return false;
            if (!ParseSound(Config.firework_Active, Config.firework_filepath, soundsValue, "Firework")) return false;
            if (!ParseSound(Config.levelCleared_Active, Config.levelCleared_filepath, soundsValue, "LevelCleared")) return false;
            LegacyConfig = true;
        }
        else {
            getLogger().debug("Config Mismatch");
            return false;
        }
        getLogger().debug("Config Loaded OK");
        return true;
    }
}