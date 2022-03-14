#pragma once
#include "questui/shared/BeatSaberUI.hpp"
#include "custom-types/shared/macros.hpp"
#include "HMUI/ViewController.hpp"

DECLARE_CLASS_CODEGEN(QuestSounds::ViewControllers, NoteMissedSdListViewController, HMUI::ViewController,
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::VerticalLayoutGroup*, QSconfigcontainer);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::HorizontalLayoutGroup*, listtxtgroup);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, SDlistscroll);

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 2), bool removedFromHierarchy, bool systemScreenDisabling);
#if defined(REGISTER_FUNCTION) && defined(BS__1_16)
    REGISTER_FUNCTION(
#elif defined(REGISTER_FUNCTION) && defined(BS__1_13_2)
    REGISTER_FUNCTION(BadHitSdListViewController,
#endif
#if defined(REGISTER_METHOD)
        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD(DidDeactivate);
    )
#endif
);