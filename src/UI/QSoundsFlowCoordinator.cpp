#include "main.hpp"
#include "QSoundsFlowCoordinator.hpp"
#include "ViewControllers/MenuSdListViewController.hpp"
#include "ViewControllers/HitSdListViewController.hpp"
#include "ViewControllers/MenuClickSdListViewController.hpp"
#include "ViewControllers/BadHitSdListViewController.hpp"
#include "ViewControllers/FireworkSdListViewController.hpp"
#include "ViewControllers/LevelClearedSdListViewController.hpp"
#include "ViewControllers/LobbyMusicSdListViewController.hpp"
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
        this->SetTitle(il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("QuestSounds MenuMusic"), HMUI::ViewController::AnimationDirection::Vertical);
        this->showBackButton = true;

        if (!this->QSConfigView) this->QSConfigView = QuestUI::BeatSaberUI::CreateViewController<QuestSounds::ConfigViewController*>();
        if (!this->QSMenuSoundListView) this->QSMenuSoundListView = QuestUI::BeatSaberUI::CreateViewController<QuestSounds::MenuSdListViewController*>();
        if (!this->QSHitSoundListView) this->QSHitSoundListView = QuestUI::BeatSaberUI::CreateViewController<QuestSounds::HitSdListViewController*>();
        if (!this->QSBadHitSoundListView) this->QSBadHitSoundListView = QuestUI::BeatSaberUI::CreateViewController<QuestSounds::BadHitSdListViewController*>();
        if (!this->QSMenuClickSoundListView) this->QSMenuClickSoundListView = QuestUI::BeatSaberUI::CreateViewController<QuestSounds::MenuClickSdListViewController*>();
        if (!this->QSFireworkSoundListView) this->QSFireworkSoundListView = QuestUI::BeatSaberUI::CreateViewController<QuestSounds::FireworkSdListViewController*>();
        if (!this->QSLevelClearedSoundListView) this->QSLevelClearedSoundListView = QuestUI::BeatSaberUI::CreateViewController<QuestSounds::LevelClearedSdListViewController*>();
#ifndef BS__1_13_2
        if (!this->QSLobbyMusicSoundListView) this->QSLobbyMusicSoundListView = QuestUI::BeatSaberUI::CreateViewController<QuestSounds::LobbyMusicSdListViewController*>();
