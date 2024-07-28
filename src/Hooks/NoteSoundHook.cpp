#include "hooking.hpp"
#include "logging.hpp"
#include "Config.hpp"
using namespace QuestSounds;

// TODO: I'd like to get rid of this header
#include "AudioClips.hpp"
using namespace QuestSounds::AudioClips;

#include "GlobalNamespace/NoteCutSoundEffectManager.hpp"
#include "GlobalNamespace/NoteCutSoundEffect.hpp"
#include "GlobalNamespace/BombCutSoundEffectManager.hpp"
#include "GlobalNamespace/BeatmapObjectManager.hpp"
#include "GlobalNamespace/NoteController.hpp"
#include "GlobalNamespace/NoteData.hpp"
#include "GlobalNamespace/PauseMenuManager.hpp"
using namespace GlobalNamespace;


#pragma region HitSounds

MAKE_AUTO_HOOK_MATCH(NoteCutSoundEffectManager_Start, &NoteCutSoundEffectManager::Start, void, NoteCutSoundEffectManager* self) {
    if(hitSoundLoader.loaded && Config.Sounds.HitSound.Active)
    {
        hitSoundArr = createAudioClipArray(hitSoundLoader);
        self->_longCutEffectsAudioClips = hitSoundArr;
        self->_shortCutEffectsAudioClips = hitSoundArr;
        getLogger().debug("NoteCutSoundEffectManager_Start: Loaded hitSoundArray");
    }
    else {
        getLogger().debug("NoteCutSoundEffectManager_Start: Loading normally");
    }
    getLogger().debug("audioSamplesBeatAlignOffset was: {}", self->_audioSamplesBeatAlignOffset);
    if (Config.Sounds.HitSound.BeatOffset.has_value())
        self->_audioSamplesBeatAlignOffset = Config.Sounds.HitSound.BeatOffset.value();
    getLogger().debug("audioSamplesBeatAlignOffset changed to: {}", self->_audioSamplesBeatAlignOffset);
    NoteCutSoundEffectManager_Start(self);
    getLogger().debug("Beatalign offset is: {}", self->_beatAlignOffset);
}

#pragma endregion

#pragma region HitSoundsAndBadHitSounds

MAKE_AUTO_HOOK_MATCH(NoteCutSoundEffect_Awake, &NoteCutSoundEffect::Awake, void, NoteCutSoundEffect* self) {
    if (hitSoundLoader.loaded && Config.Sounds.HitSound.Active) {
        self->_goodCutVolume += Config.Sounds.HitSound.VolumeOffset.value_or(0.0f);
    }

    if(badHitSoundLoader.loaded && Config.Sounds.BadHitSound.Active)
    {
        badHitSoundArr = createAudioClipArray(badHitSoundLoader);
        self->_badCutSoundEffectAudioClips = badHitSoundArr;
        self->_badCutVolume += Config.Sounds.BadHitSound.VolumeOffset.value_or(0.0f);
    }
    NoteCutSoundEffect_Awake(self);
}

#pragma endregion

#pragma region NoteMissedSounds

MAKE_AUTO_HOOK_MATCH(BeatmapObjectManager_HandleNoteWasMissed, &BeatmapObjectManager::HandleNoteControllerNoteWasMissed, void, BeatmapObjectManager* self, NoteController* noteController) {
    BeatmapObjectManager_HandleNoteWasMissed(self, noteController);
    if (noteMissedSoundLoader.loaded && 
        Config.Sounds.NoteMissedSound.Active &&
        noteController->get_noteData()->get_scoringType() != NoteData::ScoringType::Ignore &&
        noteController->get_noteData()->get_gameplayType() != NoteData::GameplayType::Bomb) {
        noteMissedSoundLoader.audioSource->set_volume(0.5f + Config.Sounds.NoteMissedSound.VolumeOffset.value_or(0.0f));
        noteMissedSoundLoader.audioSource->Play();
    }
}

MAKE_AUTO_HOOK_MATCH(PauseMenuManager_MenuButtonPressed, &PauseMenuManager::MenuButtonPressed, void, PauseMenuManager* self) {
    if (noteMissedSoundLoader.loaded &&
        Config.Sounds.NoteMissedSound.Active) {
        if (noteMissedSoundLoader.audioSource) noteMissedSoundLoader.audioSource->Stop();
    }
    PauseMenuManager_MenuButtonPressed(self);
}

#pragma endregion

#pragma region BombHitSounds

MAKE_AUTO_HOOK_MATCH(BombCutSoundEffectManager_Start, &BombCutSoundEffectManager::Start, void, BombCutSoundEffectManager* self) {
    if (bombExplosionSoundLoader.loaded && Config.Sounds.BombExplosionSound.Active) {
        bombExplosionSoundLoaderArr = createAudioClipArray(bombExplosionSoundLoader);
        self->_bombExplosionAudioClips = bombExplosionSoundLoaderArr;
        self->_volume += Config.Sounds.BombExplosionSound.VolumeOffset.value_or(0.0f);
    }
    BombCutSoundEffectManager_Start(self);
}

#pragma endregion