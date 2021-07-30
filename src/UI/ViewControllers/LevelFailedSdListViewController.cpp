#include "main.hpp"
#include "QSoundsConfig.hpp"
#include "ViewControllers/LevelFailedSdListViewController.hpp"
#include "AudioClips.hpp"
#include "ObjectInstances.hpp"

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
DEFINE_TYPE(QuestSounds::ViewControllers, LevelFailedSdListViewController);
#elif defined(DEFINE_TYPE)
DEFINE_TYPE(QuestSounds::ViewControllers::LevelFailedSdListViewController);
#elif defined(DEFINE_CLASS)
DEFINE_CLASS(QuestSounds::ViewControllers::LevelFailedSdListViewController);
#endif

namespace QuestSounds::ViewControllers {

    LevelFailedSdListViewController* LevelFailedListView;
    std::list<UnityEngine::UI::Button*> LevelFailedQSlist = {};

    void SelectSound()
    {
        for (UnityEngine::UI::Button* button : LevelFailedQSlist)
        {
            if (button->hasSelection)
            {
                std::string filename = to_utf8(csstrtostr(button->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->get_text()));
                QSoundsConfig::Config.levelFailed_filepath = QSoundsConfig::LevelFailedPath + filename;
                QuestSounds::AudioClips::levelFailedLoader.filePath = QSoundsConfig::Config.levelFailed_filepath;
                if (AudioClips::levelFailedLoader.audioSource != nullptr) AudioClips::levelFailedLoader.audioSource->Stop();
                AudioClips::levelFailedLoader.load();
                std::thread PlayAudio([&]() {
                    while (!AudioClips::levelFailedLoader.loaded && QSoundsConfig::Config.levelFailed_Active) {
                        usleep(100);
                    }
                    if (!QSoundsConfig::Config.levelFailed_Active) {
                        return;
                    }
                    AudioClips::levelFailedLoader.audioSource->Stop();
                    AudioClips::levelFailedLoader.audioSource->set_volume(0.6f);
                    return AudioClips::levelFailedLoader.audioSource->Play();
                    });
                PlayAudio.detach();
                getLogger().debug("Selected filename %s, Sound Path %s", filename.c_str(), QSoundsConfig::Config.levelFailed_filepath.c_str());
            }
        }
    }

    void LevelFailedRefreshList()
    {
        if (LevelFailedListView->listtxtgroup && LevelFailedListView->listtxtgroup->m_CachedPtr.m_value) UnityEngine::GameObject::Destroy(LevelFailedListView->listtxtgroup->get_gameObject());
        for (UnityEngine::UI::Button* button : LevelFailedQSlist) UnityEngine::Object::Destroy(button->get_transform()->get_parent()->get_gameObject());
        LevelFailedQSlist = {};
        DIR* sounddir = opendir(QSoundsConfig::LevelFailedPath.c_str());
        dirent* fileent;
        while ((fileent = readdir(sounddir)) != NULL)
        {
            std::string filename = fileent->d_name;
            for (char& ch : filename) ch = tolower(ch);

            if (std::regex_search(filename, std::regex(".ogg|.mp3|.mp2|.wav|.aiff|.aif")))
            {
                UnityEngine::UI::HorizontalLayoutGroup* rowgroup = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(LevelFailedListView->SDlistscroll->get_transform());
                UnityEngine::UI::Button* button = QuestUI::BeatSaberUI::CreateUIButton(rowgroup->get_rectTransform(), fileent->d_name, SelectSound);
                button->get_gameObject()->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_fontStyle(2);
                LevelFailedQSlist.push_back(button);
            }
        }
        if (LevelFailedQSlist.size() < 1)
        {
            LevelFailedListView->listtxtgroup = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(LevelFailedListView->SDlistscroll->get_transform());
            QuestUI::BeatSaberUI::CreateText(LevelFailedListView->listtxtgroup->get_rectTransform(), "No sound files were found!\nPlease add a sound file into\n"+ QSoundsConfig::LevelFailedPath +"\nto continue.", false)->set_enableWordWrapping(true);
        }
        (void)closedir(sounddir);
    }

    void LevelFailedSdListViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        LevelFailedListView = this;
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

            // Enable or Disable LevelFailedSounds
            //QSoundsConfig::QSAddConfigValueToggle(QSconfigcontainer->get_rectTransform(), "Custom LevelFailedSounds", &QSoundsConfig::Config.levelFailed_Active, this, "Activates or deactivates Custom LevelFailedSounds");
            auto object = ::QuestUI::BeatSaberUI::CreateToggle(QSconfigcontainer->get_rectTransform(), "Custom LevelFailed Sounds", QSoundsConfig::Config.levelFailed_Active,
                [&](bool value) {
                    QSoundsConfig::Config.levelFailed_Active = value;
                    this->SDlistscroll->get_gameObject()->SetActive(value);
                    if (AudioClips::levelFailedLoader.audioSource != nullptr) AudioClips::levelFailedLoader.audioSource->Stop();
                });
            ::QuestUI::BeatSaberUI::AddHoverHint(object->get_gameObject(), "Activates or deactivates Custom LevelFailed Sounds");


            // Sound List (recursively adds buttons as ListView isn't an easy type to deal with)
            this->SDlistscroll = QuestUI::BeatSaberUI::CreateScrollView(container->get_rectTransform());
            SDlistscroll->GetComponent<QuestUI::ExternalComponents*>()->Get<UnityEngine::UI::LayoutElement*>()->set_minHeight(56.0);
            auto* SDlistcontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(SDlistscroll->get_transform());
            SDlistcontainer->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
            SDlistcontainer->set_childForceExpandHeight(false);
            SDlistcontainer->set_childControlHeight(true);

            this->SDlistscroll->get_gameObject()->SetActive(QSoundsConfig::Config.levelFailed_Active);
        }
        LevelFailedRefreshList();
    }

    void LevelFailedSdListViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling)
    {
        //QSoundsConfig::SaveConfig();
        for (UnityEngine::UI::Button* button : LevelFailedQSlist) UnityEngine::Object::Destroy(button->get_transform()->get_parent()->get_gameObject());
        LevelFailedQSlist = {};
        if (AudioClips::levelFailedLoader.audioSource != nullptr) AudioClips::levelFailedLoader.audioSource->Stop();

    }

}