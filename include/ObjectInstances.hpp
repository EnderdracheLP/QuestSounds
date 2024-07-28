#pragma once
#include "main.hpp"
// #include "QSoundsFlowCoordinator.hpp"

#include "GlobalNamespace/SongPreviewPlayer.hpp"
#include "GlobalNamespace/BasicUIAudioManager.hpp"

// TODO: See if I can get rid of this header also possibly just forward delcare SongPreviewPlayer to make this more tidy

namespace QuestSounds::ObjectInstances {
	extern GlobalNamespace::SongPreviewPlayer* SPP;
	extern GlobalNamespace::BasicUIAudioManager* BUIAM;
}