#endif

        QSConfigViewController = reinterpret_cast<QuestSounds::ConfigViewController*>(QSConfigView);
        //QSConfigView->add_openSubMenu(il2cpp_utils::MakeDelegate<System::Action_1<QuestUI::CustomDataType*>*>(classof(System::Action_1<QuestUI::CustomDataType*>*), this, OnOpenSubMenu));
        
        std::function<void(int)> func = std::bind(&QSoundsFlowCoordinator::SubMenuButtonWasPressed, this, std::placeholders::_1);
        QSConfigViewController->set_selectCallback(func);

        //if (!this->QSConfigView) this->QSConfigView = QuestUI::BeatSaberUI::CreateViewController<QSConfigViewController>();

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
        if (!this->QSMenuSoundListView) this->QSMenuSoundListView = QuestUI::BeatSaberUI::CreateViewController<QuestSounds::MenuSdListViewController*>();
        if (!this->QSMenuSoundListView) break;
        this->SetTitle(il2cpp_utils::createcsstr("QuestSounds MenuMusic"), HMUI::ViewController::AnimationType::In);
        this->ReplaceTopViewController(this->QSMenuSoundListView, this, this, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
        CurrentActive = VCtype;
        break;
    case 2:
        if (!this->QSHitSoundListView) this->QSHitSoundListView = QuestUI::BeatSaberUI::CreateViewController<QuestSounds::HitSdListViewController*>();
        if (!this->QSHitSoundListView) break;
        this->SetTitle(il2cpp_utils::createcsstr("QuestSounds HitSounds"), HMUI::ViewController::AnimationType::In);
        this->ReplaceTopViewController(this->QSHitSoundListView, this, this, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
        CurrentActive = VCtype;
        break;
    case 3:
        if (!this->QSMenuClickSoundListView) this->QSMenuClickSoundListView = QuestUI::BeatSaberUI::CreateViewController<QuestSounds::MenuClickSdListViewController*>();
        if (!this->QSMenuClickSoundListView || this->providedMainViewController == this->QSMenuClickSoundListView) break;
        this->SetTitle(il2cpp_utils::createcsstr("QuestSounds MenuClicks"), HMUI::ViewController::AnimationType::In);
        this->ReplaceTopViewController(this->QSMenuClickSoundListView, this, this, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
        CurrentActive = VCtype;
        break;
    case 4:
        if (!this->QSBadHitSoundListView) this->QSBadHitSoundListView = QuestUI::BeatSaberUI::CreateViewController<QuestSounds::BadHitSdListViewController*>();
        if (!this->QSBadHitSoundListView || this->providedMainViewController == this->QSBadHitSoundListView) break;
        this->SetTitle(il2cpp_utils::createcsstr("QuestSounds BadHitSounds"), HMUI::ViewController::AnimationType::In);
        this->ReplaceTopViewController(this->QSBadHitSoundListView, this, this, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
        CurrentActive = VCtype;
        break;
    case 5:
        if (!this->QSFireworkSoundListView) this->QSFireworkSoundListView = QuestUI::BeatSaberUI::CreateViewController<QuestSounds::FireworkSdListViewController*>();
        if (!this->QSFireworkSoundListView || this->providedMainViewController == this->QSFireworkSoundListView) break;
        this->SetTitle(il2cpp_utils::createcsstr("QuestSounds FireworkSounds"), HMUI::ViewController::AnimationType::In);
        this->ReplaceTopViewController(this->QSFireworkSoundListView, this, this, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
        CurrentActive = VCtype;
        break;
    case 6:
        if (!this->QSLevelClearedSoundListView) this->QSLevelClearedSoundListView = QuestUI::BeatSaberUI::CreateViewController<QuestSounds::LevelClearedSdListViewController*>();
        if (!this->QSLevelClearedSoundListView || this->providedMainViewController == this->QSLevelClearedSoundListView) break;
        this->SetTitle(il2cpp_utils::createcsstr("QuestSounds LevelClearedSounds"), HMUI::ViewController::AnimationType::In);
        this->ReplaceTopViewController(this->QSLevelClearedSoundListView, this, this, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
        CurrentActive = VCtype;
        break;
#ifndef BS__1_13_2
    case 7:
        if (!this->QSLobbyMusicSoundListView) this->QSLobbyMusicSoundListView = QuestUI::BeatSaberUI::CreateViewController<QuestSounds::LobbyMusicSdListViewController*>();
        if (!this->QSLobbyMusicSoundListView || this->providedMainViewController == this->QSLobbyMusicSoundListView) break;
        this->SetTitle(il2cpp_utils::createcsstr("QuestSounds FireworkSounds"), HMUI::ViewController::AnimationType::In);
        this->ReplaceTopViewController(this->QSLobbyMusicSoundListView, this, this, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
        CurrentActive = VCtype;
        break;
#endif
        //case 3:
        //    if (!self->WallSwitcherViewController) break;
        //    self->SetTitle(il2cpp_utils::createcsstr("QuestSounds BadHitSounds"), HMUI::ViewController::AnimationType::In);
        //    self->ReplaceTopViewController(self->WallSwitcherViewController, self, self, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
        //    self->ActiveViewController = self->WallSwitcherViewController;
        //    break;
    default:
        getLogger().debug("switch default");
        break;
    }
}


void QSoundsFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topView)
{
    QSoundsConfig::SaveConfig();
    //QuestSounds::AudioClips::loadAudioClips();
    HMUI::FlowCoordinator* ModSettingsFC = QuestUI::GetModSettingsFlowCoordinator();
    ModSettingsFC->DismissFlowCoordinator(this, HMUI::ViewController::AnimationDirection::Horizontal, nullptr, false);
}