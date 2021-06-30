#include "main.hpp"
#include "QSoundsConfig.hpp"
#include "ViewControllers/MenuClickSdListViewController.hpp"
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
#include "UnityEngine/Events/UnityAction.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
using namespace QuestSounds;

#ifndef REGISTER_FUNCTION
DEFINE_TYPE(QuestSounds, MenuClickSdListViewController);
#elif defined(DEFINE_TYPE)
DEFINE_TYPE(QuestSounds::MenuClickSdListViewController);
#elif defined(DEFINE_CLASS)
DEFINE_CLASS(QuestSounds::MenuClickSdListViewController);
#endif

MenuClickSdListViewController* MenuClickListView;
std::list<UnityEngine::UI::Button*> MenuClickQSlist = {};

void MenuClickSelectSound()
{
    for (UnityEngine::UI::Button* button : MenuClickQSlist)
    {
        if (button->hasSelection)
        {
            std::string filename = to_utf8(csstrtostr(button->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->get_text()));
            QSoundsConfig::Config.menuClick_filepath = QSoundsConfig::soundPath + filename;
            AudioClips::menuClickLoader.filePath = QSoundsConfig::Config.menuClick_filepath;
            AudioClips::menuClickLoader.loaded = false;
            AudioClips::menuClickLoader.load();
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

        if (std::regex_search(filename, std::regex(".ogg|.mp3|.wav|.aiff|.aif")))
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
        QuestUI::BeatSaberUI::CreateText(MenuClickListView->listtxtgroup->get_rectTransform(), "No sound files were found!\nPlease add a sound file into\n"+ QSoundsConfig::MenuClickPath +"\nto continue.", false);
    }
    (void)closedir(sounddir);
}

void MenuClickSdListViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    MenuClickListView = this;
    if (firstActivation && addedToHierarchy)
    {
        UnityEngine::UI::VerticalLayoutGroup* container = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_rectTransform());
        container->set_spacing(0.4f);
        container->GetComponent<UnityEngine::UI::LayoutElement*>()->set_minWidth(125.0);

        // Sound List (recursively adds buttons as ListView isn't an easy type to deal with)
        this->SDlistscroll = QuestUI::BeatSaberUI::CreateScrollView(container->get_rectTransform());
        SDlistscroll->GetComponent<QuestUI::ExternalComponents*>()->Get<UnityEngine::UI::LayoutElement*>()->set_minHeight(56.0);
        auto* SDlistcontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(SDlistscroll->get_transform());
        SDlistcontainer->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
        SDlistcontainer->set_childForceExpandHeight(false);
        SDlistcontainer->set_childControlHeight(true);

        this->SDlistscroll->get_gameObject()->SetActive(true);
    }
    MenuClickRefreshList();
}

void MenuClickSdListViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling)
{
    //QSoundsConfig::SaveConfig();
    for (UnityEngine::UI::Button* button : MenuClickQSlist) UnityEngine::Object::Destroy(button->get_transform()->get_parent()->get_gameObject());
    MenuClickQSlist = {};
}