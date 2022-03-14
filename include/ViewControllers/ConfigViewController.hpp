#pragma once
#include "custom-types/shared/macros.hpp"
#include "HMUI/ViewController.hpp"

// ViewController for the settings UI
DECLARE_CLASS_CODEGEN(QuestSounds::ViewControllers, ConfigViewController, HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    //DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 2), bool removedFromHierarchy, bool systemScreenDisabling);
    
    public:
        void set_selectCallback(std::function<void(int)> callback);
        std::function<void(int)> callback;

#if defined(BS__1_16) && defined(REGISTER_FUNCTION)
    REGISTER_FUNCTION(
#elif defined(BS__1_13_2)
    REGISTER_FUNCTION(ConfigViewController,
#endif
#if defined(REGISTER_METHOD)
        REGISTER_METHOD(DidActivate);
        //REGISTER_METHOD(DidDeactivate);
    )
#endif
);


