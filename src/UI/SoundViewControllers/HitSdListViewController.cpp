#include "main.hpp"
#include "QSoundsConfig.hpp"
#include "ViewControllers/HitSdListViewController.hpp"
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
DEFINE_TYPE(QuestSounds, HitSdListViewController);
#elif defined(DEFINE_TYPE)
DEFINE_TYPE(QuestSounds::HitSdListViewController);
#elif defined(DEFINE_CLASS)
DEFINE_CLASS(QuestSounds::HitSdListViewController);
#endif

HitSdListViewController* HitListView;
std::list<UnityEngine::UI::Button*> HitQSlist = {};

void HitSelectSound()
{
    for (UnityEngine::UI::Button* button : HitQSlist)
    {
        if (button->hasSelection)
        {
            std::string filename = to_utf8(csstrtostr(button->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->get_text()));
            QSoundsConfig::Config.hitSound_filepath = QSoundsConfig::HitSoundPath + filename;
            AudioClips::hitSoundLoader.filePath = QSoundsConfig::Config.hitSound_filepath;
            AudioClips::hitSoundLoader.loaded = false;
            AudioClips::hitSoundLoader.load();
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

        if (std::regex_search(filename, std::regex(".ogg|.mp3|.wav|.aiff|.aif")))
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
        QuestUI::BeatSaberUI::CreateText(HitListView->listtxtgroup->get_rectTransform(), "No sound files were found!\nPlease add a sound file into\n"+ QSoundsConfig::HitSoundPath +"\nto continue.", false);
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
        QSoundsConfig::QSAddConfigValueToggle(QSconfigcontainer->get_rectTransform(), "Custom HitSounds", &QSoundsConfig::Config.hitSound_Active, SDlistscroll, "Activates or deactivates Custom HitSounds");


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
}