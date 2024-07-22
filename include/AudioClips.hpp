#pragma once

#include "Utils/AsyncAudioClipLoader.hpp"

namespace QuestSounds::AudioClips {
	
	extern QuestSounds::Utils::AsyncAudioClipLoader	hitSoundLoader,     // hitSound
										badHitSoundLoader,  // badHitSound
										noteMissedSoundLoader,
										menuMusicLoader,    // menuMusic
										menuClickLoader,
										fireworkSoundLoader,
										levelClearedLoader,
										levelFailedLoader,
										lobbyAmbienceLoader;    // Added for LobbyMusic

	extern ::ArrayW<::UnityW<::UnityEngine::AudioClip>>	hitSoundArr,    // hitSoundArray
														badHitSoundArr, // badHitSoundArray
														menuClickArr,
														fireworkSoundArr;

	extern ::ArrayW<::UnityW<::UnityEngine::AudioClip>>	origMenuClickArr;
	extern void loadAudioClips();

	extern ::ArrayW<::UnityW<::UnityEngine::AudioClip>> createAudioClipArray(QuestSounds::Utils::AsyncAudioClipLoader clipLoader, bool GetOriginalClip = false);
}
