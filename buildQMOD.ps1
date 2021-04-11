# Builds a .qmod file for loading with QP
if ($args.Count -eq 0) {
$ModID = "QuestSounds"
$BSHook = "1_2_6"
echo "Compiling Mod"
& $PSScriptRoot/build.ps1
}

# TODO: Get the below working with Github Actions variables.
if ($args[0] -eq "--package") {
echo "Actions: Packaging QMod with ModID: $ModID"
    $ModID = $env:module_id
    $BSHook = $env:bs_hook
    Compress-Archive -Path "./libs/arm64-v8a/lib$ModID.so", "./libs/arm64-v8a/libbeatsaber-hook_$BSHook.so", ".\mod.json" -DestinationPath "./Temp$ModID.zip" -Update
    Move-Item "./Temp$ModID.zip" "./$ModID.qmod" -Force
}
if ($?) {
echo "Packaging QMod with ModID: $ModID"
    Compress-Archive -Path "./libs/arm64-v8a/lib$ModID.so", "./libs/arm64-v8a/libbeatsaber-hook_$BSHook.so", ".\mod.json" -DestinationPath "./Temp$ModID.zip" -Update
    Move-Item "./Temp$ModID.zip" "./$ModID.qmod" -Force
}
echo "Task Completed"