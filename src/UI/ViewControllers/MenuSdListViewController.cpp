#include "main.hpp"
#include "QSoundsConfig.hpp"
#include "ViewControllers/MenuSdListViewController.hpp"
#include "AudioClips.hpp"

#include <dirent.h>
#include <regex>
#include <list>
#include <unistd.h>
#include <chrono>

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "questui/shared/CustomTypes/Components/MainThreadScheduler.hpp"

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

#include "GlobalNamespace/SongPreviewPlayer.hpp"

using namespace QuestSounds;

#ifndef REGISTER_FUNCTION
DEFINE_TYPE(QuestSounds::ViewControllers, MenuSdListViewController);
#elif defined(DEFINE_TYPE)
DEFINE_TYPE(QuestSounds::ViewControllers::MenuSdListViewController);
#elif defined(DEFINE_CLASS)
DEFINE_CLASS(QuestSounds::ViewControllers::MenuSdListViewController);
#endif

namespace QuestSounds::ObjectInstances {
    GlobalNamespace::SongPreviewPlayer* SPP;
}
using namespace QuestSounds::ObjectInstances;

namespace QuestSounds::ViewControllers {

    MenuSdListViewController* MenuListView;
    std::list<UnityEngine::UI::Button*> MenuQSlist = {};

    void MenuSelectSound()
    {
        for (UnityEngine::UI::Button* button : MenuQSlist)
        {
            if (button->get_hasSelection())
            {
                std::string filename = to_utf8(csstrtostr(button->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->get_text()));
                QSoundsConfig::Config.menuMusic_filepath = QSoundsConfig::MenuMusicPath + filename;
                QuestSounds::AudioClips::menuMusicLoader.filePath = QSoundsConfig::Config.menuMusic_filepath;
                AudioClips::menuMusicLoader.loaded = false;
                AudioClips::menuMusicLoader.load();
                std::thread replaceAudio([&]() { 
                    while (!AudioClips::menuMusicLoader.loaded && QSoundsConfig::Config.menuMusic_Active) {
                        usleep(100);
                    }
                    if (!QSoundsConfig::Config.menuMusic_Active) { 
                        return; 
                    } 
                    if (AudioClips::menuMusicLoader.audioSource != nullptr) {
                        return SPP->CrossfadeToNewDefault(AudioClips::menuMusicLoader.getClip());
                    }
                    });
                replaceAudio.detach();
                getLogger().debug("Selected Sound Path %s", QSoundsConfig::Config.menuMusic_filepath.c_str());

            }
        }
    }

    void MenuRefreshList()
    {
        if (MenuListView->listtxtgroup && MenuListView->listtxtgroup->m_CachedPtr.m_value) UnityEngine::GameObject::Destroy(MenuListView->listtxtgroup->get_gameObject());
        for (UnityEngine::UI::Button* button : MenuQSlist) UnityEngine::Object::Destroy(button->get_transform()->get_parent()->get_gameObject());
        MenuQSlist = {};
        DIR* sounddir = opendir(QSoundsConfig::MenuMusicPath.c_str());
        dirent* fileent;
        while ((fileent = readdir(sounddir)) != NULL)
        {
            std::string filename = fileent->d_name;
            for (char& ch : filename) ch = tolower(ch);

            if (std::regex_search(filename, std::regex(".ogg|.mp3|.mp2|.wav|.aiff|.aif")))
            {
                UnityEngine::UI::HorizontalLayoutGroup* rowgroup = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(MenuListView->SDlistscroll->get_transform());
                UnityEngine::UI::Button* button = QuestUI::BeatSaberUI::CreateUIButton(rowgroup->get_rectTransform(), fileent->d_name, MenuSelectSound);
                button->get_gameObject()->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_fontStyle(2);
                MenuQSlist.push_back(button);
            }
        }
        if (MenuQSlist.size() < 1)
        {
            MenuListView->listtxtgroup = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(MenuListView->SDlistscroll->get_transform());
            QuestUI::BeatSaberUI::CreateText(MenuListView->listtxtgroup->get_rectTransform(), "No sound files were found!\nPlease add a sound file into\n<size=80%>"+ QSoundsConfig::MenuMusicPath +"\n<size=100%>to continue.", false)->set_enableWordWrapping(true);
        }
        (void)closedir(sounddir);
    }

    void MenuSdListViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        MenuListView = this;
        if (firstActivation && addedToHierarchy)
        {
            SPP = UnityEngine::GameObject::FindObjectOfType<GlobalNamespace::SongPreviewPlayer*>();
            UnityEngine::UI::VerticalLayoutGroup* container = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_rectTransform());
            container->set_spacing(0.4f);
            container->GetComponent<UnityEngine::UI::LayoutElement*>()->set_minWidth(125.0);


