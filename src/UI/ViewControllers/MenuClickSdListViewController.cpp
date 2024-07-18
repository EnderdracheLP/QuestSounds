#include "main.hpp"
#include "QSoundsConfig.hpp"
#include "ViewControllers/MenuClickSdListViewController.hpp"
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

#include "GlobalNamespace/BasicUIAudioManager.hpp"
#include "GlobalNamespace/RandomObjectPicker_1.hpp"

#ifndef REGISTER_FUNCTION
DEFINE_TYPE(QuestSounds::ViewControllers, MenuClickSdListViewController);
#elif defined(DEFINE_TYPE)
DEFINE_TYPE(QuestSounds::ViewControllers::MenuClickSdListViewController);
#elif defined(DEFINE_CLASS)
DEFINE_CLASS(QuestSounds::ViewControllers::MenuClickSdListViewController);
#endif

namespace QuestSounds::ViewControllers {

    GlobalNamespace::BasicUIAudioManager* BUIAM;

    MenuClickSdListViewController* MenuClickListView;
    std::list<UnityEngine::UI::Button*> MenuClickQSlist = {};

    void MenuClickSelectSound()
    {
        for (UnityEngine::UI::Button* button : MenuClickQSlist)
        {
            if (button->get_hasSelection())
            {
                std::string filename = button->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->get_text();
                QSoundsConfig::Config.menuClick_filepath = QSoundsConfig::MenuClickPath + filename;
                AudioClips::menuClickLoader.filePath = QSoundsConfig::Config.menuClick_filepath;
                if (AudioClips::menuClickLoader.audioSource != nullptr) AudioClips::menuClickLoader.audioSource->Stop();
                AudioClips::menuClickLoader.load();
                std::thread PlayAudio([&]() {
                    while (!AudioClips::menuClickLoader.loaded && QSoundsConfig::Config.menuClick_Active) {
                        usleep(100);
                    }
                    if (!QSoundsConfig::Config.menuClick_Active) {
                        return;
                    }
                    AudioClips::menuClickLoader.audioSource->set_volume(0.6f);
                    return AudioClips::menuClickLoader.audioSource->Play();
                    });
                PlayAudio.detach();
                getLogger().debug("Selected filename %s, Sound Path %s", filename.c_str(), QSoundsConfig::Config.menuClick_filepath.c_str());
            }
        }
    }

    void MenuClickRefreshList()
    {
        if (MenuClickListView->listtxtgroup && MenuClickListView->listtxtgroup->m_CachedPtr.m_value) UnityEngine::GameObject::Destroy(MenuClickListView->listtxtgroup->get_gameObject());
        for (UnityEngine::UI::Button* button : MenuClickQSlist) UnityEngine::Object::Destroy(button->get_transform()->get_parent()->get_gameObject());
        MenuClickQSlist = {};
        DIR* sounddir = opendir(QSoundsConfig::MenuClickPath.c_str());
        dirent* fileent;
        while ((fileent = readdir(sounddir)) != NULL)
        {
            std::string filename = fileent->d_name;
            for (char& ch : filename) ch = tolower(ch);

            if (std::regex_search(filename, std::regex(".ogg|.mp3|.mp2|.wav|.aiff|.aif")))
            {
                UnityEngine::UI::HorizontalLayoutGroup* rowgroup = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(MenuClickListView->SDlistscroll->get_transform());
                UnityEngine::UI::Button* button = QuestUI::BeatSaberUI::CreateUIButton(rowgroup->get_rectTransform(), fileent->d_name, MenuClickSelectSound);
                button->get_gameObject()->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_fontStyle(2);
                MenuClickQSlist.push_back(button);
            }
        }
        if (MenuClickQSlist.size() < 1)
        {
            MenuClickListView->listtxtgroup = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(MenuClickListView->SDlistscroll->get_transform());
            QuestUI::BeatSaberUI::CreateText(MenuClickListView->listtxtgroup->get_rectTransform(), "No sound files were found!\nPlease add a sound file into\n<size=80%>" + QSoundsConfig::MenuClickPath + "\n<size=100%>to continue.", false)->set_enableWordWrapping(true);
        }
        (void)closedir(sounddir);
    }

