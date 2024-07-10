#include "QSoundsConfig.hpp"
#include "QSoundsFlowCoordinator.hpp"
#include "ViewControllers/ConfigViewController.hpp"
#include "main.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "questui/shared/QuestUI.hpp"

#include "UnityEngine/RectOffset.hpp"

#include "UnityEngine/Object.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/GameObject.hpp"
#include "HMUI/Touchable.hpp"
#include "HMUI/ScrollView.hpp"
using namespace QuestUI;
using namespace UnityEngine::UI;
using namespace UnityEngine;
using namespace HMUI;
using namespace QSoundsConfig;

#ifndef REGISTER_FUNCTION
DEFINE_TYPE(QuestSounds::ViewControllers, ConfigViewController);
#elif defined(DEFINE_TYPE)
DEFINE_TYPE(QuestSounds::ViewControllers::ConfigViewController);
#elif defined(DEFINE_CLASS)
DEFINE_CLASS(QuestSounds::ViewControllers::ConfigViewController);
#endif

//ConfigValue SoundValue;
//ConfigValue HitSoundValue;
//ConfigValue HitSoundString = HitSoundValue["filepath"];

//void HitSoundToggle(QuestSounds::QSoundsConfigViewController* parent, bool newValue) {
//    //HitSoundValue["activated"].SetBool(newValue);
//    Config.hitSound_Active = newValue;
//    getConfig().Write();
//}
//void HitSoundVolume(QuestSounds::QSoundsViewController* parent, float newValue) {
//    HitSoundValue["Volume"].SetFloat(newValue);
//    getConfig().Write();
//}
namespace QuestSounds::ViewControllers {

    void ConfigViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        if (firstActivation) {
            get_gameObject()->AddComponent<Touchable*>();

            GameObject* mainLayout = GameObject::New_ctor();
            RectTransform* parent = mainLayout->AddComponent<RectTransform*>();
            parent->SetParent(get_transform(), false);
            parent->set_localPosition({38.0f, 3.0f, 0.0f });

            VerticalLayoutGroup* settingsLayout = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(parent);
            RectTransform* settingsLayoutTransform = settingsLayout->GetComponent<RectTransform*>();
            settingsLayout->get_gameObject()->AddComponent<Backgroundable*>()->ApplyBackground("round-rect-panel");
            settingsLayout->set_spacing(1.2f);
            settingsLayout->set_padding(UnityEngine::RectOffset::New_ctor(3, 3, 2, 2));

            ContentSizeFitter* contentSizeFitter = settingsLayout->get_gameObject()->AddComponent<ContentSizeFitter*>();
            contentSizeFitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);
            contentSizeFitter->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);

            QuestUI::BeatSaberUI::CreateText(settingsLayoutTransform, "<line-height=130%><b>Quest Sounds</b>", false)->set_alignment(TMPro::TextAlignmentOptions::Center);

            BeatSaberUI::CreateUIButton(settingsLayoutTransform, "Menu Music",
                [&]() {
                    getLogger().debug("MenuMusic Button pressed!");
                    this->callback(1);
                })->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

            BeatSaberUI::CreateUIButton(settingsLayoutTransform, "Menu Click Sounds",
                [&]() {
                    getLogger().debug("MenuClickSounds Button pressed!");
                    this->callback(3);
                })->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);


            BeatSaberUI::CreateUIButton(settingsLayoutTransform, "Hit Sounds",
                [&]() {
                    getLogger().debug("HitSounds Button pressed!");
                    this->callback(2);
                })->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

            BeatSaberUI::CreateUIButton(settingsLayoutTransform, "Bad Hit Sounds",
                [&]() {
                    getLogger().debug("BadHitSounds Button pressed!");
                    this->callback(4);
                })->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

            BeatSaberUI::CreateUIButton(settingsLayoutTransform, "Note Missed Sounds",
                [&]() {
                    getLogger().debug("NoteMissedSounds Button pressed!");
                    this->callback(5);
                })->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

            BeatSaberUI::CreateUIButton(settingsLayoutTransform, "Firework Sounds",
                [&]() {
                    getLogger().debug("FireworkSounds Button pressed!");
                    this->callback(6);
                })->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

            BeatSaberUI::CreateUIButton(settingsLayoutTransform, "Level Cleared Sounds",
                [&]() {
                    getLogger().debug("LevelClearedSounds Button pressed!");
                    this->callback(7);
                })->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

            BeatSaberUI::CreateUIButton(settingsLayoutTransform, "Level Failed Sounds",
                [&]() {
                    getLogger().debug("LevelFailedSounds Button pressed!");
                    this->callback(8);
                })->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);
            BeatSaberUI::CreateUIButton(settingsLayoutTransform, "Lobby Music",
                [&]() {
                    getLogger().debug("LobbyMusic Button pressed!");
                    this->callback(9);
                })->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);
        }
    }

    void ConfigViewController::set_selectCallback(std::function<void(int)> callback)
    {
        this->callback = callback;
    }
}
