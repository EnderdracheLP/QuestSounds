#include "main.hpp"
#include "QSoundsFlowCoordinator.hpp"
#include "ViewControllers/MenuSdListViewController.hpp"
#include "ViewControllers/HitSdListViewController.hpp"
#include "ViewControllers/MenuClickSdListViewController.hpp"
#include "ViewControllers/BadHitSdListViewController.hpp"
#include "ViewControllers/NoteMissedSdListViewController.hpp"
#include "ViewControllers/FireworkSdListViewController.hpp"
#include "ViewControllers/LevelClearedSdListViewController.hpp"
#include "ViewControllers/LevelFailedSdListViewController.hpp"
#ifndef BS__1_13_2
#include "ViewControllers/LobbyMusicSdListViewController.hpp"
#endif
#include "ViewControllers/ConfigViewController.hpp"
#include "QSoundsConfig.hpp"
//#include "audiocliploader.hpp"
#include "AudioClips.hpp"
#include "ObjectInstances.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

#include "HMUI/ViewController.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/FlowCoordinator.hpp"
using namespace QuestSounds;
using namespace QuestSounds::ViewControllers;

#ifndef REGISTER_FUNCTION
DEFINE_TYPE(QuestSounds, QSoundsFlowCoordinator);
#elif defined(DEFINE_TYPE)
DEFINE_TYPE(QSoundsFlowCoordinator);
#elif defined(DEFINE_CLASS)
DEFINE_CLASS(QSoundsFlowCoordinator);
#else
#error Unsupported Custom-Types version!
#endif

//void FindViewControllers(QuestSounds::QSoundsFlowCoordinator* self)
//{
//    //self->QSMenuSoundListView = UnityUtils::GetFirstObjectOfType<QuestSounds::QSoundsMenuSdListViewController*>();
//
//    //self->QSHitSoundListView = UnityUtils::GetFirstObjectOfType<QuestSounds::QSoundsHitSdListViewController*>();
//
//    //self->WallSwitcherViewController = UnityUtils::GetFirstObjectOfType<Qosmetics::WallSwitcherViewController*>();
//
//    //self->QosmeticsViewController = UnityUtils::GetFirstObjectOfType<Qosmetics::QosmeticsViewController*>();
//}

int CurrentActive;

void QSoundsFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    CurrentActive = 1;
    if (firstActivation)
    {
        this->SetTitle("QuestSounds MenuMusic", HMUI::ViewController::AnimationDirection::Vertical);
        this->showBackButton = true;

        if (!this->QSConfigView) this->QSConfigView = QuestUI::BeatSaberUI::CreateViewController<ConfigViewController*>();
        if (!this->QSMenuSoundListView) this->QSMenuSoundListView = QuestUI::BeatSaberUI::CreateViewController<MenuSdListViewController*>();
        if (!this->QSHitSoundListView) this->QSHitSoundListView = QuestUI::BeatSaberUI::CreateViewController<HitSdListViewController*>();
        if (!this->QSBadHitSoundListView) this->QSBadHitSoundListView = QuestUI::BeatSaberUI::CreateViewController<BadHitSdListViewController*>();
        if (!this->QSNoteMissedSoundListView) this->QSNoteMissedSoundListView = QuestUI::BeatSaberUI::CreateViewController<NoteMissedSdListViewController*>();
        if (!this->QSMenuClickSoundListView) this->QSMenuClickSoundListView = QuestUI::BeatSaberUI::CreateViewController<MenuClickSdListViewController*>();
        if (!this->QSFireworkSoundListView) this->QSFireworkSoundListView = QuestUI::BeatSaberUI::CreateViewController<FireworkSdListViewController*>();
        if (!this->QSLevelClearedSoundListView) this->QSLevelClearedSoundListView = QuestUI::BeatSaberUI::CreateViewController<LevelClearedSdListViewController*>();
        if (!this->QSLevelFailedSoundListView) this->QSLevelFailedSoundListView = QuestUI::BeatSaberUI::CreateViewController<LevelFailedSdListViewController*>();
#ifndef BS__1_13_2
        if (!this->QSLobbyMusicSoundListView) this->QSLobbyMusicSoundListView = QuestUI::BeatSaberUI::CreateViewController<LobbyMusicSdListViewController*>();
#endif

        QSConfigViewController = reinterpret_cast<QuestSounds::ViewControllers::ConfigViewController*>(QSConfigView);
        
        std::function<void(int)> func = std::bind(&QSoundsFlowCoordinator::SubMenuButtonWasPressed, this, std::placeholders::_1);
        QSConfigViewController->set_selectCallback(func);

        QSoundsFlowCoordinator::ProvideInitialViewControllers(QSMenuSoundListView, QSConfigViewController, nullptr, nullptr, nullptr);
    }
}

