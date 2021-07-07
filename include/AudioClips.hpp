#pragma once
//#include "main.hpp"
//#include "audiocliploader.hpp"
#include "Utils/AsyncAudiocliploader.hpp"

namespace QuestSounds::AudioClips {
	//extern audioClipLoader::loader
	extern AsyncAudioClipLoader::loader	hitSoundLoader,     // hitSound
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
	extern Array<UnityEngine::AudioClip*>	*origFireworkSoundArr,
											*origMenuClickArr;
	extern void loadAudioClips();

	extern Array<UnityEngine::AudioClip*>* createAudioClipArray(AsyncAudioClipLoader::loader clipLoader, bool GetOriginalClip = false);
}