             //Bool settings
            this->QSconfigcontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(container->get_rectTransform());
            QSconfigcontainer->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
            QSconfigcontainer->set_childForceExpandHeight(false);
            QSconfigcontainer->set_childControlHeight(true);

            // Enable or Disable Custom Menu Music
            //QSoundsConfig::QSAddConfigValueToggle(QSconfigcontainer->get_rectTransform(), "Custom Menu Music", &QSoundsConfig::Config.menuMusic_Active, SDlistscroll, "Activates or deactivates Custom Menu Music");
            auto object = ::QuestUI::BeatSaberUI::CreateToggle(QSconfigcontainer->get_rectTransform(), "Custom Menu Music", QSoundsConfig::Config.menuMusic_Active,
                [&](bool value) {
                    if (value) MenuRefreshList();
                    QSoundsConfig::Config.menuMusic_Active = value;
                    this->SDlistscroll->get_gameObject()->SetActive(value);
                    if (AudioClips::menuMusicLoader.loaded && value) {
                        SPP->CrossfadeToNewDefault(AudioClips::menuMusicLoader.getClip());
                    }
                    else {
                        if (AudioClips::menuMusicLoader.OriginalAudioSource) SPP->CrossfadeToNewDefault(AudioClips::menuMusicLoader.get_OriginalClip());
                    }
                });
            ::QuestUI::BeatSaberUI::AddHoverHint(object->get_gameObject(), "Activates or deactivates Custom Menu Music");


            //if (QSoundsConfig::LegacyConfig) QuestUI::BeatSaberUI::CreateText(settingsLayoutTransform, "Legacy Config Detected,\nplease move your files into the new folders,\nfor selection in-game", false);
            if (QSoundsConfig::LegacyConfig) {
                modal = QuestUI::BeatSaberUI::CreateModal(get_transform(), {40, 35}, nullptr);

                auto wrapper = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(modal->get_transform());
                auto container = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(wrapper->get_transform());
                container->set_childAlignment(UnityEngine::TextAnchor::MiddleCenter);

                QuestUI::BeatSaberUI::CreateText(container->get_transform(), "<line-height=130%><b>Legacy Config Detected</b>\n<line-height=80%><size=80%>please move your files\ninto the new folders,\nfor selection in-game\n\n", false)->set_alignment(TMPro::TextAlignmentOptions::Center);

                auto horizon = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(container->get_transform());

                QuestUI::BeatSaberUI::CreateUIButton(horizon->get_transform(), "<size=80%>Close", UnityEngine::Vector2(0, -3), { 10, 7 }, [this]() -> void {
                    modal->Hide(true, nullptr);
                    });

                std::thread showModal([&]() {
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));

                    QuestUI::MainThreadScheduler::Schedule([this]() {
                        modal->Show(true, true, nullptr);
                        });
                    });
                showModal.detach();
            }


            //QSconfigcontainer->set_childForceExpandHeight(false);
            //QSconfigcontainer->set_childControlHeight(true);

            //UnityEngine::UI::Button::Button();

            //bool enabled_initval = getConfig().config["enabled"].GetBool();
            //this->masterEnabled = QuestUI::BeatSaberUI::CreateToggle(QSconfigcontainer->get_rectTransform(), "Enable Quest Sounds", enabled_initval, UnityEngine::Vector2(0, 0), OnChangeEnabled);

            // Sound List (recursively adds buttons as MenuListView isn't an easy type to deal with)
            this->SDlistscroll = QuestUI::BeatSaberUI::CreateScrollView(container->get_rectTransform());
            SDlistscroll->GetComponent<QuestUI::ExternalComponents*>()->Get<UnityEngine::UI::LayoutElement*>()->set_minHeight(56.0);
            auto* SDlistcontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(SDlistscroll->get_transform());
            SDlistcontainer->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
            SDlistcontainer->set_childForceExpandHeight(false);
            SDlistcontainer->set_childControlHeight(true);

            //this->SDlistscroll->get_gameObject()->SetActive(enabled_initval);
            this->SDlistscroll->get_gameObject()->SetActive(QSoundsConfig::Config.menuMusic_Active);
        }
        MenuRefreshList();
    }

    void MenuSdListViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling)
    {
        for (UnityEngine::UI::Button* button : MenuQSlist) UnityEngine::Object::Destroy(button->get_transform()->get_parent()->get_gameObject());
        MenuQSlist = {};
        //if (AudioClips::menuMusicLoader.loaded && QSoundsConfig::Config.menuMusic_Active) {
        //    getLogger().debug("Returning custom audioClip");
        //    SPP->CrossfadeToNewDefault(AudioClips::menuMusicLoader.getClip());
        //}
        //else {
        //    getLogger().debug("Returning default audioClip");
        //    SPP->CrossfadeToNewDefault(AudioClips::menuMusicLoader.get_OriginalClip());
        //}
    }
}
