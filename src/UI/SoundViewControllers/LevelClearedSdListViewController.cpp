#include "main.hpp"
#include "QSoundsConfig.hpp"
#include "ViewControllers/LevelClearedSdListViewController.hpp"
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
DEFINE_TYPE(QuestSounds, QSoundsLevelClearedSdListViewController);
#elif defined(DEFINE_TYPE)
DEFINE_TYPE(QuestSounds::LevelClearedSdListViewController);
#elif defined(DEFINE_CLASS)
DEFINE_CLASS(QuestSounds::QSoundsLevelClearedSdListViewController);
#endif

LevelClearedSdListViewController* LevelClearedListView;
std::list<UnityEngine::UI::Button*> LevelClearedQSlist = {};

void LevelClearedSelectSound()
{
    for (UnityEngine::UI::Button* button : LevelClearedQSlist)
    {
        if (button->hasSelection)
        {
            std::string filename = to_utf8(csstrtostr(button->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->get_text()));
            QSoundsConfig::Config.levelCleared_filepath = QSoundsConfig::LevelClearedPath + filename;
            QuestSounds::AudioClips::levelClearedLoader.filePath = QSoundsConfig::Config.levelCleared_filepath;
            AudioClips::levelClearedLoader.loaded = false;
            AudioClips::levelClearedLoader.load();
            getLogger().debug("Selected filename %s, Sound Path %s", filename.c_str(), QSoundsConfig::Config.levelCleared_filepath.c_str());
        }
    }
}

void LevelClearedRefreshList()
{
    if (LevelClearedListView->listtxtgroup && LevelClearedListView->listtxtgroup->m_CachedPtr.m_value) UnityEngine::GameObject::Destroy(LevelClearedListView->listtxtgroup->get_gameObject());
    for (UnityEngine::UI::Button* button : LevelClearedQSlist) UnityEngine::Object::Destroy(button->get_transform()->get_parent()->get_gameObject());
    LevelClearedQSlist = {};
    DIR* sounddir = opendir(QSoundsConfig::LevelClearedPath.c_str());
    dirent* fileent;
    while ((fileent = readdir(sounddir)) != NULL)
    {
        std::string filename = fileent->d_name;
        for (char& ch : filename) ch = tolower(ch);

        if (std::regex_search(filename, std::regex(".ogg|.mp3|.wav|.aiff|.aif")))
        {
            UnityEngine::UI::HorizontalLayoutGroup* rowgroup = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(LevelClearedListView->SDlistscroll->get_transform());
            UnityEngine::UI::Button* button = QuestUI::BeatSaberUI::CreateUIButton(rowgroup->get_rectTransform(), fileent->d_name, LevelClearedSelectSound);
            button->get_gameObject()->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_fontStyle(2);
            LevelClearedQSlist.push_back(button);
        }
    }
    if (LevelClearedQSlist.size() < 1)
    {
        LevelClearedListView->listtxtgroup = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(LevelClearedListView->SDlistscroll->get_transform());
        QuestUI::BeatSaberUI::CreateText(LevelClearedListView->listtxtgroup->get_rectTransform(), "No sound files were found!\nPlease add a sound file into\n"+ QSoundsConfig::LevelClearedPath +"\nto continue.", false);
    }
    (void)closedir(sounddir);
}

void LevelClearedSdListViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    LevelClearedListView = this;
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

        // Enable or Disable LevelClearedSounds
        QSoundsConfig::QSAddConfigValueToggle(QSconfigcontainer->get_rectTransform(), "Custom LevelClearedSounds", QSoundsConfig::Config.levelCleared_Active, SDlistscroll, "Activates or deactivates Custom LevelClearedSounds");


        // Sound List (recursively adds buttons as ListView isn't an easy type to deal with)
        this->SDlistscroll = QuestUI::BeatSaberUI::CreateScrollView(container->get_rectTransform());
        SDlistscroll->GetComponent<QuestUI::ExternalComponents*>()->Get<UnityEngine::UI::LayoutElement*>()->set_minHeight(56.0);
        auto* SDlistcontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(SDlistscroll->get_transform());
        SDlistcontainer->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
        SDlistcontainer->set_childForceExpandHeight(false);
        SDlistcontainer->set_childControlHeight(true);

        this->SDlistscroll->get_gameObject()->SetActive(QSoundsConfig::Config.levelCleared_Active);
    }
    LevelClearedRefreshList();
}

void LevelClearedSdListViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling)
{
    //QSoundsConfig::SaveConfig();
    for (UnityEngine::UI::Button* button : LevelClearedQSlist) UnityEngine::Object::Destroy(button->get_transform()->get_parent()->get_gameObject());
    LevelClearedQSlist = {};
}