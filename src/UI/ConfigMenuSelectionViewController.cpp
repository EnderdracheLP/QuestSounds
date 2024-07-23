#include "UI/ConfigMenuSelectionViewController.hpp"
#include "assets.hpp"
#include "logging.hpp"

#include "bsml/shared/BSML.hpp"

DEFINE_TYPE(QuestSounds::UI, ConfigMenuSelectionViewController);

namespace QuestSounds::UI {
    void ConfigMenuSelectionViewController::ctor() {
        // _menus = ListW<BSML::CustomCellInfo*>();
        getLogger().info("ctor");
        return;
    }

    void ConfigMenuSelectionViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        getLogger().info("DidActivate");
        if (firstActivation) {
            getLogger().info("DidActivate firstActivation");
            BSML::parse_and_construct(Assets::ConfigMenuSelectionViewController_bsml, get_transform(), this);
        }
        return;
    }

    void ConfigMenuSelectionViewController::set_selectCallback(std::function<void(int)> callback) {
        getLogger().info("Setting selectCallback");
        this->callback = callback;
    }

    void ConfigMenuSelectionViewController::MenuMusicButtonPressed() {
        getLogger().info("MenuMusicButtonPressed");
        if (callback) {
            callback(0);
        }
    }

    void ConfigMenuSelectionViewController::LobbyMusicButtonPressed() {
        getLogger().info("LobbyMusicButtonPressed");
        if (callback) {
            callback(1);
        }
    }

    void ConfigMenuSelectionViewController::MenuClickButtonPressed() {
        getLogger().info("MenuClickButtonPressed");
        if (callback) {
            callback(2);
        }
    }

    void ConfigMenuSelectionViewController::HitSoundButtonPressed() {
        getLogger().info("HitSoundButtonPressed");
        if (callback) {
            callback(3);
        }
    }

    void ConfigMenuSelectionViewController::BadHitSoundButtonPressed() {
        getLogger().info("BadHitSoundButtonPressed");
        if (callback) {
            callback(4);
        }
    }

    void ConfigMenuSelectionViewController::MissSoundButtonPressed() {
        getLogger().info("MissSoundButtonPressed");
        if (callback) {
            callback(5);
        }
    }

    void ConfigMenuSelectionViewController::LevelClearedButtonPressed() {
        getLogger().info("LevelClearedButtonPressed");
        if (callback) {
            callback(6);
        }
    }

    void ConfigMenuSelectionViewController::LevelFailedButtonPressed() {
        getLogger().info("LevelFailedButtonPressed");
        if (callback) {
            callback(7);
        }
    }

    void ConfigMenuSelectionViewController::FireworkButtonPressed() {
        getLogger().info("FireworkButtonPressed");
        if (callback) {
            callback(8);
        }
    }
}