#include "main.hpp"
#include "QSoundsFlowCoordinator.hpp"
#include "QSoundsMenuSdListViewController.hpp"
#include "QSoundsConfigViewController.hpp"
#include "QSoundsConfig.hpp"
//#include "audiocliploader.hpp"
#include "AudioClips.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

#include "HMUI/ViewController.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/FlowCoordinator.hpp"
using namespace QuestSounds;

DEFINE_TYPE(QSoundsFlowCoordinator);

void QSoundsFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    if (firstActivation)
    {
        this->SetTitle(il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("Sounds"), HMUI::ViewController::AnimationDirection::Vertical);
        this->showBackButton = true;

        if (!this->QSMenuSoundListView) this->QSMenuSoundListView = QuestUI::BeatSaberUI::CreateViewController<QuestSounds::QSoundsMenuSdListViewController*>();
        if (!this->QSConfigView) this->QSConfigView = QuestUI::BeatSaberUI::CreateViewController<QuestSounds::QSoundsConfigViewController*>();
        QSoundsFlowCoordinator::ProvideInitialViewControllers(QSMenuSoundListView, QSConfigView, nullptr, nullptr, nullptr);
    }
}

void QSoundsFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topView)
{
    QSoundsConfig::SaveConfig();
    QuestSounds::AudioClips::loadAudioClips();
    HMUI::FlowCoordinator* ModSettingsFC = QuestUI::GetModSettingsFlowCoordinator();
    ModSettingsFC->DismissFlowCoordinator(this, HMUI::ViewController::AnimationDirection::Horizontal, nullptr, false);
}