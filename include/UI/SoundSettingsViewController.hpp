#pragma once

#include "custom-types/shared/macros.hpp"
#include "bsml/shared/macros.hpp"
#include "bsml/shared/BSML/Components/Settings/ToggleSetting.hpp"
#include "bsml/shared/BSML/Components/Settings/IncrementSetting.hpp"
#include "bsml/shared/BSML/Components/Settings/SliderSetting.hpp"
#include "bsml/shared/BSML/Components/CustomListTableData.hpp"

#include "Config.hpp"
#include "Utils/AsyncAudioClipLoader.hpp"

#include "UI/CustomSoundFileCell.hpp"

#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"

#include "HMUI/ViewController.hpp"

DECLARE_CLASS_CODEGEN(QuestSounds::UI, SoundSettingsViewController, HMUI::ViewController) {
    std::string Name;
    QuestSounds::Sound* Sound;
    QuestSounds::Utils::AsyncAudioClipLoader* Loader;

    std::optional<std::function<void()>> OnSoundChanged;

    DECLARE_BSML_PROPERTY(bool, Active);
    DECLARE_BSML_PROPERTY(float, VolumeOffset);
    DECLARE_BSML_PROPERTY(float, BeatOffset);
    DECLARE_BSML_PROPERTY(bool, HasSoundFiles);
    DECLARE_BSML_PROPERTY(bool, IsSetup);

    DECLARE_INSTANCE_FIELD(ListW<BSML::CustomCellInfo*>, _Sounds);
    DECLARE_INSTANCE_METHOD(ListW<BSML::CustomCellInfo*>, get_Sounds);
    DECLARE_INSTANCE_FIELD(BSML::CustomListTableData*, SoundList);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::HorizontalLayoutGroup*, NoSoundsFoundHorizontal);

    DECLARE_INSTANCE_FIELD(BSML::ToggleSetting*, SoundEnabled);
    DECLARE_INSTANCE_FIELD(BSML::SliderSetting*, SoundVolumeOffset);
    DECLARE_INSTANCE_FIELD(BSML::SliderSetting*, SoundBeatOffset);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::HorizontalLayoutGroup*, NotSetupHorizontal);

    DECLARE_INSTANCE_FIELD(bool, showSoundVolumeOffset);
    DECLARE_INSTANCE_FIELD(bool, showSoundBeatOffset);

    DECLARE_OVERRIDE_METHOD_MATCH(void, DidActivate, &HMUI::ViewController::DidActivate, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD_MATCH(void, DidDeactivate, &HMUI::ViewController::DidDeactivate, bool removedFromHierarchy, bool screenSystemDisabling);

    DECLARE_INSTANCE_METHOD(void, SoundSelected, HMUI::TableView* table, int cellIdx);

    DECLARE_CTOR(ctor);

public:
    void Setup(std::string name, QuestSounds::Sound* sound, QuestSounds::Utils::AsyncAudioClipLoader* loader, std::optional<std::function<void()>> onSoundChanged = std::nullopt);

    custom_types::Helpers::Coroutine PreviewSelection();
};