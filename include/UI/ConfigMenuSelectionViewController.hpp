#pragma once

#include "custom-types/shared/macros.hpp"
#include "bsml/shared/macros.hpp"

#include "HMUI/ViewController.hpp"

DECLARE_CLASS_CODEGEN(QuestSounds::UI, ConfigMenuSelectionViewController, HMUI::ViewController) {
    // DECLARE_INSTANCE_FIELD(ListW<BSML::CustomCellInfo*>, _menus);
    // DECLARE_INSTANCE_FIELD(BSML::CustomListTableData*, menuList);
    
    DECLARE_OVERRIDE_METHOD_MATCH(void, DidActivate, &HMUI::ViewController::DidActivate, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    // DECLARE_OVERRIDE_METHOD_MATCH(void, DidDeactivate, &HMUI::ViewController::DidDeactivate, bool removedFromHierarchy, bool screenSystemDisabling);

    DECLARE_INSTANCE_METHOD(void, MenuMusicButtonPressed);
    DECLARE_INSTANCE_METHOD(void, LobbyMusicButtonPressed);
    DECLARE_INSTANCE_METHOD(void, MenuClickButtonPressed);
    DECLARE_INSTANCE_METHOD(void, HitSoundButtonPressed);
    DECLARE_INSTANCE_METHOD(void, BadHitSoundButtonPressed);
    DECLARE_INSTANCE_METHOD(void, MissSoundButtonPressed);
    DECLARE_INSTANCE_METHOD(void, LevelClearedButtonPressed);
    DECLARE_INSTANCE_METHOD(void, LevelFailedButtonPressed);
    DECLARE_INSTANCE_METHOD(void, FireworkButtonPressed);
    DECLARE_INSTANCE_METHOD(void, BombExplosionSoundButtonPressed);

    DECLARE_CTOR(ctor);

public:
    void set_selectCallback(std::function<void(int)> callback);
    std::function<void(int)> callback;
};