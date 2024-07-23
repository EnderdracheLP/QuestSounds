#include "UI/QuestSoundsFlowCoordinator.hpp"
#include "Config.hpp"
#include "logging.hpp"
#include "AudioClips.hpp"

#include "bsml/shared/Helpers/creation.hpp"

DEFINE_TYPE(QuestSounds::UI, QuestSoundsFlowCoordinator);

namespace QuestSounds::UI {
    void QuestSoundsFlowCoordinator::Awake() {
        getLogger().info("Awake");
        if (!configMenuSelectionViewController) {
            getLogger().info("Awake ViewController null, Creating ConfigMenuSelectionViewController");
            configMenuSelectionViewController = BSML::Helpers::CreateViewController<ConfigMenuSelectionViewController*>();
            configMenuSelectionViewController->set_selectCallback(std::bind(&QuestSoundsFlowCoordinator::SubMenuButtonPressed, this, std::placeholders::_1));
        }

        if (!soundSettingsViewController) {
            getLogger().info("Awake ViewController null, Creating SoundSettingsViewController");
            soundSettingsViewController = BSML::Helpers::CreateViewController<SoundSettingsViewController*>();
            // TODO: Implement iteration through all sounds in the Config and switching between them also possible use some sort of safeptr for the config and loader
            // Testing using HitSounds for now
            // soundSettingsViewController->Setup("HitSound", &QuestSounds::Config.Sounds.HitSound, &QuestSounds::AudioClips::hitSoundLoader);
        }
    }

    void QuestSoundsFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        getLogger().info("DidActivate");
        if (firstActivation) {
            ProvideInitialViewControllers(soundSettingsViewController, configMenuSelectionViewController, nullptr, nullptr, nullptr);
        }
        SetTitle("QuestSounds", HMUI::ViewController::AnimationType::In);
        showBackButton = true;
    }

    void QuestSoundsFlowCoordinator::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling) {
        getLogger().info("DidDeactivate");
    }

    void QuestSoundsFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topViewController) {
        getLogger().info("BackButtonWasPressed");
        _parentFlowCoordinator->DismissFlowCoordinator(this, HMUI::ViewController::AnimationDirection::Horizontal, nullptr, false);
    }

    void QuestSoundsFlowCoordinator::SubMenuButtonPressed(int index) {
        getLogger().info("SubMenuButtonPressed");
        getLogger().info("Index: {}", index);

        switch (index) {
            case 0:
                getLogger().info("MenuMusicButtonPressed");
                soundSettingsViewController->Setup("MenuMusic", &QuestSounds::Config.Sounds.MenuMusic, &QuestSounds::AudioClips::menuMusicLoader);
                SetTitle("Menu Music", HMUI::ViewController::AnimationType::In);
                // ReplaceTopViewController(soundSettingsViewController, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
                break;
            case 1:
                getLogger().info("LobbyMusicButtonPressed");
                soundSettingsViewController->Setup("LobbyMusic", &QuestSounds::Config.Sounds.LobbyMusic, &QuestSounds::AudioClips::lobbyAmbienceLoader);
                SetTitle("Lobby Music", HMUI::ViewController::AnimationType::In);
                // ReplaceTopViewController(soundSettingsViewController, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
                break;
            case 2:
                getLogger().info("MenuClickButtonPressed");
                soundSettingsViewController->Setup("MenuClicks", &QuestSounds::Config.Sounds.MenuClick, &QuestSounds::AudioClips::menuClickLoader);
                SetTitle("Menu Clicks", HMUI::ViewController::AnimationType::In);
                // ReplaceTopViewController(soundSettingsViewController, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
                break;
            case 3:
                getLogger().info("HitSoundButtonPressed");
                soundSettingsViewController->Setup("HitSounds", &QuestSounds::Config.Sounds.HitSound, &QuestSounds::AudioClips::hitSoundLoader);
                SetTitle("Hit Sounds", HMUI::ViewController::AnimationType::In);
                // ReplaceTopViewController(soundSettingsViewController, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
                break;
            case 4:
                getLogger().info("BadHitSoundButtonPressed");
                soundSettingsViewController->Setup("BadHitSounds", &QuestSounds::Config.Sounds.BadHitSound, &QuestSounds::AudioClips::badHitSoundLoader);
                SetTitle("Bad Hit Sounds", HMUI::ViewController::AnimationType::In);
                // ReplaceTopViewController(soundSettingsViewController, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
                break;
            case 5:
                getLogger().info("MissSoundButtonPressed");
                soundSettingsViewController->Setup("NoteMissedSound", &QuestSounds::Config.Sounds.NoteMissedSound, &QuestSounds::AudioClips::noteMissedSoundLoader);
                SetTitle("Note Miss Sound", HMUI::ViewController::AnimationType::In);
                // ReplaceTopViewController(soundSettingsViewController, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
                break;
            case 6:
                getLogger().info("LevelClearedButtonPressed");
                soundSettingsViewController->Setup("LevelCleared", &QuestSounds::Config.Sounds.LevelCleared, &QuestSounds::AudioClips::levelClearedLoader);
                SetTitle("Level Cleared", HMUI::ViewController::AnimationType::In);
                // ReplaceTopViewController(soundSettingsViewController, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
                break;
            case 7:
                getLogger().info("LevelFailedButtonPressed");
                soundSettingsViewController->Setup("LevelFailed", &QuestSounds::Config.Sounds.LevelFailed, &QuestSounds::AudioClips::levelFailedLoader);
                SetTitle("Level Failed", HMUI::ViewController::AnimationType::In);
                // ReplaceTopViewController(soundSettingsViewController, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
                break;
            case 8:
                getLogger().info("FireworkButtonPressed");
                soundSettingsViewController->Setup("Firework", &QuestSounds::Config.Sounds.Firework, &QuestSounds::AudioClips::fireworkSoundLoader);
                SetTitle("Firework", HMUI::ViewController::AnimationType::In);
                // ReplaceTopViewController(soundSettingsViewController, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
                break;
            default:
                getLogger().error("Invalid index");
                SetTitle("QuestSounds", HMUI::ViewController::AnimationType::In);
                // ReplaceTopViewController(soundSettingsViewController, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
                break;
        }
    }
}