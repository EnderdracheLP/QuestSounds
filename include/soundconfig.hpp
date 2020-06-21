#define RAPIDJSON_HAS_STDSTRING 1
#include "../extern/beatsaber-hook/shared/utils/utils.h"
#include "../extern/beatsaber-hook/shared/config/config-utils.hpp"

Configuration& getConfig();



extern void SaveConfig();
extern bool LoadConfig();