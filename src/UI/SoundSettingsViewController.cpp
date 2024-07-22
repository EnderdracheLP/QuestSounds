#include "UI/SoundSettingsViewController.hpp"
#include "UI/QuestSoundsFlowCoordinator.hpp"
#include "logging.hpp"
#include "assets.hpp"

#include "bsml/shared/BSML.hpp"

DEFINE_TYPE(QuestSounds::UI, SoundSettingsViewController);

namespace QuestSounds::UI {
    void SoundSettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        if (firstActivation) {
            getLogger().info("DidActivate firstActivation");
            BSML::parse_and_construct(Assets::SoundSettingsViewController_bsml, get_transform(), this);
        }
        if (SoundVolumeOffset) {
            getLogger().info("DidActivate SoundVolumeOffset");
            SoundVolumeOffset->gameObject->SetActive(showSoundVolumeOffset);
            SoundVolumeOffset->set_Value(Sound->VolumeOffset.value());
        }
        if (SoundBeatOffset) {
            getLogger().info("DidActivate SoundBeatOffset");
            SoundBeatOffset->gameObject->SetActive(showSoundBeatOffset);
            SoundBeatOffset->set_Value(Sound->BeatOffset.value());
        }
        getLogger().info("DidActivate");
    }

    void SoundSettingsViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling) {
        getLogger().info("DidDeactivate");
    }

    void SoundSettingsViewController::Setup(std::string name, QuestSounds::Sound* sound, QuestSounds::Utils::AsyncAudioClipLoader* loader) {
        getLogger().info("Setup for sound: '{}' cfg values: active='{}', filepath='{}' FolderPath='{}'", name, sound->Active, sound->FilePath, sound->FolderPath);
        Name = name;
        Sound = sound;
        Loader = loader;

        showSoundVolumeOffset = sound->VolumeOffset.has_value();
        showSoundBeatOffset = sound->BeatOffset.has_value();

        if (SoundVolumeOffset && showSoundVolumeOffset) {
            SoundVolumeOffset->set_Value(sound->VolumeOffset.value());
        }

        if (SoundBeatOffset && showSoundBeatOffset) {
            SoundBeatOffset->set_Value(sound->BeatOffset.has_value());
        }
    }

    void SoundSettingsViewController::set_Active(bool value) {
        getLogger().info("Setting Active for sound: '{}' to '{}'", Name, value);
        _Active = value;
        Sound->Active = value;
    }

    bool SoundSettingsViewController::get_Active() {
        getLogger().info("Getting Active for sound: '{}' as '{}'", Name, Sound->Active);
        return Sound->Active;
    }

    void SoundSettingsViewController::set_VolumeOffset(float value) {
        getLogger().info("Setting VolumeOffset for sound: '{}' to '{}'", Name, value);
        _VolumeOffset = value;
        Sound->VolumeOffset = value;
    }

    float SoundSettingsViewController::get_VolumeOffset() {
        getLogger().info("Getting VolumeOffset for sound: '{}' as '{}'", Name, Sound->VolumeOffset.value_or(0));
        return Sound->VolumeOffset.value_or(0);
    }

    void SoundSettingsViewController::set_BeatOffset(float value) {
        getLogger().info("Setting BeatOffset for sound: '{}' to '{}'", Name, value);
        _BeatOffset = value;
        Sound->BeatOffset = value;
    }

    float SoundSettingsViewController::get_BeatOffset() {
        getLogger().info("Getting BeatOffset for sound: '{}' as '{}'", Name, Sound->BeatOffset.value_or(0.185f));
        return Sound->BeatOffset.value_or(0.185f);
    }

    void SoundSettingsViewController::set_HasSoundFiles(bool value) {
        getLogger().info("Setting HasSoundFiles for sound: '{}' to '{}'", Name, value);
        _HasSoundFiles = value;
    }

    bool SoundSettingsViewController::get_HasSoundFiles() {
        getLogger().info("Getting HasSoundFiles for sound: '{}' as '{}'", Name, _HasSoundFiles);
        return _HasSoundFiles;
    }
}