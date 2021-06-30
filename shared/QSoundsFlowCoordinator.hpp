#pragma once
#include "custom-types/shared/macros.hpp"
#include "HMUI/FlowCoordinator.hpp"
#include "HMUI/ViewController.hpp"

#include "ViewControllers/ConfigViewController.hpp"

DECLARE_CLASS_CODEGEN(QuestSounds, QSoundsFlowCoordinator, HMUI::FlowCoordinator,

    DECLARE_INSTANCE_FIELD_DEFAULT(QuestSounds::ConfigViewController*, QSConfigViewController, nullptr);
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "FlowCoordinator", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, BackButtonWasPressed, il2cpp_utils::FindMethodUnsafe("HMUI", "FlowCoordinator", "BackButtonWasPressed", 1), HMUI::ViewController* topViewController);

    DECLARE_INSTANCE_FIELD(HMUI::ViewController*, QSConfigView);
    DECLARE_INSTANCE_FIELD(HMUI::ViewController*, QSMenuSoundListView);
    DECLARE_INSTANCE_FIELD(HMUI::ViewController*, QSHitSoundListView);
    DECLARE_INSTANCE_FIELD(HMUI::ViewController*, QSMenuClickSoundListView);
    DECLARE_INSTANCE_FIELD(HMUI::ViewController*, QSBadHitSoundListView);
    DECLARE_INSTANCE_FIELD(HMUI::ViewController*, QSFireworkSoundListView);
    DECLARE_INSTANCE_FIELD(HMUI::ViewController*, QSLevelClearedSoundListView);
#ifndef BS__1_13_2
    DECLARE_INSTANCE_FIELD(HMUI::ViewController*, QSLobbyMusicSoundListView);
#endif


    public:
        void SubMenuButtonWasPressed(int type);

#if defined(BS__1_16) && defined(REGISTER_FUNCTION)
    REGISTER_FUNCTION(
#elif defined(BS__1_13_2)
    REGISTER_FUNCTION(QSoundsFlowCoordinator,
#endif
#if defined(REGISTER_METHOD)
        REGISTER_FIELD(QSConfigViewController);
        REGISTER_FIELD(QSConfigView);
        REGISTER_FIELD(QSMenuSoundListView);
        REGISTER_FIELD(QSHitSoundListView);
        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD(BackButtonWasPressed);
    )
#endif
);