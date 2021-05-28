$NDKPath = Get-Content $PSScriptRoot/ndkpath.txt
if ($args.Count -eq 0) {
$ModID = "QuestSounds"
$VERSION = "0.3.0-InDev"
$BSHook = "1_3_5"
$codegen_ver = "0_9_0"
}

if ($args[0] -eq "--actions") {
    $ModID = $env:module_id
    $BSHook = $env:bs_hook
    $VERSION = $env:version
    $codegen_ver = $env:codegen
}
$BS_HOOK_CHECK = Test-Path -Path $PSScriptRoot/extern/libbeatsaber-hook_$BSHook.so
$CG_CHECK = Test-Path -Path $PSScriptRoot/extern/libcodegen_$codegen_ver.so
if ($BS_HOOK_CHECK -And $CG_CHECK) {
    echo "Checked mod paths valid"
    echo "Building mod with ModID: $ModID version: $VERSION, BS-Hook version: $BSHook"
    Copy-Item "./Android_Template.mk" "./Android.mk" -Force
    (Get-Content "./Android.mk").replace('{BS_Hook}',   "$BSHook")        | Set-Content "./Android.mk"
    (Get-Content "./Android.mk").replace('{VERSION}',   "$VERSION")       | Set-Content "./Android.mk"
    (Get-Content "./Android.mk").replace('{CG_VER}',    "$codegen_ver")   | Set-Content "./Android.mk"
}
else {
    echo "Checked mod paths invalid"
    echo "Skipping setting mod versions in Android.mk"
    (Get-Content "./Android.mk").replace('{VERSION}',   "$VERSION")       | Set-Content "./Android.mk"
    # TODO: Add autodetect for core-mod versions

    echo "Building mod with ModID: $ModID version: $VERSION, BS-Hook UNKNOWN"
}

$buildScript = "$NDKPath/build/ndk-build"
if (-not ($PSVersionTable.PSEdition -eq "Core")) {
    $buildScript += ".cmd"
}

& $buildScript NDK_PROJECT_PATH=$PSScriptRoot APP_BUILD_SCRIPT=$PSScriptRoot/Android.mk NDK_APPLICATION_MK=$PSScriptRoot/Application.mk

echo Done
