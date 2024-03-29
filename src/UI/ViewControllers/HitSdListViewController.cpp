#include "main.hpp"
#include "QSoundsConfig.hpp"
#include "ViewControllers/HitSdListViewController.hpp"
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
#include "UnityEngine/AudioSource.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "TMPro/TextMeshProUGUI.hpp"

using namespace QuestSounds;

#ifndef REGISTER_FUNCTION
DEFINE_TYPE(QuestSounds::ViewControllers, HitSdListViewController);
#elif defined(DEFINE_TYPE)
DEFINE_TYPE(QuestSounds::ViewControllers::HitSdListViewController);
#elif defined(DEFINE_CLASS)
DEFINE_CLASS(QuestSounds::ViewControllers::HitSdListViewController);
#endif
namespace QuestSounds::ViewControllers {

    HitSdListViewController* HitListView;
    std::list<UnityEngine::UI::Button*> HitQSlist = {};

    void HitSelectSound()
    {
        for (UnityEngine::UI::Button* button : HitQSlist)
        {
            if (button->get_hasSelection())
            {
                std::string filename = to_utf8(csstrtostr(button->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->get_text()));
                QSoundsConfig::Config.hitSound_filepath = QSoundsConfig::HitSoundPath + filename;
                AudioClips::hitSoundLoader.filePath = QSoundsConfig::Config.hitSound_filepath;
                if (AudioClips::hitSoundLoader.audioSource != nullptr) AudioClips::hitSoundLoader.audioSource->Stop();
                AudioClips::hitSoundLoader.load();
                std::thread PlayAudio([&]() {
                    while (!AudioClips::hitSoundLoader.loaded && QSoundsConfig::Config.hitSound_Active) {
                        usleep(100);
                    }
                    if (!QSoundsConfig::Config.hitSound_Active) {
                        return;
                    }
                    if (AudioClips::hitSoundLoader.audioSource != nullptr) {
                        AudioClips::hitSoundLoader.audioSource->set_volume(0.6f + QSoundsConfig::Config.hitSound_audioVolumeOffset);
                        return AudioClips::hitSoundLoader.audioSource->Play();
                    }
                    });
                PlayAudio.detach();
                getLogger().debug("Selected filename %s, Sound Path %s", filename.c_str(), QSoundsConfig::Config.hitSound_filepath.c_str());
            }
        }
    }

    void HitRefreshList()
    {
        if (HitListView->listtxtgroup && HitListView->listtxtgroup->m_CachedPtr.m_value) UnityEngine::GameObject::Destroy(HitListView->listtxtgroup->get_gameObject());
        for (UnityEngine::UI::Button* button : HitQSlist) UnityEngine::Object::Destroy(button->get_transform()->get_parent()->get_gameObject());
        HitQSlist = {};
        DIR* sounddir = opendir(QSoundsConfig::HitSoundPath.c_str());
        dirent* fileent;
        while ((fileent = readdir(sounddir)) != NULL)
        {
            std::string filename = fileent->d_name;
            for (char& ch : filename) ch = tolower(ch);

            if (std::regex_search(filename, std::regex(".ogg|.mp3|.mp2|.wav|.aiff|.aif")))
            {
                UnityEngine::UI::HorizontalLayoutGroup* rowgroup = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(HitListView->SDlistscroll->get_transform());
                UnityEngine::UI::Button* button = QuestUI::BeatSaberUI::CreateUIButton(rowgroup->get_rectTransform(), fileent->d_name, HitSelectSound);
                button->get_gameObject()->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_fontStyle(2);
                HitQSlist.push_back(button);
            }
        }
        if (HitQSlist.size() < 1)
        {
            HitListView->listtxtgroup = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(HitListView->SDlistscroll->get_transform());
            QuestUI::BeatSaberUI::CreateText(HitListView->listtxtgroup->get_rectTransform(), "No sound files were found!\nPlease add a sound file into\n<size=80%>" + QSoundsConfig::HitSoundPath + "\n<size=100%>to continue.", false)->set_enableWordWrapping(true);
        }
        (void)closedir(sounddir);
    }

