#include "UI/SoundSettingsViewController.hpp"
#include "UI/QuestSoundsFlowCoordinator.hpp"
#include "logging.hpp"
#include "assets.hpp"

#include "bsml/shared/BSML.hpp"
#include "bsml/shared/BSML-Lite.hpp"

#include "UnityEngine/WaitForSeconds.hpp"

#include <regex>

DEFINE_TYPE(QuestSounds::UI, SoundSettingsViewController);

namespace QuestSounds::UI {
    void SoundSettingsViewController::ctor() {
        _Sounds = ListW<BSML::CustomCellInfo*>::New();
        getLogger().info("ctor");
    }

    void SoundSettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        if (firstActivation) {
            getLogger().info("DidActivate firstActivation");
            BSML::parse_and_construct(Assets::SoundSettingsViewController_bsml, get_transform(), this);
        }
        if (SoundEnabled) {
            SoundEnabled->set_Value(Sound->Active);
        }
        if (SoundVolumeOffset) {
            getLogger().info("DidActivate SoundVolumeOffset");
            SoundVolumeOffset->gameObject->SetActive(showSoundVolumeOffset);
            if (showSoundVolumeOffset)
                SoundVolumeOffset->set_Value(Sound->VolumeOffset.value());
        }
        if (SoundBeatOffset) {
            getLogger().info("DidActivate SoundBeatOffset");
            SoundBeatOffset->gameObject->SetActive(showSoundBeatOffset);
            if (showSoundBeatOffset)
                SoundBeatOffset->set_Value(Sound->BeatOffset.value());
        }

