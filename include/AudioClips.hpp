#pragma once

#include "Utils/AsyncAudioClipLoader.hpp"

namespace QuestSounds::AudioClips {
	
	extern QuestSounds::Utils::AsyncAudioClipLoader	hitSoundLoader,     // hitSound
										badHitSoundLoader,  // badHitSound
										noteMissedSoundLoader,
										bombExplosionSoundLoader,
										menuMusicLoader,    // menuMusic
										menuClickLoader,
										fireworkSoundLoader,
										levelClearedLoader,
										levelFailedLoader,
										lobbyAmbienceLoader;    // Added for LobbyMusic

	extern ::ArrayW<::UnityW<::UnityEngine::AudioClip>>	hitSoundArr,    // hitSoundArray
														badHitSoundArr, // badHitSoundArray
														bombExplosionSoundLoaderArr,
														menuClickArr,
														fireworkSoundArr;

	extern ::ArrayW<::UnityW<::UnityEngine::AudioClip>>	origMenuClickArr;
	extern void loadAudioClips();

	extern ::ArrayW<::UnityW<::UnityEngine::AudioClip>> createAudioClipArray(QuestSounds::Utils::AsyncAudioClipLoader clipLoader, bool GetOriginalClip = false);
}
