# Builds a .zip file for loading with BMBF
& $PSScriptRoot/build.ps1

$ModID = "QuestSounds"
$BSHook = "1_1_6"

if ($?) {
    Compress-Archive -Path "./libs/arm64-v8a/lib$ModID.so", "./libs/arm64-v8a/libbeatsaber-hook_$BSHook.so", -DestinationPath "./$ModID.zip" -Update
}
echo "Task Completed"
