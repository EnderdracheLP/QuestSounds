#include "QSoundsViewController.hpp"
#include "main.hpp"

#include "questui/shared/BeatSaberUI.hpp"

#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/GameObject.hpp"
#include "HMUI/Touchable.hpp"
#include "HMUI/ScrollView.hpp"
using namespace UnityEngine::UI;
using namespace UnityEngine;
using namespace HMUI;

DEFINE_CLASS(QuestSounds::QSoundViewController);

void InSongToggle(QuestSounds::QSoundViewController* parent, bool newValue) {
    getConfig().config["insong"].SetBool(newValue);
}
void SetFontSize(QuestSounds::QSoundViewController* parent, float newValue) {
    getConfig().config["FontSize"].SetFloat(newValue);
}
void QuestSounds::QSoundViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (firstActivation) {

        get_gameObject()->AddComponent<Touchable*>();
        GameObject* container = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(get_transform());
        // Show Clock during Song Play
        auto onInSongSettingChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(
            classof(UnityEngine::Events::UnityAction_1<bool>*), this, InSongToggle
            );
        UnityEngine::UI::Toggle* InsongToggleObject = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Show During Song", getConfig().config["insong"].GetBool(), onInSongSettingChange);
        QuestUI::BeatSaberUI::AddHoverHint(InsongToggleObject->get_gameObject(), "If the Clock should be shown while playing a Beatmap.");
        // Change Size
        auto onFontSizeChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(
            classof(UnityEngine::Events::UnityAction_1<float>*), this, SetFontSize);                                 //              Decimal places   Value Steps                              Min Value   Max Value
        QuestUI::IncrementSetting* FontIncrementObject = QuestUI::BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Font Size", 1, 0.1, getConfig().config["FontSize"].GetFloat(), 1.0f, 5.0f, onFontSizeChange);
        QuestUI::BeatSaberUI::AddHoverHint(FontIncrementObject->get_gameObject(), "Changes the Font Size of the Clock (Default: 4)");
    }
}
// Write config
void QuestSounds::QSoundViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling) {
//    getConfig().Write();
}