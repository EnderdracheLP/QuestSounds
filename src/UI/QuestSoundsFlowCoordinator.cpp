#include "UI/QuestSoundsFlowCoordinator.hpp"
#include "Config.hpp"
#include "logging.hpp"
#include "AudioClips.hpp"

#include "bsml/shared/Helpers/creation.hpp"

DEFINE_TYPE(QuestSounds::UI, QuestSoundsFlowCoordinator);

namespace QuestSounds::UI {
    void QuestSoundsFlowCoordinator::Awake() {
        getLogger().info("Awake");
        if (!soundSettingsViewController) {
            getLogger().info("Awake ViewController null, Creating SoundSettingsViewController");
            soundSettingsViewController = BSML::Helpers::CreateViewController<SoundSettingsViewController*>();
            // TODO: Implement iteration through all sounds in the Config and switching between them also possible use some sort of safeptr for the config and loader
            // Testing using HitSounds for now
            soundSettingsViewController->Setup("HitSound", &QuestSounds::Config.Sounds.HitSound, &QuestSounds::AudioClips::hitSoundLoader);
        }
    }

    void QuestSoundsFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        getLogger().info("DidActivate");
        if (firstActivation) {
            ProvideInitialViewControllers(soundSettingsViewController, nullptr, nullptr, nullptr, nullptr);
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
}