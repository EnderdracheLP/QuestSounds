$NDKPath = Get-Content $PSScriptRoot/ndkpath.txt
if ($args.Count -eq 0) {
$ModID = "QuestSounds"
$BSHook = "1_3_3"
$VERSION = "0.3.0"
}

if ($args[0] -eq "--actions") {
    $ModID = $env:module_id
    $BSHook = $env:bs_hook
    $VERSION = $env:version
}
echo "Building mod with ModID: $ModID version: $VERSION, BS-Hook version: $BSHook"
Copy-Item "./Android_Template.mk" "./Android.mk" -Force
(Get-Content "./Android.mk").replace('{BS_Hook}', "$BSHook") | Set-Content "./Android.mk"
(Get-Content "./Android.mk").replace('{VERSION}', "$VERSION") | Set-Content "./Android.mk"

$buildScript = "$NDKPath/build/ndk-build"
if (-not ($PSVersionTable.PSEdition -eq "Core")) {
    $buildScript += ".cmd"
}

& $buildScript NDK_PROJECT_PATH=$PSScriptRoot APP_BUILD_SCRIPT=$PSScriptRoot/Android.mk NDK_APPLICATION_MK=$PSScriptRoot/Application.mk

echo Done
