#include "main.hpp"
#include "QSoundsConfig.hpp"
#include "ViewControllers/NoteMissedSdListViewController.hpp"
#include "AudioClips.hpp"

#include <dirent.h>
#include <regex>
#include <list>




#include "UnityEngine/Object.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "TMPro/TextMeshProUGUI.hpp"

using namespace QuestSounds;

#ifndef REGISTER_FUNCTION
DEFINE_TYPE(QuestSounds::ViewControllers, NoteMissedSdListViewController);
#elif defined(DEFINE_TYPE)
DEFINE_TYPE(QuestSounds::ViewControllers::NoteMissedSdListViewController);
#elif defined(DEFINE_CLASS)
DEFINE_CLASS(QuestSounds::ViewControllers::NoteMissedSdListViewController);
#endif

namespace QuestSounds::ViewControllers {

    NoteMissedSdListViewController* NoteMissedListView;
    std::list<UnityEngine::UI::Button*> NoteMissedQSlist = {};

    void NoteMissedSelectSound()
    {
        for (UnityEngine::UI::Button* button : NoteMissedQSlist)
        {
            if (button->get_hasSelection())
            {
                std::string filename = static_cast<std::string>(button->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->get_text());
                QSoundsConfig::Config.noteMissedSound_filepath = QSoundsConfig::NoteMissedSoundPath + filename;
                AudioClips::noteMissedSoundLoader.filePath = QSoundsConfig::Config.noteMissedSound_filepath;
                if (AudioClips::noteMissedSoundLoader.audioSource != nullptr) AudioClips::noteMissedSoundLoader.audioSource->Stop();
                AudioClips::noteMissedSoundLoader.load();
                std::thread PlayAudio([&]() {
                    while (!AudioClips::noteMissedSoundLoader.loaded && QSoundsConfig::Config.noteMissedSound_Active) {
                        usleep(100);
                    }
                    if (!QSoundsConfig::Config.noteMissedSound_Active) {
                        return;
                    }
                    if (AudioClips::noteMissedSoundLoader.audioSource != nullptr) {
                        AudioClips::noteMissedSoundLoader.audioSource->set_volume(0.5f + QSoundsConfig::Config.noteMissedSound_audioVolumeOffset);
                        return AudioClips::noteMissedSoundLoader.audioSource->Play();
                    }
                    });
                PlayAudio.detach();
                getLogger().debug("Selected Sound Path %s", QSoundsConfig::Config.badHitSound_filepath.c_str());
            }
        }
    }

    void NoteMissedRefreshList()
    {
        if (NoteMissedListView->listtxtgroup && NoteMissedListView->listtxtgroup->m_CachedPtr.m_value) UnityEngine::GameObject::Destroy(NoteMissedListView->listtxtgroup->get_gameObject());
        for (UnityEngine::UI::Button* button : NoteMissedQSlist) UnityEngine::Object::Destroy(button->get_transform()->get_parent()->get_gameObject());
        NoteMissedQSlist = {};
        DIR* sounddir = opendir(QSoundsConfig::NoteMissedSoundPath.c_str());
        dirent* fileent;
        while ((fileent = readdir(sounddir)) != NULL)
        {
            std::string filename = fileent->d_name;
            for (char& ch : filename) ch = tolower(ch);

            if (std::regex_search(filename, std::regex(".ogg|.mp3|.mp2|.wav|.aiff|.aif")))
            {
                UnityEngine::UI::HorizontalLayoutGroup* rowgroup = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(NoteMissedListView->SDlistscroll->get_transform());
                UnityEngine::UI::Button* button = QuestUI::BeatSaberUI::CreateUIButton(rowgroup->get_rectTransform(), fileent->d_name, NoteMissedSelectSound);
                button->get_gameObject()->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_fontStyle(2);
                NoteMissedQSlist.push_back(button);
            }
        }
        if (NoteMissedQSlist.size() < 1)
        {
            NoteMissedListView->listtxtgroup = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(NoteMissedListView->SDlistscroll->get_transform());
            QuestUI::BeatSaberUI::CreateText(NoteMissedListView->listtxtgroup->get_rectTransform(), "No sound files were found!\nPlease add a sound file into\n<size=80%>"+ QSoundsConfig::NoteMissedSoundPath +"\n<size=100%>to continue.", false)->set_enableWordWrapping(true);
        }
        (void)closedir(sounddir);
    }

