#include "UI/CustomSoundFileCell.hpp"

DEFINE_TYPE(QuestSounds::UI, CustomSoundFileCell);

namespace QuestSounds::UI {
    CustomSoundFileCell* CustomSoundFileCell::construct(StringW text, StringW subText, std::filesystem::path filePath, UnityEngine::Sprite* icon) {
        CustomSoundFileCell* cell = CustomSoundFileCell::New_ctor();
        cell->FilePath = filePath;
        cell->text = text;
        cell->subText = subText;
        return cell;
    }

    CustomSoundFileCell* CustomSoundFileCell::construct(StringW text, std::filesystem::path filePath, UnityEngine::Sprite* icon) {
        return CustomSoundFileCell::construct(text, StringW(), filePath, icon);
    }
}