#pragma once

#include "custom-types/shared/macros.hpp"
#include "bsml/shared/macros.hpp"

#include "HMUI/FlowCoordinator.hpp"

#include "UI/SoundSettingsViewController.hpp"

#include "GlobalNamespace/MainFlowCoordinator.hpp"

#include "Zenject/IInitializable.hpp"

DECLARE_CLASS_CODEGEN(QuestSounds::UI, QuestSoundsFlowCoordinator, HMUI::FlowCoordinator,

    DECLARE_INSTANCE_FIELD(QuestSounds::UI::SoundSettingsViewController*, soundSettingsViewController);

    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_OVERRIDE_METHOD_MATCH(void, DidActivate, &HMUI::FlowCoordinator::DidActivate, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD_MATCH(void, DidDeactivate, &HMUI::FlowCoordinator::DidDeactivate, bool removedFromHierarchy, bool screenSystemDisabling);
    DECLARE_OVERRIDE_METHOD_MATCH(void, BackButtonWasPressed, &HMUI::FlowCoordinator::BackButtonWasPressed, HMUI::ViewController* topViewController);
    DECLARE_DEFAULT_CTOR();
)