$NDKPath = Get-Content $PSScriptRoot/ndkpath.txt
$PARAM = ""
for ($i = 0; $i -le $args.Length-1; $i++) {
echo "Arg $($i) is $($args[$i])"
    if ($args[$i] -eq "--actions") { $actions = $true }
    elseif ($args[$i] -eq "--debug") { $PARAM += "-DDEBUG_BUILD=1 " }
    elseif ($args[$i] -eq "--1_13_2") { $1_13_2build = $true }
    elseif ($args[$i] -eq "--trace") { $trace = $true }
    elseif ($args[$i] -eq "--dry-run") { $dryrun = $true }
    elseif ($args[$i] -eq "--release") { $release = $true }
    else { $PARAM += $args[$i] }
}
if ($args.Count -eq 0 -or $actions -ne $true) {
$ModID = "QuestSounds"
$BSHook = "3_4_4"
$VERSION = "1.0.4"
if ($release -ne $true) {
    $VERSION += "-Dev"
}
$codegen_ver = "0_13_0"
}


if ($actions -eq $true) {
    $ModID = $env:module_id
    $BSHook = $env:bs_hook
    $VERSION = $env:version
    $codegen_ver = $env:codegen
    if ($env:BSVersion -eq "1.13.2") { $1_13_2build = $true }
}
if ($1_13_2build -eq $true) {
echo "Making 1.13.2 Build!"
    $PARAM += " -DBS__1_13_2=1 "
    if (!$actions) {
        $BSHook = "1_1_5"
        $codegen_ver = "0_6_2"
    }
} else { $PARAM += " -DBS__1_16=1 " }
echo "Building mod with ModID: $ModID version: $VERSION, BS-Hook version: $BSHook"
Copy-Item "./Android_Template.mk" "./Android.mk" -Force
(Get-Content "./Android.mk").replace('{BS_Hook}',   "$BSHook")        | Set-Content "./Android.mk"
(Get-Content "./Android.mk").replace('{CG_VER}',    "$codegen_ver")   | Set-Content "./Android.mk"

if ($debug) {
echo "Building Debug Build!"
    $PARAM += " -DDEBUG_BUILD=1 "
}
(Get-Content "./Android.mk").replace('{DEBUG_PARAMS}',    $PARAM)   | Set-Content "./Android.mk"

$buildScript = "$NDKPath/build/ndk-build"
if (-not ($PSVersionTable.PSEdition -eq "Core")) {
    $buildScript += ".cmd"
}
if ($trace -eq $true) {
& $buildScript NDK_PROJECT_PATH=$PSScriptRoot APP_BUILD_SCRIPT=$PSScriptRoot/Android.mk NDK_APPLICATION_MK=$PSScriptRoot/Application.mk -j 6 --output-sync=none VERSION=$VERSION --trace
} elseif ($dryrun -eq $true) {
& $buildScript NDK_PROJECT_PATH=$PSScriptRoot APP_BUILD_SCRIPT=$PSScriptRoot/Android.mk NDK_APPLICATION_MK=$PSScriptRoot/Application.mk -j 6 --output-sync=none VERSION=$VERSION -n
} else {
& $buildScript NDK_PROJECT_PATH=$PSScriptRoot APP_BUILD_SCRIPT=$PSScriptRoot/Android.mk NDK_APPLICATION_MK=$PSScriptRoot/Application.mk -j 6 --output-sync=none VERSION=$VERSION
}

echo Done