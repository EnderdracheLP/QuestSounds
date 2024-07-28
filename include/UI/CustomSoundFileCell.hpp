#include "bsml/shared/BSML/Components/CustomListTableData.hpp"

DECLARE_CLASS_CUSTOM(QuestSounds::UI, CustomSoundFileCell, BSML::CustomCellInfo,
public:
    std::filesystem::path FilePath;

    DECLARE_DEFAULT_CTOR();
public:
    static CustomSoundFileCell* construct(StringW text, StringW subText, std::filesystem::path filePath, UnityEngine::Sprite* icon = nullptr);
    static CustomSoundFileCell* construct(StringW text, std::filesystem::path filePath, UnityEngine::Sprite* icon = nullptr);
)