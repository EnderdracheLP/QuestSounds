# Builds a .qmod file for loading with QP
if ($args.Count -eq 0) {
echo "Compiling Mod"
& $PSScriptRoot/build.ps1
}

$ModID = "QuestSounds"

if ($?) {
echo "Packaging QMod with ModID: $ModID"
    Compress-Archive -Path "./libs/arm64-v8a/lib$ModID.so", ".\mod.json" -DestinationPath "./Temp$ModID.zip" -Update
    Remove-Item "./$ModID.qmod"
    Rename-Item "./Temp$ModID.zip" "./$ModID.qmod"
}
echo "Task Completed"