    void HitSdListViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        HitListView = this;
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

            // Enable or Disable HitSounds
            //QSoundsConfig::QSAddConfigValueToggle(QSconfigcontainer->get_rectTransform(), "Custom HitSounds", &QSoundsConfig::Config.hitSound_Active, this, "Activates or deactivates Custom HitSounds");
            auto HSToggle = ::QuestUI::BeatSaberUI::CreateToggle(QSconfigcontainer->get_rectTransform(), "Custom Hit Sounds", QSoundsConfig::Config.hitSound_Active,
                [&](bool value) {
                    if (value) HitRefreshList();
                    QSoundsConfig::Config.hitSound_Active = value;
                    this->SDlistscroll->get_gameObject()->SetActive(value);
                    if (AudioClips::hitSoundLoader.audioSource != nullptr) AudioClips::hitSoundLoader.audioSource->Stop();
                });
            QuestUI::BeatSaberUI::AddHoverHint(HSToggle->get_gameObject(), "Activates or deactivates Custom Hit Sounds");

            QuestUI::SliderSetting* volumeSlider = ::QuestUI::BeatSaberUI::CreateSliderSetting(QSconfigcontainer->get_rectTransform(), "Volume Offset", 0.01f, QSoundsConfig::Config.hitSound_audioVolumeOffset, -1.0f, 1.0f, 0.5f, [](float volume) {
                // Checks for safety
                QSoundsConfig::Config.hitSound_audioVolumeOffset = volume;
                if (AudioClips::hitSoundLoader.loaded) {
                    if (AudioClips::hitSoundLoader.audioSource->get_isPlaying()) AudioClips::hitSoundLoader.audioSource->Stop();
                    AudioClips::hitSoundLoader.audioSource->set_volume(0.6f + volume);
                    AudioClips::hitSoundLoader.audioSource->Play();
                }
            });
            QuestUI::BeatSaberUI::AddHoverHint(volumeSlider->get_gameObject(), "Lets you select a Volume Offset that is applied to the sound (Preview volume does not necessarily match volume played in-map)");

            auto HSOffSet = ::QuestUI::BeatSaberUI::CreateIncrementSetting(QSconfigcontainer->get_rectTransform(), "Hit Sound OffSet (ms)", 3, 0.005f, QSoundsConfig::Config.hitSound_beatOffSet, 0.0f, 0.25f,
                [&](float value) {
                    QSoundsConfig::Config.hitSound_beatOffSet = value;
                });
            QuestUI::BeatSaberUI::AddHoverHint(HSOffSet->get_gameObject(), "Sets a custom HitSound beatOffSet (default: 0.185 ms)");


            // Sound List (recursively adds buttons as ListView isn't an easy type to deal with)
            this->SDlistscroll = QuestUI::BeatSaberUI::CreateScrollView(container->get_rectTransform());
            SDlistscroll->GetComponent<QuestUI::ExternalComponents*>()->Get<UnityEngine::UI::LayoutElement*>()->set_minHeight(56.0);
            auto* SDlistcontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(SDlistscroll->get_transform());
            SDlistcontainer->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
            SDlistcontainer->set_childForceExpandHeight(false);
            SDlistcontainer->set_childControlHeight(true);

            this->SDlistscroll->get_gameObject()->SetActive(QSoundsConfig::Config.hitSound_Active);
        }
        HitRefreshList();
    }

    void HitSdListViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling)
    {
        //QSoundsConfig::SaveConfig();
        for (UnityEngine::UI::Button* button : HitQSlist) UnityEngine::Object::Destroy(button->get_transform()->get_parent()->get_gameObject());
        HitQSlist = {};
        if (AudioClips::hitSoundLoader.audioSource != nullptr) AudioClips::hitSoundLoader.audioSource->Stop();
    }
}