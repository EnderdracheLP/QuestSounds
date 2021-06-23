#include "main.hpp"
#include "QSoundsConfig.hpp"
#include "QSoundsSdListViewController.hpp"

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
DEFINE_TYPE(SoundsListViewController);

SoundsListViewController* ListView;
std::list<UnityEngine::UI::Button*> QSlist = {};

//void OnChangeEnabled(bool newval)
//{
//    auto& modcfg = getConfig().config;
//    modcfg["enabled"].SetBool(newval);
//    ListView->SDlistscroll->SetActive(newval);
//    if (backgroundObject) backgroundObject->SetActive(newval);
//    else LoadBackground(bgDirectoryPath + modcfg["selectedFile"].GetString());
//    HideMenuEnv();
//}

void SelectSound()
{
    for (UnityEngine::UI::Button* button : QSlist)
    {
        if (button->hasSelection)
        {
            std::string filename = to_utf8(csstrtostr(button->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->get_text()));
            QSoundsConfig::Config.menuMusic_filepath = QSoundsConfig::soundPath + filename;
            getLogger().debug("SelectedSound Path %s", QSoundsConfig::Config.menuMusic_filepath.c_str());
            //getConfig().config["selectedFile"].SetString(filename, getConfig().config.GetAllocator());
            //getConfig().Write();
            //LoadBackground(bgDirectoryPath + filename);
        }
    }
}

void RefreshList()
{
    if (ListView->listtxtgroup && ListView->listtxtgroup->m_CachedPtr.m_value) UnityEngine::GameObject::Destroy(ListView->listtxtgroup->get_gameObject());
    for (UnityEngine::UI::Button* button : QSlist) UnityEngine::Object::Destroy(button->get_transform()->get_parent()->get_gameObject());
    QSlist = {};
    DIR* sounddir = opendir(QSoundsConfig::soundPath.c_str());
    dirent* fileent;
    while ((fileent = readdir(sounddir)) != NULL)
    {
        std::string filename = fileent->d_name;
        for (char& ch : filename) ch = tolower(ch);

        if (std::regex_search(filename, std::regex(".ogg|.mp3|.wav")))
        {
            UnityEngine::UI::HorizontalLayoutGroup* rowgroup = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(ListView->SDlistscroll->get_transform());
            UnityEngine::UI::Button* button = QuestUI::BeatSaberUI::CreateUIButton(rowgroup->get_rectTransform(), fileent->d_name, SelectSound);
            button->get_gameObject()->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_fontStyle(2);
            QSlist.push_back(button);
        }
    }
    if (QSlist.size() < 1)
    {
        ListView->listtxtgroup = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(ListView->SDlistscroll->get_transform());
        QuestUI::BeatSaberUI::CreateText(ListView->listtxtgroup->get_rectTransform(), "No sound files were found!\nPlease install a sound file to continue.", false);
    }
    (void)closedir(sounddir);
}

void SoundsListViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    ListView = this;
    if (firstActivation && addedToHierarchy)
    {
        UnityEngine::UI::VerticalLayoutGroup* container = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_rectTransform());
        container->set_spacing(0.4f);
        container->GetComponent<UnityEngine::UI::LayoutElement*>()->set_minWidth(25.0);

        // Bool settings
        this->QSconfigcontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(container->get_rectTransform());
        // QSconfigcontainer->get_gameObject()->AddComponent<QuestUI::Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));
        QSconfigcontainer->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
        QSconfigcontainer->set_childForceExpandHeight(false);
        QSconfigcontainer->set_childControlHeight(true);

        QSconfigcontainer->set_childForceExpandHeight(false);
        QSconfigcontainer->set_childControlHeight(true);

        //bool enabled_initval = getConfig().config["enabled"].GetBool();
        //this->masterEnabled = QuestUI::BeatSaberUI::CreateToggle(QSconfigcontainer->get_rectTransform(), "Enable Quest Sounds", enabled_initval, UnityEngine::Vector2(0, 0), OnChangeEnabled);

        // Image List (recursively adds buttons as ListView isn't an easy type to deal with)
        this->SDlistscroll = QuestUI::BeatSaberUI::CreateScrollView(container->get_rectTransform());
        SDlistscroll->GetComponent<QuestUI::ExternalComponents*>()->Get<UnityEngine::UI::LayoutElement*>()->set_minHeight(56.0);
        auto* SDlistcontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(SDlistscroll->get_transform());
        // SDlistcontainer->get_gameObject()->AddComponent<QuestUI::Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));
        SDlistcontainer->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
        SDlistcontainer->set_childForceExpandHeight(false);
        SDlistcontainer->set_childControlHeight(true);

        //this->SDlistscroll->get_gameObject()->SetActive(enabled_initval);
        this->SDlistscroll->get_gameObject()->SetActive(true);
    }
    RefreshList();
}

void SoundsListViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling)
{
    QSoundsConfig::SaveConfig();
    for (UnityEngine::UI::Button* button : QSlist) UnityEngine::Object::Destroy(button->get_transform()->get_parent()->get_gameObject());
    QSlist = {};
}