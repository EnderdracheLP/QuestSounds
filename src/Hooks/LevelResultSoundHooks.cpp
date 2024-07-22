#include "hooking.hpp"
#include "logging.hpp"
#include "Config.hpp"
using namespace QuestSounds;

// TODO: I'd like to get rid of this header
#include "AudioClips.hpp"
using namespace QuestSounds::AudioClips;

#include "GlobalNamespace/ResultsViewController.hpp"
#include "GlobalNamespace/LevelCompletionResults.hpp"
#include "GlobalNamespace/SongPreviewPlayer.hpp"
#include "GlobalNamespace/FireworkItemController.hpp"
using namespace GlobalNamespace;

#pragma region LevelResultSound
MAKE_AUTO_HOOK_MATCH(ResultsViewController_DidActivate, &ResultsViewController::DidActivate, void, ResultsViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (firstActivation && addedToHierarchy && !levelClearedLoader.OriginalAudioSource) {
        levelClearedLoader.set_OriginalClip(self->_levelClearedAudioClip);
    }
    if (self->_levelCompletionResults->levelEndStateType == LevelCompletionResults::LevelEndStateType::Failed) {
        self->_songPreviewPlayer->StopAllCoroutines();
        if (levelFailedLoader.loaded && addedToHierarchy && Config.Sounds.LevelFailed.Active) {
            UnityEngine::AudioClip* FailedSound = levelFailedLoader.getClip();
            float length = FailedSound->get_length();
            getLogger().debug("Duration of LevelFailed Sound: {}", length);
            if (length > 7.0f) {
                getLogger().info("Long LevelFailedSound");
                self->_songPreviewPlayer->CrossfadeTo(FailedSound, -4.0f, 0.0f, length, nullptr);
            }
            else {
                getLogger().info("Short LevelFailedSound");
                self->_songPreviewPlayer->FadeOut(0.1f);
                self->_songPreviewPlayer->_fadeSpeed = self->_songPreviewPlayer->_fadeInSpeed;
                getLogger().debug("volume: {}", self->_songPreviewPlayer->_volume);
                getLogger().debug("AmbientVolume: {}", self->_songPreviewPlayer->_ambientVolumeScale);
                getLogger().debug("Set Volume: {}", self->_songPreviewPlayer->_volume * self->_songPreviewPlayer->_ambientVolumeScale);

                levelFailedLoader.audioSource->set_volume(self->_songPreviewPlayer->_volume * self->_songPreviewPlayer->_ambientVolumeScale);
                self->_songPreviewPlayer->StartCoroutine(self->_songPreviewPlayer->CrossFadeAfterDelayCoroutine(length - 1.2f));
                levelFailedLoader.audioSource->Play();
            }
        }
    }
    else {
        if (levelClearedLoader.loaded && addedToHierarchy && Config.Sounds.LevelCleared.Active)
        {
            UnityEngine::AudioClip* audioClip = levelClearedLoader.getClip();
            self->_levelClearedAudioClip = audioClip;
        }
        else {
            self->_levelClearedAudioClip = levelClearedLoader.get_OriginalClip();
        }
    }
    ResultsViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
}

MAKE_AUTO_HOOK_MATCH(ResultsViewController_RestartButtonPressed, &ResultsViewController::RestartButtonPressed, void, ResultsViewController* self) {
    if (levelFailedLoader.loaded && levelFailedLoader.audioSource->get_isPlaying()) {
        levelFailedLoader.audioSource->Stop();
    }
    ResultsViewController_RestartButtonPressed(self);
}
#pragma endregion

#pragma region FireworkSound
// Replacing the function here, as replacing the AudioClips proves to be difficult
MAKE_AUTO_HOOK_MATCH(FireworkItemController_PlayExplosionSound, &FireworkItemController::PlayExplosionSound, void, FireworkItemController* self) {
    if (fireworkSoundLoader.loaded && Config.Sounds.Firework.Active) {
        self->_audioSource->set_clip(fireworkSoundLoader.getClip());
        float pitch = 1.2f + (((float)rand()) / (float)RAND_MAX) * (0.8f - 1.2f);
        self->_audioSource->set_pitch(pitch);
        self->_audioSource->Play();
    }
    else FireworkItemController_PlayExplosionSound(self);
}
#pragma endregion