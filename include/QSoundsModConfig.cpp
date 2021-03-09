#pragma once
#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(ModConfig,

    DECLARE_VALUE(ModBool, bool, "ModConfigBool", true);
    DECLARE_VALUE(ModInt, int, "ModConfignt", 1337);
    DECLARE_VALUE(ModFloat, float, "ModConfigFloat", 10.0f);
    DECLARE_VALUE(ModString, std::string, "ModConfigString", "hi");
    DECLARE_VALUE(ModColor, UnityEngine::Color, "ModConfigColor", UnityEngine::Color(0.0f, 0.0f, 0.0f, 0.0f));

INIT_FUNCTION(
    INIT_VALUE(ModBool);
    INIT_VALUE(ModInt);
    INIT_VALUE(ModFloat);
    INIT_VALUE(ModString);
    INIT_VALUE(ModColor);
    )
)