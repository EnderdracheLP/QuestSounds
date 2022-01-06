# Builds a .qmod file for loading with QP
if ($args.Count -eq 0 -or $args[0] -eq "--dev") {
    $ModID = "QuestSounds"
    $BSHook = "3_4_4"
    $VERSION = "1.0.4"
    if ($args[0] -eq "--dev") {
        $VERSION += "-Dev"
    }
    $BS_Version = "1.19.0"
    echo "Compiling Mod"
    if ($args[0] -ne "--dev") {
        & $PSScriptRoot/build.ps1 --release
    }
    else {
        & $PSScriptRoot/build.ps1
    }
}

    $modjson = "$PSScriptRoot/mod.json"
    Copy-Item "./mod_Template.json" "./mod.json" -Force
# TODO: Get the below working with Github Actions variables.
if ($args[0] -eq "--package") {
    $ModID = $env:module_id
    $BSHook = $env:bs_hook
    $BS_Version = $env:BSVersion
    $VERSION = $env:VERSION
echo "Actions: Packaging QMod with ModID: $ModID and BS-Hook version: $BSHook"
    (Get-Content "./mod.json").replace('{VERSION_NUMBER_PLACEHOLDER}', "$VERSION") | Set-Content "./mod.json"
    (Get-Content "./mod.json").replace('{BS_Hook}', "$BSHook") | Set-Content "./mod.json"
    (Get-Content "./mod.json").replace('{BS_Version}', "$BS_Version") | Set-Content "./mod.json"
    Compress-Archive -Path "./libs/arm64-v8a/lib$ModID.so", "./libs/arm64-v8a/libbeatsaber-hook_$BSHook.so", ".\Cover.jpg", ".\mod.json" -DestinationPath "./Temp$ModID.zip" -Update
    Move-Item "./Temp$ModID.zip" "./$ModID.qmod" -Force
}
if ($? -And $args.Count -eq 0  -or $args[0] -eq "--dev") {
echo "Packaging QMod with ModID: $ModID for GameVersion: "
    (Get-Content "./mod.json").replace('{VERSION_NUMBER_PLACEHOLDER}', "$VERSION") | Set-Content "./mod.json"
    (Get-Content "./mod.json").replace('{BS_Hook}', "$BSHook") | Set-Content "./mod.json"
    (Get-Content "./mod.json").replace('{BS_Version}', "$BS_Version") | Set-Content "./mod.json"
    Compress-Archive -Path "./libs/arm64-v8a/lib$ModID.so", "./libs/arm64-v8a/libbeatsaber-hook_$BSHook.so", ".\Cover.jpg", ".\mod.json" -DestinationPath "./Temp$ModID.zip" -Update
    Move-Item "./Temp$ModID.zip" "./$ModID.qmod" -Force
}
echo "Task Completed"