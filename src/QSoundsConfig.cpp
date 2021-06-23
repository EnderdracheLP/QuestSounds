#include "audiocliploader.hpp"
#include "QSoundsConfig.hpp"

namespace QSoundsConfig {

    //std::string soundPath = string_format(SOUND_PATH_FORMAT, Modloader::getApplicationId().c_str());

    void AddChildSound(ConfigValue& parent, std::string_view soundName, bool active, std::string filePath, /*float Volume,*/ ConfigDocument::AllocatorType& allocator)
    {
        ConfigValue value(rapidjson::kObjectType);
        value.AddMember("activated", active, allocator);
        std::string data(filePath);
        value.AddMember("filepath", data, allocator);
        //value.AddMember("Volume", Volume, allocator);
        parent.AddMember((ConfigValue::StringRefType)soundName.data(), value, allocator);
    }

    // TODO: Fails here
    // Edited ParseVector made by Darknight1050
    bool ParseSound(bool& active, std::string& filepath, /*float& Volume,*/ ConfigValue& parent, std::string_view soundName)
    {
        if (!parent.HasMember(soundName.data()) || !parent[soundName.data()].IsObject()) {
            getLogger().error("Error Parsing, %s, parent member bool: %d, parent object bool: %d", soundName.data(), parent.HasMember(soundName.data()), parent[soundName.data()].IsObject());
            return false;
        }
        ConfigValue value = parent[soundName.data()].GetObject();
        if (!(value.HasMember("activated") && value["activated"].IsBool() &&
            value.HasMember("filepath") && value["filepath"].IsString() /*&&
            value.HasMember("Volume") && value["Volume"].IsFloat()*/)) {
            std::string test(value["filepath"].GetString());
            getLogger().error("Error Parsing, %s, bool activated is: %d, string filepath is: %s", std::string(soundName).c_str(), active, value["activated"].GetBool(), test.c_str());
            return false;
        }
        active = value["activated"].GetBool();
        filepath = value["filepath"].GetString();
        //Volume = value["Volume"].GetFloat();
        return true;
    }

    void SaveConfig()
    {
        getLogger().debug("Save config run");
        getConfig().config.RemoveAllMembers();
        getConfig().config.SetObject();
        auto& allocator = getConfig().config.GetAllocator();

        ConfigValue soundsValue(rapidjson::kObjectType);
        AddChildSound(soundsValue, "HitSound", Config.hitSound_Active, Config.hitSound_filepath, /*Config.hitSound_Volume,*/ allocator);
        AddChildSound(soundsValue, "BadHitSound", Config.badHitSound_Active, Config.badHitSound_filepath, /*Config.badHitSound_Volume,*/ allocator);
        AddChildSound(soundsValue, "MenuMusic", Config.menuMusic_Active, Config.menuMusic_filepath, /*Config.menuMusic_Volume,*/ allocator);
        AddChildSound(soundsValue, "MenuClick", Config.menuClick_Active, Config.menuClick_filepath, /*Config.menuClick_Volume,*/ allocator);
        AddChildSound(soundsValue, "Firework", Config.firework_Active, Config.firework_filepath, /*Config.firework_Volume,*/ allocator);
        AddChildSound(soundsValue, "LevelCleared", Config.levelCleared_Active, Config.levelCleared_filepath, /*Config.levelCleared_Volume,*/ allocator);
        AddChildSound(soundsValue, "LobbyMusic", Config.lobbyAmbience_Active, Config.lobbyAmbience_filepath, /*Config.lobbyAmbience_Volume,*/ allocator);
        getConfig().config.AddMember(CONFIG_VERSION, soundsValue, allocator);
        getConfig().Write();
    }

    bool LoadConfig()
    {
        getConfig().Load();

        if (getConfig().config.HasMember(CONFIG_VERSION) && getConfig().config[CONFIG_VERSION].IsObject())
        {
            ConfigValue soundsValue = getConfig().config[CONFIG_VERSION].GetObject();
            if (!ParseSound(Config.hitSound_Active, Config.hitSound_filepath, /*Config.hitSound_Volume,*/ soundsValue, "HitSound")) return false;
            if (!ParseSound(Config.badHitSound_Active, Config.badHitSound_filepath, /*Config.badHitSound_Volume,*/ soundsValue, "BadHitSound")) return false;
            if (!ParseSound(Config.menuMusic_Active, Config.menuMusic_filepath, /*Config.menuMusic_Volume,*/ soundsValue, "MenuMusic")) return false;
            if (!ParseSound(Config.menuClick_Active, Config.menuClick_filepath, /*Config.menuClick_Volume,*/ soundsValue, "MenuClick")) return false;
            if (!ParseSound(Config.firework_Active, Config.firework_filepath, /*Config.firework_Volume,*/ soundsValue, "Firework")) return false;
            if (!ParseSound(Config.levelCleared_Active, Config.levelCleared_filepath, /*Config.levelCleared_Volume,*/ soundsValue, "LevelCleared")) return false;
            if (!ParseSound(Config.lobbyAmbience_Active, Config.lobbyAmbience_filepath, /*Config.lobbyAmbience_Volume,*/ soundsValue, "LobbyMusic")) return false;
        }
        else {
            getLogger().debug("Config Mismatch");
            return false;
        }
        getLogger().debug("Config Loaded OK");
        return true;
    }
}