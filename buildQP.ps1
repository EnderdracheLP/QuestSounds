# Builds a .qmod file for loading with QP
& $PSScriptRoot/build.ps1

$ModID = "QuestSounds"
$BSHook = "1_1_6"

if ($?) {
    Compress-Archive -Path "./libs/arm64-v8a/lib$ModID.so", "./libs/arm64-v8a/libbeatsaber-hook_$BSHook.so", ".\mod.json" -DestinationPath "./$ModID.zip" -Update
    Remove-Item "./$ModID.qmod"
    Rename-Item "./$ModID.zip" "./$ModID.qmod"
}
echo "Task Completed"