#include "hooking.hpp"
#include "logging.hpp"
#include "Config.hpp"
using namespace QuestSounds;

// TODO: I'd like to get rid of this header
#include "AudioClips.hpp"
using namespace QuestSounds::AudioClips;

#include "ObjectInstances.hpp"

#include "GlobalNamespace/SongPreviewPlayer.hpp"
#include "GlobalNamespace/GameServerLobbyFlowCoordinator.hpp"
#include "GlobalNamespace/MultiplayerModeSelectionFlowCoordinator.hpp"
using namespace GlobalNamespace;

namespace QuestSounds::ObjectInstances {
    GlobalNamespace::SongPreviewPlayer* SPP;
}

MAKE_AUTO_HOOK_MATCH(SongPreviewPlayer_OnEnable, &SongPreviewPlayer::OnEnable, void, SongPreviewPlayer* self) {
    getLogger().info("is it true: %i", menuMusicLoader.loaded);
    QuestSounds::ObjectInstances::SPP = self;

    if (!menuMusicLoader.OriginalAudioSource) {
        menuMusicLoader.set_OriginalClip(self->_defaultAudioClip);
    }

    if (menuMusicLoader.loaded && Config.Sounds.MenuMusic.Active)
    {
        getLogger().debug("Overriding MenuMusic Audio");
        UnityEngine::AudioClip* audioClip = menuMusicLoader.getClip();
        if (audioClip != nullptr)
            self->_defaultAudioClip = audioClip;
    }
    else {
        getLogger().debug("Loading MenuMusic Audio normally: isLoaded='{}' isActive='{}'", menuMusicLoader.loaded, Config.Sounds.MenuMusic.Active);
        self->_defaultAudioClip = menuMusicLoader.get_OriginalClip();
    }
    SongPreviewPlayer_OnEnable(self);
}

MAKE_AUTO_HOOK_MATCH(GameServerLobbyFlowCoordinator_DidActivate, &GameServerLobbyFlowCoordinator::DidActivate, void, GameServerLobbyFlowCoordinator* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    getLogger().debug("GameServerLobbyFlowCoordinator_DidActivate");
    if (!lobbyAmbienceLoader.OriginalAudioSource) lobbyAmbienceLoader.set_OriginalClip(self->_ambienceAudioClip);

    if (lobbyAmbienceLoader.loaded && Config.Sounds.LobbyMusic.Active)
    {
        getLogger().debug("Overwriting LobbyAmbience Audio");
        self->_ambienceAudioClip = lobbyAmbienceLoader.getClip();
    }
    else {
        self->_ambienceAudioClip = lobbyAmbienceLoader.get_OriginalClip();
    }
    GameServerLobbyFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
}

MAKE_AUTO_HOOK_MATCH(GameServerLobbyFlowCoordinator_DidDeactivate, &GameServerLobbyFlowCoordinator::DidDeactivate, void, GameServerLobbyFlowCoordinator* self, bool removedFromHierarchy, bool screenSystemDisabling)
{
    getLogger().debug("GameServerLobbyFlowCoordinator_DidActivate");
    if (menuMusicLoader.loaded && Config.Sounds.MenuMusic.Active && removedFromHierarchy)
    {
        getLogger().debug("Switching LobbyMusic to MenuMusic Audio");
        self->_ambienceAudioClip = menuMusicLoader.getClip();
    }
    else {
        self->_ambienceAudioClip = menuMusicLoader.get_OriginalClip();
    }
    GameServerLobbyFlowCoordinator_DidDeactivate(self, removedFromHierarchy, screenSystemDisabling);
}

MAKE_AUTO_HOOK_MATCH(MultiplayerModeSelectionFlowCoordinator_DidActivate, &MultiplayerModeSelectionFlowCoordinator::DidActivate, void, MultiplayerModeSelectionFlowCoordinator* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    getLogger().debug("MultiplayerModeSelectionFlowCoordinator_DidActivate");
    if (menuMusicLoader.loaded && Config.Sounds.MenuMusic.Active)
    {
        getLogger().debug("Switching LobbyMusic to MenuMusic Audio");
        self->_ambienceAudioClip = menuMusicLoader.getClip();
    }
    else {
        self->_ambienceAudioClip = menuMusicLoader.get_OriginalClip();
    }
    MultiplayerModeSelectionFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling); // This has to be ran last, otherwise it will not work correctly
}

MAKE_AUTO_HOOK_MATCH(MultiplayerModeSelectionFlowCoordinator_DidDeactivate, &MultiplayerModeSelectionFlowCoordinator::DidDeactivate, void, MultiplayerModeSelectionFlowCoordinator* self, bool removedFromHierarchy, bool screenSystemDisabling)
{
    getLogger().debug("MultiplayerModeSelectionFlowCoordinator_DidDeactivate");
    if (menuMusicLoader.loaded && Config.Sounds.MenuMusic.Active && removedFromHierarchy)
    {
        self->_ambienceAudioClip = menuMusicLoader.getClip();
    }
    else {
        self->_ambienceAudioClip = menuMusicLoader.get_OriginalClip();
    }

    MultiplayerModeSelectionFlowCoordinator_DidDeactivate(self, removedFromHierarchy, screenSystemDisabling);
}