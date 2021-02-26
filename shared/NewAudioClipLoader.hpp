#pragma once
#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(QuestSounds, NewAudioClipLoader, UnityEngine::MonoBehaviour,

    DECLARE_METHOD(void, Update);

    REGISTER_FUNCTION(NewAudioClipLoader,
        REGISTER_METHOD(Update);
    )
)