    void NoteMissedSdListViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        NoteMissedListView = this;
        if (firstActivation && addedToHierarchy)
        {
            UnityEngine::UI::VerticalLayoutGroup* container = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_rectTransform());
            container->set_spacing(0.4f);
            container->GetComponent<UnityEngine::UI::LayoutElement*>()->set_minWidth(125.0);

            // Bool settings
            this->QSconfigcontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(container->get_rectTransform());
            QSconfigcontainer->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
            QSconfigcontainer->set_childForceExpandHeight(false);
            QSconfigcontainer->set_childControlHeight(true);

            // Enable or Disable BadHitSounds
            //QSoundsConfig::QSAddConfigValueToggle(QSconfigcontainer->get_rectTransform(), "Custom BadHitSounds", &QSoundsConfig::Config.noteMissedSound_Active, this, "Activates or deactivates Custom BadHitSounds");
            auto object = ::QuestUI::BeatSaberUI::CreateToggle(QSconfigcontainer->get_rectTransform(), "Custom Note Missed Sounds", QSoundsConfig::Config.noteMissedSound_Active,
                [&](bool value) {
                    if (value) NoteMissedRefreshList();
                    QSoundsConfig::Config.noteMissedSound_Active = value;
                    this->SDlistscroll->get_gameObject()->SetActive(value);
                    if (AudioClips::noteMissedSoundLoader.audioSource != nullptr) AudioClips::noteMissedSoundLoader.audioSource->Stop();
                });
            ::QuestUI::BeatSaberUI::AddHoverHint(object->get_gameObject(), "Activates or deactivates Custom Note Missed Sounds");

            // AudioVolumeOffset Slider
            QuestUI::SliderSetting* volumeSlider = ::QuestUI::BeatSaberUI::CreateSliderSetting(QSconfigcontainer->get_rectTransform(), "Volume Offset", 0.01f, QSoundsConfig::Config.noteMissedSound_audioVolumeOffset, -1.0f, 1.0f, 0.5f, [](float volume) {
                // Checks for safety
                QSoundsConfig::Config.noteMissedSound_audioVolumeOffset = volume;
                if (AudioClips::noteMissedSoundLoader.loaded) {
                    if (AudioClips::noteMissedSoundLoader.audioSource->get_isPlaying()) AudioClips::noteMissedSoundLoader.audioSource->Stop();
                    AudioClips::noteMissedSoundLoader.audioSource->set_volume(0.5f + volume);
                    AudioClips::noteMissedSoundLoader.audioSource->Play();
                }
                //std::thread PlayAudio([&]() {
                //    usleep(100);
                //});
            });
            QuestUI::BeatSaberUI::AddHoverHint(volumeSlider->get_gameObject(), "Lets you select a Volume Offset that is applied to the sound");

            // Sound List (recursively adds buttons as ListView isn't an easy type to deal with)
            this->SDlistscroll = QuestUI::BeatSaberUI::CreateScrollView(container->get_rectTransform());
            SDlistscroll->GetComponent<QuestUI::ExternalComponents*>()->Get<UnityEngine::UI::LayoutElement*>()->set_minHeight(56.0);
            auto* SDlistcontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(SDlistscroll->get_transform());
            SDlistcontainer->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
            SDlistcontainer->set_childForceExpandHeight(false);
            SDlistcontainer->set_childControlHeight(true);

            this->SDlistscroll->get_gameObject()->SetActive(QSoundsConfig::Config.noteMissedSound_Active);
        }
        NoteMissedRefreshList();
    }

    void NoteMissedSdListViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling)
    {
        //QSoundsConfig::SaveConfig();
        for (UnityEngine::UI::Button* button : NoteMissedQSlist) UnityEngine::Object::Destroy(button->get_transform()->get_parent()->get_gameObject());
        NoteMissedQSlist = {};
    }
}
