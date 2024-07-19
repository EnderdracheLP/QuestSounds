#pragma once
//#include "main.hpp"
//#include "audiocliploader.hpp"
#include "Utils/AsyncAudioClipLoader.hpp"

namespace QuestSounds::AudioClips {
	//extern audioClipLoader::loader
	extern QuestSounds::Utils::AsyncAudioClipLoader	hitSoundLoader,     // hitSound
										badHitSoundLoader,  // badHitSound
										noteMissedSoundLoader,
										menuMusicLoader,    // menuMusic
										menuClickLoader,
										fireworkSoundLoader,
										levelClearedLoader,
										levelFailedLoader,
										lobbyAmbienceLoader;    // Added for LobbyMusic
	extern Array<UnityEngine::AudioClip*> *	hitSoundArr,    // hitSoundArray
										  * badHitSoundArr, // badHitSoundArray
										  * menuClickArr,
										  * fireworkSoundArr;

	extern Array<UnityEngine::AudioClip*> *	origMenuClickArr;
	extern void loadAudioClips();

	extern Array<UnityEngine::AudioClip*>* createAudioClipArray(QuestSounds::Utils::AsyncAudioClipLoader clipLoader, bool GetOriginalClip = false);
}
