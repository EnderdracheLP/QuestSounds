#include "QSoundsConfig.hpp"
#include "QSoundsViewController.hpp"
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
DEFINE_TYPE(QuestSounds, QSoundsViewController);
#elif defined(DEFINE_TYPE) && defined(REGISTER_FUNCTION)
DEFINE_TYPE(QuestSounds::QSoundsViewController);
#elif defined(DEFINE_CLASS)
DEFINE_CLASS(QuestSounds::QSoundsViewController);
#endif

std::list<UnityEngine::UI::Button*> AClipList = {};

inline ::UnityEngine::UI::Toggle* QSAddConfigValueToggle(::UnityEngine::Transform* parent, ConfigValue& configValue, std::string text, bool* config, std::string HoverHint = nullptr) {
    auto object = ::QuestUI::BeatSaberUI::CreateToggle(parent, text, configValue.GetBool(),
        [&configValue, config](bool value) /*mutable -> bool*/ {
            getLogger().debug("Config Value before change: in ModConfig: %d, in Config_t: %d", configValue.GetBool(), *config);
            configValue.SetBool(value);
            *config = value;
            getLogger().debug("Value set: %d", value);
            getLogger().debug("Config Value after change: in ModConfig: %d, in Config_t: %d", configValue.GetBool(), *config);
            getConfig().Write();
        }
    );
    if (!HoverHint.empty())
        ::QuestUI::BeatSaberUI::AddHoverHint(object->get_gameObject(), HoverHint);
    return object;
}

ConfigValue SoundValue;
ConfigValue HitSoundValue = SoundValue["HitSound"].GetObject();

void HitSoundToggle(QuestSounds::QSoundsViewController* parent, bool newValue) {
    HitSoundValue["activated"].SetBool(newValue);
    getConfig().Write();
}
//void HitSoundVolume(QuestSounds::QSoundsViewController* parent, float newValue) {
//    HitSoundValue["Volume"].SetFloat(newValue);
//    getConfig().Write();
//}
void QuestSounds::QSoundsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (firstActivation) {
        SoundValue = getConfig().config[CONFIG_VERSION].GetObject();
        get_gameObject()->AddComponent<Touchable*>();
        GameObject* container = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(get_transform());
        // Enable or Disable HitSounds
        Transform* parent = container->get_transform();
        QSAddConfigValueToggle(parent, HitSoundValue["activated"], "Custom HitSound", &Config.hitSound_Active, "Activates or deactives HitSound");
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
void QuestSounds::QSoundsViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling) {
    //SaveConfig();
}