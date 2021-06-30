#include "QSoundsConfig.hpp"
#include "QSoundsFlowCoordinator.hpp"
#include "ViewControllers/ConfigViewController.hpp"
#include "main.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

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
DEFINE_TYPE(QuestSounds, ConfigViewController);
#elif defined(DEFINE_TYPE)
DEFINE_TYPE(QuestSounds::ConfigViewController);
#elif defined(DEFINE_CLASS)
DEFINE_CLASS(QuestSounds::ConfigViewController);
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
void QuestSounds::ConfigViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (firstActivation) {
        get_gameObject()->AddComponent<Touchable*>();
        UnityEngine::UI::VerticalLayoutGroup* container = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_rectTransform());
        container->set_spacing(0.2f);
        container->GetComponent<UnityEngine::UI::LayoutElement*>()->set_minWidth(25.0);
        container->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(40.0);

        // Bool settings
        UnityEngine::UI::VerticalLayoutGroup* configcontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(container->get_rectTransform());
        configcontainer->set_childAlignment(UnityEngine::TextAnchor::MiddleRight);
        configcontainer->set_childForceExpandHeight(false);
        configcontainer->set_childControlHeight(true);
        configcontainer->set_childForceExpandWidth(false);
        configcontainer->set_childControlWidth(true);
        configcontainer->GetComponent<UnityEngine::UI::LayoutElement*>()->set_minWidth(80.0);
        configcontainer->GetComponent<UnityEngine::RectTransform*>()->set_sizeDelta(UnityEngine::Vector2(6.0f, 6.0f));
        UnityEngine::UI::LayoutElement* ccle = configcontainer->GetComponent<UnityEngine::UI::LayoutElement*>();

        getLogger().debug("Width is minWidth: %d, preferredWidth %d, flexibleWidth %d", ccle->get_minWidth(), ccle->get_preferredWidth(), ccle->get_flexibleWidth());
        //configcontainer->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(100.0);
        //configcontainer->GetComponent<UnityEngine::RectTransform*>()->

        UnityEngine::UI::Button* MenuMusic = BeatSaberUI::CreateUIButton(configcontainer->get_rectTransform(), "MenuMusic",
            [&]() {
                getLogger().debug("MenuMusic Button pressed!");
                this->callback(1);
            });

        UnityEngine::UI::Button* MenuClicks = BeatSaberUI::CreateUIButton(configcontainer->get_rectTransform(), "MenuClickSounds",
            [&]() {
                getLogger().debug("MenuClickSounds Button pressed!");
                this->callback(3);
            });


        UnityEngine::UI::Button* HitSounds = BeatSaberUI::CreateUIButton(configcontainer->get_rectTransform(), "HitSounds",
            [&]() {
                getLogger().debug("HitSounds Button pressed!");
                this->callback(2);
            });

        UnityEngine::UI::Button* BadHitSounds = BeatSaberUI::CreateUIButton(configcontainer->get_rectTransform(), "BadHitSounds",
            [&]() {
                getLogger().debug("BadHitSounds Button pressed!");
                this->callback(4);
            });

        UnityEngine::UI::Button* FireworkSounds = BeatSaberUI::CreateUIButton(configcontainer->get_rectTransform(), "FireworkSounds",
            [&]() {
                getLogger().debug("FireworkSounds Button pressed!");
                this->callback(5);
            });

        UnityEngine::UI::Button* LevelClearedSounds = BeatSaberUI::CreateUIButton(configcontainer->get_rectTransform(), "LevelClearedSounds",
            [&]() {
                getLogger().debug("LevelClearedSounds Button pressed!");
                this->callback(6);
            });

#ifndef BS__1_13_2
        UnityEngine::UI::Button* LobbyMusic = BeatSaberUI::CreateUIButton(configcontainer->get_rectTransform(), "LobbyMusic",
            [&]() {
                getLogger().debug("LobbyMusic Button pressed!");
                this->callback(7);
            });
#endif
        //auto onHitSoundSettingChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(
        //    classof(UnityEngine::Events::UnityAction_1<bool>*), this, HitSoundToggle
        //    );
        //UnityEngine::UI::Toggle* HitSoundToggleObject = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Enable/Disable HitSounds", HitSoundValue["activated"].GetBool(), onHitSoundSettingChange);
        //QuestUI::BeatSaberUI::AddHoverHint(HitSoundToggleObject->get_gameObject(), "If the HitSound should be enabled.");
         //Change HitSoundVolume
        //auto onHitSoundVolumeChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(
        //    classof(UnityEngine::Events::UnityAction_1<float>*), this, HitSoundVolume);                                 //              Decimal places   Value Steps                              Min Value   Max Value
        //QuestUI::IncrementSetting* HitSoundVolIncrementObject = QuestUI::BeatSaberUI::CreateIncrementSetting(container->get_transform(), "HitSound Volume", 1.0f, 0.1f, HitSoundValue["Volume"].GetFloat(), 0.0f, 5.0f, onHitSoundVolumeChange);
        //QuestUI::BeatSaberUI::AddHoverHint(HitSoundVolIncrementObject->get_gameObject(), "Volume for the HitSound default (1)");
    }
}

void QuestSounds::ConfigViewController::set_selectCallback(std::function<void(int)> callback)
{
    this->callback = callback;
}

// Write config
//void QuestSounds::ConfigViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling) {
//    SaveConfig();
//    //getConfig().Write();
//}