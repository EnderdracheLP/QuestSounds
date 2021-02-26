#pragma once
#include "main.hpp"
#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(QuestSounds, NewAudioClipLoader, UnityEngine::MonoBehaviour,

    DECLARE_METHOD(void, audioClipCompleted);

    REGISTER_FUNCTION(NewAudioClipLoader,
        REGISTER_METHOD(audioClipCompleted);
    )
)