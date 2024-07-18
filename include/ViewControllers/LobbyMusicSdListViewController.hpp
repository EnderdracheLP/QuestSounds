#pragma once

#include "custom-types/shared/macros.hpp"
#include "HMUI/ViewController.hpp"

DECLARE_CLASS_CODEGEN(QuestSounds::ViewControllers, LobbyMusicSdListViewController, HMUI::ViewController,
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::VerticalLayoutGroup*, QSconfigcontainer);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::HorizontalLayoutGroup*, listtxtgroup);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, SDlistscroll);

    DECLARE_OVERRIDE_METHOD_MATCH(void, DidActivate, &HMUI::ViewController::DidActivate, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD_MATCH(void, DidDeactivate, &HMUI::ViewController::DidDeactivate, bool removedFromHierarchy, bool systemScreenDisabling);
);