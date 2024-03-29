#include "questui/shared/BeatSaberUI.hpp"
#include "custom-types/shared/macros.hpp"
#include "HMUI/ViewController.hpp"
#include "HMUI/ModalView.hpp"

DECLARE_CLASS_CODEGEN(QuestSounds::ViewControllers, MenuSdListViewController, HMUI::ViewController,
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::VerticalLayoutGroup*, QSconfigcontainer);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::HorizontalLayoutGroup*, listtxtgroup);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, SDlistscroll);

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 2), bool removedFromHierarchy, bool systemScreenDisabling);

    DECLARE_INSTANCE_FIELD(HMUI::ModalView*, modal);

#if defined(REGISTER_FUNCTION) && defined(BS__1_16)
    REGISTER_FUNCTION(
#elif defined(REGISTER_FUNCTION) && defined(BS__1_13_2)
    REGISTER_FUNCTION(MenuSdListViewController,
#endif
#if defined(REGISTER_METHOD)
        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD(DidDeactivate);
    )
#endif
);