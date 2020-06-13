#include <dlfcn.h>
#include "../extern/beatsaber-hook/shared/utils/utils.h"
#include "../extern/beatsaber-hook/shared/utils/logging.hpp"
#include "../extern/beatsaber-hook/include/modloader.hpp"
#include "../extern/beatsaber-hook/shared/utils/typedefs.h"
#include "../extern/beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "../extern/beatsaber-hook/shared/utils/il2cpp-functions.hpp"

static const Logger *logger;


extern "C" void setup(ModInfo &info)
{
    info.id = "QuestSounds";
    info.version = "0.1.0";
    static std::unique_ptr<const Logger> ptr(new Logger(info));
    logger = ptr.get();
    logger->info("Completed setup!");
    logger->info("Modloader name: %s", Modloader::getInfo().name.c_str());
}

// This function is called when the mod is loaded for the first time, immediately after il2cpp_init.
extern "C" void load()
{
    logger->debug("Installing QuestSounds!");
    logger->debug("Installed QuestSounds!");
}
