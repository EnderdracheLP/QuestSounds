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

DEFINE_TYPE(QuestSounds::QSoundsViewController);

std::list<UnityEngine::UI::Button*> AClipList = {};

void HitSoundToggle(QuestSounds::QSoundsViewController* parent, bool newValue) {
    HitSound["activated"].SetBool(newValue);
}
void HitSoundVolume(QuestSounds::QSoundsViewController* parent, float newValue) {
    HitSound["Volume"].SetFloat(newValue);
}
void QuestSounds::QSoundsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (firstActivation) {

        get_gameObject()->AddComponent<Touchable*>();
        GameObject* container = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(get_transform());
        //// Enable or Disable HitSounds
        //auto onHitSoundSettingChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(
        //    classof(UnityEngine::Events::UnityAction_1<bool>*), this, HitSoundToggle
        //    );
        //UnityEngine::UI::Toggle* HitSoundToggleObject = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Enable/Disable HitSounds", HitSound["activated"].GetBool(), onHitSoundSettingChange);
        //QuestUI::BeatSaberUI::AddHoverHint(HitSoundToggleObject->get_gameObject(), "If the HitSound should be enabled.");
        // //Change HitSoundVolume
        //auto onHitSoundVolumeChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(
        //    classof(UnityEngine::Events::UnityAction_1<float>*), this, HitSoundVolume);                                 //              Decimal places   Value Steps                              Min Value   Max Value
        //QuestUI::IncrementSetting* HitSoundVolIncrementObject = QuestUI::BeatSaberUI::CreateIncrementSetting(container->get_transform(), "HitSound Volume", 1.0f, 0.1f, HitSound["Volume"].GetFloat(), 0.0f, 5.0f, onHitSoundVolumeChange);
        //QuestUI::BeatSaberUI::AddHoverHint(HitSoundVolIncrementObject->get_gameObject(), "Volume for the HitSound default (1)");
    }
}
// Write config
void QuestSounds::QSoundsViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling) {
    SaveConfig();
}