void QSoundsFlowCoordinator::SubMenuButtonWasPressed(int VCtype) {
    if (!VCtype) return;
    //FindViewControllers(self);
    if (CurrentActive == VCtype) return;
    switch (VCtype)
    {
    case 1:
        if (!this->QSMenuSoundListView) this->QSMenuSoundListView = QuestUI::BeatSaberUI::CreateViewController<MenuSdListViewController*>();
        if (!this->QSMenuSoundListView) break;
        this->SetTitle("Menu Music", HMUI::ViewController::AnimationType::In);
        this->ReplaceTopViewController(this->QSMenuSoundListView, this, this, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
        CurrentActive = VCtype;
        break;
    case 2:
        if (!this->QSHitSoundListView) this->QSHitSoundListView = QuestUI::BeatSaberUI::CreateViewController<HitSdListViewController*>();
        if (!this->QSHitSoundListView) break;
        this->SetTitle("Hit Sounds", HMUI::ViewController::AnimationType::In);
        this->ReplaceTopViewController(this->QSHitSoundListView, this, this, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
        CurrentActive = VCtype;
        break;
    case 3:
        if (!this->QSMenuClickSoundListView) this->QSMenuClickSoundListView = QuestUI::BeatSaberUI::CreateViewController<MenuClickSdListViewController*>();
        if (!this->QSMenuClickSoundListView || this->providedMainViewController == this->QSMenuClickSoundListView) break;
        this->SetTitle("Menu Clicks", HMUI::ViewController::AnimationType::In);
        this->ReplaceTopViewController(this->QSMenuClickSoundListView, this, this, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
        CurrentActive = VCtype;
        break;
    case 4:
        if (!this->QSBadHitSoundListView) this->QSBadHitSoundListView = QuestUI::BeatSaberUI::CreateViewController<BadHitSdListViewController*>();
        if (!this->QSBadHitSoundListView || this->providedMainViewController == this->QSBadHitSoundListView) break;
        this->SetTitle("Bad Hit Sounds", HMUI::ViewController::AnimationType::In);
        this->ReplaceTopViewController(this->QSBadHitSoundListView, this, this, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
        CurrentActive = VCtype;
        break;
    case 5:
        if (!this->QSNoteMissedSoundListView) this->QSNoteMissedSoundListView = QuestUI::BeatSaberUI::CreateViewController<NoteMissedSdListViewController*>();
        if (!this->QSNoteMissedSoundListView || this->providedMainViewController == this->QSNoteMissedSoundListView) break;
        this->SetTitle("Note Missed Sounds", HMUI::ViewController::AnimationType::In);
        this->ReplaceTopViewController(this->QSNoteMissedSoundListView, this, this, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
        CurrentActive = VCtype;
        break;
    case 6:
        if (!this->QSFireworkSoundListView) this->QSFireworkSoundListView = QuestUI::BeatSaberUI::CreateViewController<FireworkSdListViewController*>();
        if (!this->QSFireworkSoundListView || this->providedMainViewController == this->QSFireworkSoundListView) break;
        this->SetTitle("Firework Sounds", HMUI::ViewController::AnimationType::In);
        this->ReplaceTopViewController(this->QSFireworkSoundListView, this, this, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
        CurrentActive = VCtype;
        break;
    case 7:
        if (!this->QSLevelClearedSoundListView) this->QSLevelClearedSoundListView = QuestUI::BeatSaberUI::CreateViewController<LevelClearedSdListViewController*>();
        if (!this->QSLevelClearedSoundListView || this->providedMainViewController == this->QSLevelClearedSoundListView) break;
        this->SetTitle("Level Cleared Sounds", HMUI::ViewController::AnimationType::In);
        this->ReplaceTopViewController(this->QSLevelClearedSoundListView, this, this, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
        CurrentActive = VCtype;
        break;
    case 8:
        if (!this->QSLevelFailedSoundListView) this->QSLevelFailedSoundListView = QuestUI::BeatSaberUI::CreateViewController<LevelFailedSdListViewController*>();
        if (!this->QSLevelFailedSoundListView || this->providedMainViewController == this->QSLevelFailedSoundListView) break;
        this->SetTitle("Level Failed Sounds", HMUI::ViewController::AnimationType::In);
        this->ReplaceTopViewController(this->QSLevelFailedSoundListView, this, this, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
        CurrentActive = VCtype;
        break;
#ifndef BS__1_13_2
    case 9:
        if (!this->QSLobbyMusicSoundListView) this->QSLobbyMusicSoundListView = QuestUI::BeatSaberUI::CreateViewController<LobbyMusicSdListViewController*>();
        if (!this->QSLobbyMusicSoundListView || this->providedMainViewController == this->QSLobbyMusicSoundListView) break;
        this->SetTitle("LobbyMusic", HMUI::ViewController::AnimationType::In);
        this->ReplaceTopViewController(this->QSLobbyMusicSoundListView, this, this, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
        CurrentActive = VCtype;
        break;
#endif
    default:
        getLogger().debug("switch default");
        break;
    }
}


void QSoundsFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topView)
{
    QSoundsConfig::SaveConfig();
    //QuestSounds::AudioClips::loadAudioClips();
    this->parentFlowCoordinator->DismissFlowCoordinator(this, HMUI::ViewController::AnimationDirection::Horizontal, nullptr, false);
}