        if (_Sounds->Count <= 0)
        {
            getLogger().info("No sound files found for sound: '{}'", name);
            // set_HasSoundFiles(false);
            if (NoSoundsFoundHorizontal) NoSoundsFoundHorizontal->gameObject->SetActive(true);
            if (SoundList) SoundList->gameObject->SetActive(false);
        }
        else
        {
            getLogger().info("Sound files found for sound: '{}'", name);
            // set_HasSoundFiles(true);
            if (NoSoundsFoundHorizontal) NoSoundsFoundHorizontal->gameObject->SetActive(false);
            if (SoundList) SoundList->gameObject->SetActive(true);
            for (int i = 0; i < _Sounds->Count; i++) {
                auto cell = il2cpp_utils::cast<CustomSoundFileCell>(_Sounds[i]);
                if (cell->FilePath == Sound->FilePath) {
                    SoundList->tableView->SelectCellWithIdx(i, false);
                    break;
                }
                
            }
        }
        getLogger().info("DidActivate");
    }

    void SoundSettingsViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling) {
        getLogger().info("DidDeactivate");

        if (Loader->audioSource && Loader->audioSource->isPlaying) {
            Loader->audioSource->Stop();
        }
    }

    void SoundSettingsViewController::Setup(std::string name, QuestSounds::Sound* sound, QuestSounds::Utils::AsyncAudioClipLoader* loader, std::optional<std::function<void()>> onSoundChanged) {
        getLogger().info("Setup for sound: '{}' cfg values: active='{}', filepath='{}' FolderPath='{}'", name, sound->Active, sound->FilePath, sound->FolderPath);
        // Make sure any previous audio is stopped
        if (Loader && Loader->audioSource && Loader->audioSource->isPlaying) {
            Loader->audioSource->Stop();
        }
        
        Name = name;
        Sound = sound;
        Loader = loader;
        OnSoundChanged = onSoundChanged;

        showSoundVolumeOffset = Sound->VolumeOffset.has_value();
        showSoundBeatOffset = Sound->BeatOffset.has_value();

        // if (SoundEnabled) {
        //     SoundEnabled->set_Value(Sound->Active);
        // }

        // if (SoundVolumeOffset) {
        //     SoundVolumeOffset->gameObject->SetActive(showSoundVolumeOffset);
        //     if (showSoundVolumeOffset)
        //         SoundVolumeOffset->set_Value(Sound->VolumeOffset.value());
        // }

        // if (SoundBeatOffset) {
        //     SoundBeatOffset->gameObject->SetActive(showSoundBeatOffset);
        //     if (showSoundBeatOffset)
        //         SoundBeatOffset->set_Value(Sound->BeatOffset.value());
        // }

        // Load list of available Sound Files in the folder
        if (_Sounds) {
            _Sounds->Clear();
            for (const auto& entry : std::filesystem::directory_iterator(Sound->FolderPath)) {
                if (entry.is_regular_file() && std::regex_search(entry.path().extension().string(), std::regex("ogg|mp3|mp2|wav|aiff|aif"))) {
                    getLogger().info("Found sound file: '{}'", entry.path().string());
                    CustomSoundFileCell* cell = CustomSoundFileCell::construct(entry.path().stem().string(), entry.path(), nullptr);
                    _Sounds->Add(cell);
                }
            }

            // if (_Sounds->Count <= 0)
            // {
            //     getLogger().info("No sound files found for sound: '{}'", name);
            //     // set_HasSoundFiles(false);
            //     if (NoSoundsFoundHorizontal) NoSoundsFoundHorizontal->gameObject->SetActive(true);
            //     if (SoundList) SoundList->gameObject->SetActive(false);
            // }
            // else
            // {
            //     getLogger().info("Sound files found for sound: '{}'", name);
            //     // set_HasSoundFiles(true);
            //     if (NoSoundsFoundHorizontal) NoSoundsFoundHorizontal->gameObject->SetActive(false);
            //     if (SoundList) {
            //         SoundList->gameObject->SetActive(true);
            //         SoundList->tableView->ReloadData();
            //     }
            // }
        }

        if (wasActivatedBefore && !_IsSetup) {
            set_IsSetup(true);
            getLogger().info("Disabling NotSetupHorizontal");
            NotSetupHorizontal->gameObject->SetActive(false);
            // // Destroying UI
            // auto t = get_transform();
            // int childCount = t->get_childCount();
            // for (int i = 0; i < childCount; i++)
            //     Object::DestroyImmediate(t->GetChild(0)->get_gameObject());

            // Recreating UI
            getLogger().info("Recreating UI");
            DidActivate(true, false, false);
        } else {
            DidActivate(false, false, false);
        }
    }

    void SoundSettingsViewController::set_Active(bool value) {
        getLogger().info("Setting Active for sound: '{}' to '{}'", Name, value);
        _Active = value;
        Sound->Active = value;

        if (Loader && Loader->audioSource && Loader->audioSource->isPlaying) {
            Loader->audioSource->Stop();
        }
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

    void SoundSettingsViewController::set_IsSetup(bool value) {
        getLogger().info("Setting IsSetup for sound: '{}' to '{}'", Name, value);
        _IsSetup = value;
    }

    bool SoundSettingsViewController::get_IsSetup() {
        getLogger().info("Getting IsSetup for sound: '{}' as '{}'", Name, _IsSetup);
        return _IsSetup;
    }

    ListW<BSML::CustomCellInfo*> SoundSettingsViewController::get_Sounds() {
        getLogger().info("Getting Sound Files for sound: '{}'", Name);
        return _Sounds;
    }

    custom_types::Helpers::Coroutine SoundSettingsViewController::PreviewSelection() {
        while (!Loader->loaded) {
            co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForSeconds::New_ctor(0.1f));
        }

        if (Loader->loaded && Loader->audioSource) {
            getLogger().info("PreviewSelection for sound: '{}'", Name);
            Loader->audioSource->set_volume(0.6f);
            Loader->audioSource->Play();

            if (OnSoundChanged.has_value()) {
                OnSoundChanged.value()();
            } else getLogger().info("OnSoundChanged is not set for sound: '{}'", Name);
        } else getLogger().error("PreviewSelection failed for sound: '{}'", Name);
    }

    void SoundSettingsViewController::SoundSelected(HMUI::TableView* table, int cellIdx) {
        getLogger().info("SoundSelected for sound: '{}' at index: '{}'", Name, cellIdx);
        if (cellIdx < 0 || cellIdx > _Sounds->Count) return;
        auto cell = il2cpp_utils::cast<CustomSoundFileCell>(_Sounds[cellIdx]);
        getLogger().info("Selected Sound: '{}'", cell->text);
        Sound->FilePath = cell->FilePath.string();
        StopAllCoroutines();
        Loader->filePath = Sound->FilePath;
        if (Loader->audioSource && Loader->audioSource->isPlaying) {
            Loader->audioSource->Stop();
        }
        Loader->loaded = false;
        Loader->load();
        StartCoroutine(custom_types::Helpers::CoroutineHelper::New(PreviewSelection()));
    }
}