# Builds a .zip file for loading with BMBF
if ($args.Count -eq 0) {
echo "Compiling Mod"
& $PSScriptRoot/build.ps1
}

$ModID = "QuestSounds"
$BSHook = "1_2_3"

if ($?) {
echo "Packaging BMBFMod with ModID: $ModID and BSHook Version: $BSHook"
    Compress-Archive -Path "./libs/arm64-v8a/lib$ModID.so", "./libs/arm64-v8a/libbeatsaber-hook_$BSHook.so", ".\bmbfmod.json" -DestinationPath "./$ModID.zip" -Update
}
echo "Task Completed"
