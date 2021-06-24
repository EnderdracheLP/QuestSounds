#include "QSoundsConfig.hpp"
#include "QSoundsConfigViewController.hpp"
#include "main.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/GameObject.hpp"
#include "HMUI/Touchable.hpp"
#include "HMUI/ScrollView.hpp"
using namespace QuestUI;
using namespace UnityEngine::UI;
using namespace UnityEngine;
using namespace HMUI;
using namespace QSoundsConfig;

#if !defined(REGISTER_FUNCTION)
DEFINE_TYPE(QuestSounds, QSoundsConfigViewController);
#elif defined(DEFINE_TYPE) && defined(REGISTER_FUNCTION)
DEFINE_TYPE(QuestSounds::QSoundsConfigViewController);
#elif defined(DEFINE_CLASS)
DEFINE_CLASS(QuestSounds::QSoundsConfigViewController);
#endif

//ConfigValue SoundValue;
//ConfigValue HitSoundValue;
//ConfigValue HitSoundString = HitSoundValue["filepath"];


inline ::UnityEngine::UI::Toggle* QSAddConfigValueToggle(::UnityEngine::Transform* parent, /*ConfigValue& configValue,*/ std::string text, bool* config, std::string HoverHint = nullptr) {
    auto object = ::QuestUI::BeatSaberUI::CreateToggle(parent, text, config,
        [config](bool value) {
            *config = value;
        }
    );
    if (!HoverHint.empty())
        ::QuestUI::BeatSaberUI::AddHoverHint(object->get_gameObject(), HoverHint);
    return object;
}

//void HitSoundToggle(QuestSounds::QSoundsConfigViewController* parent, bool newValue) {
//    //HitSoundValue["activated"].SetBool(newValue);
//    Config.hitSound_Active = newValue;
//    getConfig().Write();
//}
//void HitSoundVolume(QuestSounds::QSoundsViewController* parent, float newValue) {
//    HitSoundValue["Volume"].SetFloat(newValue);
//    getConfig().Write();
//}
void QuestSounds::QSoundsConfigViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (firstActivation) {
        //SoundValue = getConfig().config[CONFIG_VERSION].GetObject();
        //HitSoundValue = SoundValue["HitSound"].GetObject();
        get_gameObject()->AddComponent<Touchable*>();
        //GameObject* container = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(get_transform());
        UnityEngine::UI::VerticalLayoutGroup* container = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_rectTransform());
        container->set_spacing(0.2f);
        container->GetComponent<UnityEngine::UI::LayoutElement*>()->set_minWidth(25.0);

        // Bool settings
        UnityEngine::UI::VerticalLayoutGroup* configcontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(container->get_rectTransform());
        configcontainer->set_childAlignment(UnityEngine::TextAnchor::UpperLeft);
        configcontainer->set_childForceExpandHeight(false);
        configcontainer->set_childControlHeight(true);

        // Enable or Disable HitSounds
        //Transform* parent = container->get_transform();
        QSAddConfigValueToggle(configcontainer->get_rectTransform(), "Custom HitSound", &Config.hitSound_Active, "Activates or deactivates HitSound");
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
// Write config
//void QuestSounds::QSoundsConfigViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling) {
//    SaveConfig();
//    //getConfig().Write();
//}