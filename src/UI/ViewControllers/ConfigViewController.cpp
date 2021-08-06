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
            if (QSoundsConfig::LegacyConfig) {
                parent->set_localPosition(UnityEngine::Vector3(38.0f, 5.0f));
            }
            else {
                parent->set_localPosition(UnityEngine::Vector3(38.0f, 0.0f));
            }

            VerticalLayoutGroup* settingsLayout = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(parent);
            RectTransform* settingsLayoutTransform = settingsLayout->GetComponent<RectTransform*>();
            settingsLayout->get_gameObject()->AddComponent<Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));
            settingsLayout->set_spacing(1.2f);
            settingsLayout->set_padding(UnityEngine::RectOffset::New_ctor(3, 3, 2, 2));

            ContentSizeFitter* contentSizeFitter = settingsLayout->get_gameObject()->AddComponent<ContentSizeFitter*>();
            contentSizeFitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);
            contentSizeFitter->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);

            if (QSoundsConfig::LegacyConfig) QuestUI::BeatSaberUI::CreateText(settingsLayoutTransform, "Legacy Config Detected,\nplease move your files into the new folders,\nfor selection in-game", false);

            BeatSaberUI::CreateUIButton(settingsLayoutTransform, "MenuMusic",
                [&]() {
                    getLogger().debug("MenuMusic Button pressed!");
                    this->callback(1);
                })->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

            BeatSaberUI::CreateUIButton(settingsLayoutTransform, "MenuClickSounds",
                [&]() {
                    getLogger().debug("MenuClickSounds Button pressed!");
                    this->callback(3);
                })->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);


            BeatSaberUI::CreateUIButton(settingsLayoutTransform, "HitSounds",
                [&]() {
                    getLogger().debug("HitSounds Button pressed!");
                    this->callback(2);
                })->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

            BeatSaberUI::CreateUIButton(settingsLayoutTransform, "BadHitSounds",
                [&]() {
                    getLogger().debug("BadHitSounds Button pressed!");
                    this->callback(4);
                })->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

            BeatSaberUI::CreateUIButton(settingsLayoutTransform, "FireworkSounds",
                [&]() {
                    getLogger().debug("FireworkSounds Button pressed!");
                    this->callback(5);
                })->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

            BeatSaberUI::CreateUIButton(settingsLayoutTransform, "LevelClearedSounds",
                [&]() {
                    getLogger().debug("LevelClearedSounds Button pressed!");
                    this->callback(6);
                })->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

            BeatSaberUI::CreateUIButton(settingsLayoutTransform, "LevelFailedSounds",
                [&]() {
                    getLogger().debug("LevelFailedSounds Button pressed!");
                    this->callback(7);
                })->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);


    #ifndef BS__1_13_2
            BeatSaberUI::CreateUIButton(settingsLayoutTransform, "LobbyMusic",
                [&]() {
                    getLogger().debug("LobbyMusic Button pressed!");
                    this->callback(8);
                })->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);
    #endif
        }
    }

    void ConfigViewController::set_selectCallback(std::function<void(int)> callback)
    {
        this->callback = callback;
    }
}
