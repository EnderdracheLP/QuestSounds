#pragma once

#include "custom-types/shared/macros.hpp"
#include "bsml/shared/macros.hpp"
#include "bsml/shared/BSML/Components/Settings/IncrementSetting.hpp"
#include "bsml/shared/BSML/Components/Settings/SliderSetting.hpp"

#include "Config.hpp"
#include "Utils/AsyncAudioClipLoader.hpp"

#include "HMUI/ViewController.hpp"

DECLARE_CLASS_CODEGEN(QuestSounds::UI, SoundSettingsViewController, HMUI::ViewController,
    std::string Name;
    QuestSounds::Sound* Sound;
    QuestSounds::Utils::AsyncAudioClipLoader* Loader;

    DECLARE_BSML_PROPERTY(bool, Active);
    DECLARE_BSML_PROPERTY(float, VolumeOffset);
    DECLARE_BSML_PROPERTY(float, BeatOffset);
    DECLARE_BSML_PROPERTY(bool, HasSoundFiles);

    DECLARE_INSTANCE_FIELD(BSML::SliderSetting*, SoundVolumeOffset);
    DECLARE_INSTANCE_FIELD(BSML::SliderSetting*, SoundBeatOffset);

    DECLARE_INSTANCE_FIELD(bool, showSoundVolumeOffset);
    DECLARE_INSTANCE_FIELD(bool, showSoundBeatOffset);

    DECLARE_OVERRIDE_METHOD_MATCH(void, DidActivate, &HMUI::ViewController::DidActivate, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD_MATCH(void, DidDeactivate, &HMUI::ViewController::DidDeactivate, bool removedFromHierarchy, bool screenSystemDisabling);

public:
    void Setup(std::string name, QuestSounds::Sound* sound, QuestSounds::Utils::AsyncAudioClipLoader* loader);
)