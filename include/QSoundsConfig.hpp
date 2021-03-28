#pragma once
#define RAPIDJSON_HAS_STDSTRING 1
#include "main.hpp"
#include "audiocliploader.hpp"
#define RAPIDJSON_HAS_STDSTRING 1
#define SOUND_PATH_FORMAT "/sdcard/ModData/%s/Mods/QuestSounds/"

std::string soundPath = string_format(SOUND_PATH_FORMAT, Modloader::getApplicationId().c_str());

//Config stuff
struct Config_t
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
}; 
extern Config_t Config;


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
    if (!parent.HasMember(soundName.data()) || !parent[soundName.data()].IsObject()) return false;
    ConfigValue value = parent[soundName.data()].GetObject();
    if (!(value.HasMember("activated") && value["activated"].IsBool() &&
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
    getConfig().config.AddMember("Sounds v1", soundsValue, allocator);
    getConfig().Write();
}

bool LoadConfig()
{
    getConfig().Load();

    if (getConfig().config.HasMember("Sounds v1") && getConfig().config["Sounds v1"].IsObject())
    {
        ConfigValue soundsValue = getConfig().config["Sounds v1"].GetObject();
        if (!ParseSound(Config.hitSound_Active, Config.hitSound_filepath, Config.hitSound_Volume, soundsValue, "HitSound")) return false;
        if (!ParseSound(Config.badHitSound_Active, Config.badHitSound_filepath, Config.badHitSound_Volume, soundsValue, "BadHitSound")) return false;
        if (!ParseSound(Config.menuMusic_Active, Config.menuMusic_filepath, Config.menuMusic_Volume, soundsValue, "MenuMusic")) return false;
        if (!ParseSound(Config.menuClick_Active, Config.menuClick_filepath, Config.menuClick_Volume, soundsValue, "MenuClick")) return false;
        if (!ParseSound(Config.firework_Active, Config.firework_filepath, Config.firework_Volume, soundsValue, "Firework")) return false;
        if (!ParseSound(Config.levelCleared_Active, Config.levelCleared_filepath, Config.levelCleared_Volume, soundsValue, "LevelCleared")) return false;
    }
    else return false;

    return true;
}
