# Builds a .zip file for loading with BMBF
& $PSScriptRoot/build.ps1

if ($?) {
    Compress-Archive -Path "./libs/arm64-v8a/libQuestSounds.so", "./bmbfmod.json" -DestinationPath "./QuestSounds.zip" -Update
}
echo "Task Completed"