    void MenuClickSdListViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        MenuClickListView = this;
        if (firstActivation && addedToHierarchy)
        {
            BUIAM = UnityEngine::GameObject::FindObjectOfType<GlobalNamespace::BasicUIAudioManager*>();
            UnityEngine::UI::VerticalLayoutGroup* container = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_rectTransform());
            container->set_spacing(0.4f);
            container->GetComponent<UnityEngine::UI::LayoutElement*>()->set_minWidth(125.0);

            // Bool settings
            this->QSconfigcontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(container->get_rectTransform());
            QSconfigcontainer->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
            QSconfigcontainer->set_childForceExpandHeight(false);
            QSconfigcontainer->set_childControlHeight(true);

            // Enable or Disable MenuClickSounds
            //QSoundsConfig::QSAddConfigValueToggle(QSconfigcontainer->get_rectTransform(), "Custom MenuClickSounds", &QSoundsConfig::Config.menuClick_Active, SDlistscroll, "Activates or deactivates Custom MenuClickSounds");
            auto object = ::QuestUI::BeatSaberUI::CreateToggle(QSconfigcontainer->get_rectTransform(), "Custom Menu ClickSounds", QSoundsConfig::Config.menuClick_Active,
                [&](bool value) {
                    if (value) MenuClickRefreshList();
                    QSoundsConfig::Config.menuClick_Active = value;
                    this->SDlistscroll->get_gameObject()->SetActive(value);
                    if (AudioClips::menuClickLoader.audioSource) AudioClips::menuClickLoader.audioSource->Stop();
                    if (value && AudioClips::menuClickLoader.loaded) {
                        AudioClips::menuClickArr = AudioClips::createAudioClipArray(AudioClips::menuClickLoader);
                        BUIAM->randomSoundPicker->objects = AudioClips::menuClickArr;
                    }
                    else {
                        AudioClips::origMenuClickArr = AudioClips::createAudioClipArray(AudioClips::menuClickLoader, true);
                        BUIAM->randomSoundPicker->objects = AudioClips::origMenuClickArr;
                    }
                });
            ::QuestUI::BeatSaberUI::AddHoverHint(object->get_gameObject(), "Activates or deactivates Custom Menu ClickSounds");


            // Sound List (recursively adds buttons as ListView isn't an easy type to deal with)
            this->SDlistscroll = QuestUI::BeatSaberUI::CreateScrollView(container->get_rectTransform());
            SDlistscroll->GetComponent<QuestUI::ExternalComponents*>()->Get<UnityEngine::UI::LayoutElement*>()->set_minHeight(56.0);
            auto* SDlistcontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(SDlistscroll->get_transform());
            SDlistcontainer->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
            SDlistcontainer->set_childForceExpandHeight(false);
            SDlistcontainer->set_childControlHeight(true);

            this->SDlistscroll->get_gameObject()->SetActive(QSoundsConfig::Config.menuClick_Active);
        }
        MenuClickRefreshList();
    }

    void MenuClickSdListViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling)
    {
        //QSoundsConfig::SaveConfig();
        for (UnityEngine::UI::Button* button : MenuClickQSlist) UnityEngine::Object::Destroy(button->get_transform()->get_parent()->get_gameObject());
        MenuClickQSlist = {};
        if (QSoundsConfig::Config.menuClick_Active && AudioClips::menuClickLoader.loaded) {
            AudioClips::menuClickArr = AudioClips::createAudioClipArray(AudioClips::menuClickLoader);
            BUIAM->randomSoundPicker->objects = AudioClips::menuClickArr;
        }
        else {
            AudioClips::origMenuClickArr = AudioClips::createAudioClipArray(AudioClips::menuClickLoader, true);
            BUIAM->randomSoundPicker->objects = AudioClips::origMenuClickArr;
        }
    }
}