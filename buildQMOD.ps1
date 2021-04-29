# Builds a .qmod file for loading with QP
if ($args.Count -eq 0) {
$ModID = "QuestSounds"
$BSHook = "1_3_3"
$VERSION = "0.3.0"
$BSVersion = "1.15.0"
echo "Compiling Mod"
& $PSScriptRoot/build.ps1
}

    $modjson = "$PSScriptRoot/mod.json"
    Copy-Item "./mod_Template.json" "./mod.json" -Force
# TODO: Get the below working with Github Actions variables.
if ($args[0] -eq "--package") {
    $ModID = $env:module_id
    $BSHook = $env:bs_hook
    $BSVersion = $env:BS_Version
echo "Actions: Packaging QMod with ModID: $ModID and BS-Hook version: $BSHook for Beat Saber $BSVersion"
    (Get-Content "./mod.json").replace('{VERSION_NUMBER_PLACEHOLDER}', "$env:version") | Set-Content "./mod.json"
    (Get-Content "./mod.json").replace('{BS_Hook}', "$BSHook") | Set-Content "./mod.json"
    (Get-Content $modjson).replace('{BS_Version}', "$BSVersion") | Set-Content "./mod.json"
    Compress-Archive -Path "./libs/arm64-v8a/lib$ModID.so", "./libs/arm64-v8a/libbeatsaber-hook_$BSHook.so", ".\mod.json" -DestinationPath "./Temp$ModID.zip" -Update
    Move-Item "./Temp$ModID.zip" "./$ModID.qmod" -Force
}
if ($? -And $args.Count -eq 0) {
echo "Packaging QMod with ModID: $ModID for Beat Saber $BSVersion"
    (Get-Content $modjson).replace('{VERSION_NUMBER_PLACEHOLDER}', "$VERSION") | Set-Content $modjson
    (Get-Content $modjson).replace('{BS_Hook}', "$BSHook") | Set-Content $modjson
    (Get-Content $modjson).replace('{BS_Version}', "$BSVersion") | Set-Content $modjson
    Compress-Archive -Path "./libs/arm64-v8a/lib$ModID.so", "./libs/arm64-v8a/libbeatsaber-hook_$BSHook.so", ".\mod.json" -DestinationPath "./Temp$ModID.zip" -Update
    Move-Item "./Temp$ModID.zip" "./$ModID.qmod" -Force
}
echo "Task Completed"