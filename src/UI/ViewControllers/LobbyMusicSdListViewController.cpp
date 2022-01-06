#include "main.hpp"
#include "QSoundsConfig.hpp"
#include "ViewControllers/LobbyMusicSdListViewController.hpp"
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

#include "GlobalNamespace/SongPreviewPlayer.hpp"

using namespace QuestSounds;

#ifndef REGISTER_FUNCTION
DEFINE_TYPE(QuestSounds::ViewControllers, LobbyMusicSdListViewController);
#elif defined(DEFINE_TYPE)
DEFINE_TYPE(QuestSounds::ViewControllers::LobbyMusicSdListViewController);
#elif defined(DEFINE_CLASS)
DEFINE_CLASS(QuestSounds::ViewControllers::LobbyMusicSdListViewController);
#endif

namespace QuestSounds::ViewControllers {

    LobbyMusicSdListViewController* LobbyMusicListView;
    std::list<UnityEngine::UI::Button*> LobbyMusicQSlist = {};

    void LobbyMusicSelectSound()
    {
        for (UnityEngine::UI::Button* button : LobbyMusicQSlist)
        {
            if (button->get_hasSelection())
            {
                std::string filename = to_utf8(csstrtostr(button->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->get_text()));
                QSoundsConfig::Config.lobbyAmbience_filepath = QSoundsConfig::LobbyMusicPath + filename;
                QuestSounds::AudioClips::lobbyAmbienceLoader.filePath = QSoundsConfig::Config.lobbyAmbience_filepath;
                if (AudioClips::lobbyAmbienceLoader.audioSource != nullptr) AudioClips::lobbyAmbienceLoader.audioSource->Stop();
                AudioClips::lobbyAmbienceLoader.load();
                std::thread PlayAudio([&]() {
                    while (!AudioClips::lobbyAmbienceLoader.loaded && QSoundsConfig::Config.lobbyAmbience_Active) {
                        usleep(100);
                    }
                    if (!QSoundsConfig::Config.lobbyAmbience_Active) {
                        return;
                    }
                    AudioClips::lobbyAmbienceLoader.audioSource->set_volume(0.6f);
                    return AudioClips::lobbyAmbienceLoader.audioSource->Play();
                    });
                PlayAudio.detach();
                getLogger().debug("Selected filename %s, Sound Path %s", filename.c_str(), QSoundsConfig::Config.lobbyAmbience_filepath.c_str());
            }
        }
    }

    void LobbyMusicRefreshList()
    {
        if (LobbyMusicListView->listtxtgroup && LobbyMusicListView->listtxtgroup->dyn_m_CachedPtr().m_value) UnityEngine::GameObject::Destroy(LobbyMusicListView->listtxtgroup->get_gameObject());
        for (UnityEngine::UI::Button* button : LobbyMusicQSlist) UnityEngine::Object::Destroy(button->get_transform()->get_parent()->get_gameObject());
        LobbyMusicQSlist = {};
        DIR* sounddir = opendir(QSoundsConfig::LobbyMusicPath.c_str());
        dirent* fileent;
        while ((fileent = readdir(sounddir)) != NULL)
        {
            std::string filename = fileent->d_name;
            for (char& ch : filename) ch = tolower(ch);

            if (std::regex_search(filename, std::regex(".ogg|.mp3|.mp2|.wav|.aiff|.aif")))
            {
                UnityEngine::UI::HorizontalLayoutGroup* rowgroup = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(LobbyMusicListView->SDlistscroll->get_transform());
                UnityEngine::UI::Button* button = QuestUI::BeatSaberUI::CreateUIButton(rowgroup->get_rectTransform(), fileent->d_name, LobbyMusicSelectSound);
                button->get_gameObject()->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_fontStyle(2);
                LobbyMusicQSlist.push_back(button);
            }
        }
        if (LobbyMusicQSlist.size() < 1)
        {
            LobbyMusicListView->listtxtgroup = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(LobbyMusicListView->SDlistscroll->get_transform());
            QuestUI::BeatSaberUI::CreateText(LobbyMusicListView->listtxtgroup->get_rectTransform(), "No sound files were found!\nPlease add a sound file into\n" + QSoundsConfig::LobbyMusicPath + "\nto continue.", false)->set_enableWordWrapping(true);
        }
        (void)closedir(sounddir);
    }

    void LobbyMusicSdListViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        LobbyMusicListView = this;
        if (firstActivation && addedToHierarchy)
        {
            //QuestSounds::ObjectInstances::SPP = UnityEngine::GameObject::FindObjectOfType<GlobalNamespace::SongPreviewPlayer*>();
            UnityEngine::UI::VerticalLayoutGroup* container = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_rectTransform());
            container->set_spacing(0.4f);
            container->GetComponent<UnityEngine::UI::LayoutElement*>()->set_minWidth(125.0);

            // Bool settings
            this->QSconfigcontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(container->get_rectTransform());
            QSconfigcontainer->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
            QSconfigcontainer->set_childForceExpandHeight(false);
            QSconfigcontainer->set_childControlHeight(true);

            QuestUI::BeatSaberUI::CreateText(QSconfigcontainer->get_rectTransform(), "Menu Music will be Paused here", false);

            // Enable or Disable LobbyMusic
            //QSoundsConfig::QSAddConfigValueToggle(QSconfigcontainer->get_rectTransform(), "Custom Lobby Music", &QSoundsConfig::Config.lobbyAmbience_Active, this, "Activates or deactivates Custom Lobby Music");
            auto object = ::QuestUI::BeatSaberUI::CreateToggle(QSconfigcontainer->get_rectTransform(), "Custom Lobby Music", QSoundsConfig::Config.lobbyAmbience_Active,
                [&](bool value) {
                    QSoundsConfig::Config.lobbyAmbience_Active = value;
                    this->SDlistscroll->get_gameObject()->SetActive(value);
                    if (AudioClips::lobbyAmbienceLoader.audioSource != nullptr) AudioClips::lobbyAmbienceLoader.audioSource->Stop();
                });
            ::QuestUI::BeatSaberUI::AddHoverHint(object->get_gameObject(), "Activates or deactivates Custom Lobby Music");


            // Sound List (recursively adds buttons as ListView isn't an easy type to deal with)
            this->SDlistscroll = QuestUI::BeatSaberUI::CreateScrollView(container->get_rectTransform());
            SDlistscroll->GetComponent<QuestUI::ExternalComponents*>()->Get<UnityEngine::UI::LayoutElement*>()->set_minHeight(56.0);
            auto* SDlistcontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(SDlistscroll->get_transform());
            SDlistcontainer->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
            SDlistcontainer->set_childForceExpandHeight(false);
            SDlistcontainer->set_childControlHeight(true);

            this->SDlistscroll->get_gameObject()->SetActive(QSoundsConfig::Config.lobbyAmbience_Active);
        }
        QuestSounds::ObjectInstances::SPP->PauseCurrentChannel();
        LobbyMusicRefreshList();
    }

    void LobbyMusicSdListViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling)
    {
        QuestSounds::ObjectInstances::SPP->UnPauseCurrentChannel();
        //QSoundsConfig::SaveConfig();
        for (UnityEngine::UI::Button* button : LobbyMusicQSlist) UnityEngine::Object::Destroy(button->get_transform()->get_parent()->get_gameObject());
        LobbyMusicQSlist = {};
        if (AudioClips::lobbyAmbienceLoader.audioSource != nullptr) AudioClips::lobbyAmbienceLoader.audioSource->Stop();
    }
}