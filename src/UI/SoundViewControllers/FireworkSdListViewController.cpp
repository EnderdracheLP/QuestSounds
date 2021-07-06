#include "main.hpp"
#include "QSoundsConfig.hpp"
#include "ViewControllers/FireworkSdListViewController.hpp"
#include "AudioClips.hpp"
//#include "ObjectInstances.hpp"

#include <dirent.h>
#include <regex>
#include <list>

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"

#include "UnityEngine/Object.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
//#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "TMPro/TextMeshProUGUI.hpp"

#include "GlobalNamespace/SongPreviewPlayer.hpp"

#include "System/Action.hpp"
#include "System/Threading/Tasks/Task_1.hpp"
#include "System/Threading/ThreadStart.hpp"


#include "GlobalNamespace/HMTask.hpp"

using namespace QuestSounds;

#ifndef REGISTER_FUNCTION
DEFINE_TYPE(QuestSounds, FireworkSdListViewController);
#elif defined(DEFINE_TYPE)
DEFINE_TYPE(QuestSounds::FireworkSdListViewController);
#elif defined(DEFINE_CLASS)
DEFINE_CLASS(QuestSounds::FireworkSdListViewController);
#endif

FireworkSdListViewController* FireworkListView;
std::list<UnityEngine::UI::Button*> FireworkQSlist = {};

void FireworkSelectSound()
{
    for (UnityEngine::UI::Button* button : FireworkQSlist)
    {
        if (button->hasSelection)
        {
            std::string filename = to_utf8(csstrtostr(button->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->get_text()));
            QSoundsConfig::Config.firework_filepath = QSoundsConfig::FireworkSoundPath + filename;
            QuestSounds::AudioClips::fireworkSoundLoader.filePath = QSoundsConfig::Config.firework_filepath;
            AudioClips::fireworkSoundLoader.loaded = false;
            AudioClips::fireworkSoundLoader.load();
            std::thread PlayAudio([&]() {
                while (!AudioClips::fireworkSoundLoader.loaded && QSoundsConfig::Config.firework_Active) {
                    usleep(100);
                }
                if (!QSoundsConfig::Config.firework_Active) {
                    return;
                }
                return AudioClips::fireworkSoundLoader.audioSource->PlayOneShot(AudioClips::fireworkSoundLoader.getClip(), 0.8f);
                });
            PlayAudio.detach();
            getLogger().debug("Selected filename %s, Sound Path %s", filename.c_str(), QSoundsConfig::Config.firework_filepath.c_str());
        }
    }
}

void FireworkRefreshList()
{
    if (FireworkListView->listtxtgroup && FireworkListView->listtxtgroup->m_CachedPtr.m_value) UnityEngine::GameObject::Destroy(FireworkListView->listtxtgroup->get_gameObject());
    for (UnityEngine::UI::Button* button : FireworkQSlist) UnityEngine::Object::Destroy(button->get_transform()->get_parent()->get_gameObject());
    FireworkQSlist = {};
    DIR* sounddir = opendir(QSoundsConfig::FireworkSoundPath.c_str());
    dirent* fileent;
    while ((fileent = readdir(sounddir)) != NULL)
    {
        std::string filename = fileent->d_name;
        for (char& ch : filename) ch = tolower(ch);

        if (std::regex_search(filename, std::regex(".ogg|.mp3|.mp2|.wav|.aiff|.aif")))
        {
            UnityEngine::UI::HorizontalLayoutGroup* rowgroup = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(FireworkListView->SDlistscroll->get_transform());
            UnityEngine::UI::Button* button = QuestUI::BeatSaberUI::CreateUIButton(rowgroup->get_rectTransform(), fileent->d_name, FireworkSelectSound);
            button->get_gameObject()->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_fontStyle(2);
            FireworkQSlist.push_back(button);
        }
    }
    if (FireworkQSlist.size() < 1)
    {
        FireworkListView->listtxtgroup = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(FireworkListView->SDlistscroll->get_transform());
        QuestUI::BeatSaberUI::CreateText(FireworkListView->listtxtgroup->get_rectTransform(), "No sound files were found!\nPlease add a sound file into\n"+ QSoundsConfig::FireworkSoundPath +"\nto continue.", false);
    }
    (void)closedir(sounddir);
}

void FireworkSdListViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    FireworkListView = this;
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

        // Enable or Disable FireworkSounds
        //QSoundsConfig::QSAddConfigValueToggle(QSconfigcontainer->get_rectTransform(), "Custom FireworkSounds", &QSoundsConfig::Config.firework_Active, this, "Activates or deactivates Custom FireworkSounds");
        auto object = ::QuestUI::BeatSaberUI::CreateToggle(QSconfigcontainer->get_rectTransform(), "Custom Firework Sounds", QSoundsConfig::Config.firework_Active,
            [&](bool value) {
                QSoundsConfig::Config.firework_Active = value;
                this->SDlistscroll->get_gameObject()->SetActive(value);
            });
        ::QuestUI::BeatSaberUI::AddHoverHint(object->get_gameObject(), "Activates or deactivates Custom Firework Sounds");


        // Sound List (recursively adds buttons as ListView isn't an easy type to deal with)
        this->SDlistscroll = QuestUI::BeatSaberUI::CreateScrollView(container->get_rectTransform());
        SDlistscroll->GetComponent<QuestUI::ExternalComponents*>()->Get<UnityEngine::UI::LayoutElement*>()->set_minHeight(56.0);
        auto* SDlistcontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(SDlistscroll->get_transform());
        SDlistcontainer->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
        SDlistcontainer->set_childForceExpandHeight(false);
        SDlistcontainer->set_childControlHeight(true);

        this->SDlistscroll->get_gameObject()->SetActive(QSoundsConfig::Config.firework_Active);
    }
    FireworkRefreshList();
}

void FireworkSdListViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling)
{
    //QSoundsConfig::SaveConfig();
    for (UnityEngine::UI::Button* button : FireworkQSlist) UnityEngine::Object::Destroy(button->get_transform()->get_parent()->get_gameObject());
    FireworkQSlist = {};
}