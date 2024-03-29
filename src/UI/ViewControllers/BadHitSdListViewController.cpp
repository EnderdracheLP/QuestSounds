#include "main.hpp"
#include "QSoundsConfig.hpp"
#include "ViewControllers/BadHitSdListViewController.hpp"
#include "AudioClips.hpp"

#include <dirent.h>
#include <regex>
#include <list>

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"

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
DEFINE_TYPE(QuestSounds::ViewControllers, BadHitSdListViewController);
#elif defined(DEFINE_TYPE)
DEFINE_TYPE(QuestSounds::ViewControllers::BadHitSdListViewController);
#elif defined(DEFINE_CLASS)
DEFINE_CLASS(QuestSounds::ViewControllers::BadHitSdListViewController);
#endif

namespace QuestSounds::ViewControllers {

    BadHitSdListViewController* BadHitListView;
    std::list<UnityEngine::UI::Button*> BadHitQSlist = {};

    void BadHitSelectSound()
    {
        for (UnityEngine::UI::Button* button : BadHitQSlist)
        {
            if (button->get_hasSelection())
            {
                std::string filename = to_utf8(csstrtostr(button->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->get_text()));
                QSoundsConfig::Config.badHitSound_filepath = QSoundsConfig::BadHitSoundPath + filename;
                AudioClips::badHitSoundLoader.filePath = QSoundsConfig::Config.badHitSound_filepath;
                if (AudioClips::badHitSoundLoader.audioSource != nullptr) AudioClips::badHitSoundLoader.audioSource->Stop();
                AudioClips::badHitSoundLoader.load();
                std::thread PlayAudio([&]() {
                    while (!AudioClips::badHitSoundLoader.loaded && QSoundsConfig::Config.badHitSound_Active) {
                        usleep(100);
                    }
                    if (!QSoundsConfig::Config.badHitSound_Active) {
                        return;
                    }
                    if (AudioClips::badHitSoundLoader.audioSource != nullptr) {
                        AudioClips::badHitSoundLoader.audioSource->set_volume(0.6f + QSoundsConfig::Config.badHitSound_audioVolumeOffset);
                        return AudioClips::badHitSoundLoader.audioSource->Play();
                    }
                    });
                PlayAudio.detach();
                getLogger().debug("Selected Sound Path %s", QSoundsConfig::Config.badHitSound_filepath.c_str());
            }
        }
    }

    void BadHitRefreshList()
    {
        if (BadHitListView->listtxtgroup && BadHitListView->listtxtgroup->m_CachedPtr.m_value) UnityEngine::GameObject::Destroy(BadHitListView->listtxtgroup->get_gameObject());
        for (UnityEngine::UI::Button* button : BadHitQSlist) UnityEngine::Object::Destroy(button->get_transform()->get_parent()->get_gameObject());
        BadHitQSlist = {};
        DIR* sounddir = opendir(QSoundsConfig::BadHitSoundPath.c_str());
        dirent* fileent;
        while ((fileent = readdir(sounddir)) != NULL)
        {
            std::string filename = fileent->d_name;
            for (char& ch : filename) ch = tolower(ch);

            if (std::regex_search(filename, std::regex(".ogg|.mp3|.mp2|.wav|.aiff|.aif")))
            {
                UnityEngine::UI::HorizontalLayoutGroup* rowgroup = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(BadHitListView->SDlistscroll->get_transform());
                UnityEngine::UI::Button* button = QuestUI::BeatSaberUI::CreateUIButton(rowgroup->get_rectTransform(), fileent->d_name, BadHitSelectSound);
                button->get_gameObject()->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_fontStyle(2);
                BadHitQSlist.push_back(button);
            }
        }
        if (BadHitQSlist.size() < 1)
        {
            BadHitListView->listtxtgroup = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(BadHitListView->SDlistscroll->get_transform());
            QuestUI::BeatSaberUI::CreateText(BadHitListView->listtxtgroup->get_rectTransform(), "No sound files were found!\nPlease add a sound file into\n<size=80%>"+ QSoundsConfig::BadHitSoundPath +"\n<size=100%>to continue.", false)->set_enableWordWrapping(true);
        }
        (void)closedir(sounddir);
    }

    void BadHitSdListViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        BadHitListView = this;
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
            //QSoundsConfig::QSAddConfigValueToggle(QSconfigcontainer->get_rectTransform(), "Custom BadHitSounds", &QSoundsConfig::Config.badHitSound_Active, this, "Activates or deactivates Custom BadHitSounds");
            auto object = ::QuestUI::BeatSaberUI::CreateToggle(QSconfigcontainer->get_rectTransform(), "Custom Bad Hit Sounds", QSoundsConfig::Config.badHitSound_Active,
                [&](bool value) {
                    if (value) BadHitRefreshList();
                    QSoundsConfig::Config.badHitSound_Active = value;
                    this->SDlistscroll->get_gameObject()->SetActive(value);
                    if (AudioClips::badHitSoundLoader.audioSource != nullptr) AudioClips::badHitSoundLoader.audioSource->Stop();
                });
            ::QuestUI::BeatSaberUI::AddHoverHint(object->get_gameObject(), "Activates or deactivates Custom Bad Hit Sounds");

            // AudioVolumeOffset Slider
            QuestUI::SliderSetting* volumeSlider = ::QuestUI::BeatSaberUI::CreateSliderSetting(QSconfigcontainer->get_rectTransform(), "Volume Offset", 0.01f, QSoundsConfig::Config.badHitSound_audioVolumeOffset, -1.0f, 1.0f, 0.5f, [](float volume) {
                // Checks for safety
                QSoundsConfig::Config.badHitSound_audioVolumeOffset = volume;
                if (AudioClips::badHitSoundLoader.loaded) {
                    if (AudioClips::badHitSoundLoader.audioSource->get_isPlaying()) AudioClips::badHitSoundLoader.audioSource->Stop();
                    AudioClips::badHitSoundLoader.audioSource->set_volume(1.0f + volume);
                    AudioClips::badHitSoundLoader.audioSource->Play();
                }
                //std::thread PlayAudio([&]() {
                //    usleep(100);
                //});
            });
            QuestUI::BeatSaberUI::AddHoverHint(volumeSlider->get_gameObject(), "Lets you select a Volume Offset that is applied to the sound (Preview volume does not necessarily match volume played in-map)");

            // Sound List (recursively adds buttons as ListView isn't an easy type to deal with)
            this->SDlistscroll = QuestUI::BeatSaberUI::CreateScrollView(container->get_rectTransform());
            SDlistscroll->GetComponent<QuestUI::ExternalComponents*>()->Get<UnityEngine::UI::LayoutElement*>()->set_minHeight(56.0);
            auto* SDlistcontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(SDlistscroll->get_transform());
            SDlistcontainer->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
            SDlistcontainer->set_childForceExpandHeight(false);
            SDlistcontainer->set_childControlHeight(true);

            this->SDlistscroll->get_gameObject()->SetActive(QSoundsConfig::Config.badHitSound_Active);
        }
        BadHitRefreshList();
    }

    void BadHitSdListViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling)
    {
        //QSoundsConfig::SaveConfig();
        for (UnityEngine::UI::Button* button : BadHitQSlist) UnityEngine::Object::Destroy(button->get_transform()->get_parent()->get_gameObject());
        BadHitQSlist = {};
    }
}
