#pragma once
#include "main.hpp"
#include "audiocliploader.hpp"

namespace QuestSounds::AudioClips {

	extern audioClipLoader::loader	hitSoundLoader,     // hitSound
									badHitSoundLoader,  // badHitSound
									menuMusicLoader,    // menuMusic
									menuClickLoader,
									fireworkSoundLoader,
									levelClearedLoader,
									lobbyAmbienceLoader;    // Added for LobbyMusic
	extern Array<UnityEngine::AudioClip*>	* hitSoundArr,    // hitSoundArray
											* badHitSoundArr, // badHitSoundArray
											* menuClickArr,
											* fireworkSoundArr;
	extern void loadAudioClips();
}
