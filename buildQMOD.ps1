Param(
    [String] $qmodname="QuestSounds",

    [Parameter(Mandatory=$false)]
    [Switch] $clean,

    [Parameter(Mandatory=$false)]
    [Switch] $help,

    [Parameter(Mandatory=$false)]
    [Switch] $package,

    [Parameter(Mandatory=$false)]
    [Switch] $dev
)

# Builds a .qmod file for loading with QP or BMBF



if ($help -eq $true) {
    echo "`"BuildQmod <qmodName>`" - Copiles your mod into a `".so`" or a `".a`" library"
    echo "`n-- Parameters --`n"
    echo "qmodName `t The file name of your qmod"

    echo "`n-- Arguments --`n"

    echo "-Clean `t`t Performs a clean build on both your library and the qmod"

    exit
}

if ($qmodName -eq "")
{
    echo "Give a proper qmod name and try again"
    exit
}

if ($package -eq $true) {
    $qmodName = "$($env:module_id)_$($env:version)"
echo "Actions: Packaging QMod $qmodName"
    # Compress-Archive -Path "./libs/arm64-v8a/lib$ModID.so", "./libs/arm64-v8a/libbeatsaber-hook_$BSHook.so", ".\Cover.jpg", ".\mod.json" -DestinationPath "./Temp$ModID.zip" -Update
    # Move-Item "./Temp$ModID.zip" "./$ModID.qmod" -Force
} else {
    $qmodName += "_$($env:version)"
}
if (($args.Count -eq 0 -or $dev -eq $true) -And $package -eq $false) {
echo "Packaging QMod $qmodName"
    & $PSScriptRoot/build.ps1 -clean:$clean

    if ($LASTEXITCODE -ne 0) {
        echo "Failed to build, exiting..."
        exit $LASTEXITCODE
    }

    qpm-rust qmod build
    # Compress-Archive -Path "./libs/arm64-v8a/lib$ModID.so", "./libs/arm64-v8a/libbeatsaber-hook_$BSHook.so", ".\Cover.jpg", ".\mod.json" -DestinationPath "./Temp$ModID.zip" -Update
    # Move-Item "./Temp$ModID.zip" "./$ModID.qmod" -Force
}

echo "Creating qmod from mod.json"

$mod = "./mod.json"
$modJson = Get-Content $mod -Raw | ConvertFrom-Json

$filelist = @($mod)

$cover = "./" + $modJson.coverImage
if ((-not ($cover -eq "./")) -and (Test-Path $cover))
{ 
    $filelist += ,$cover
} else {
    echo "No cover Image found"
}

foreach ($mod in $modJson.modFiles)
{
    $path = "./build/" + $mod
    if (-not (Test-Path $path))
    {
        $path = "./extern/libs/" + $mod
    }
    $filelist += $path
}

foreach ($lib in $modJson.libraryFiles)
{
    $path = "./extern/libs/" + $lib
    if (-not (Test-Path $path))
    {
        $path = "./build/" + $lib
    }
    $filelist += $path
}

$zip = $qmodName + ".zip"
$qmod = $qmodName + ".qmod"

if ((-not ($clean.IsPresent)) -and (Test-Path $qmod))
{
    echo "Making Clean Qmod"
    Move-Item $qmod $zip -Force
}

Compress-Archive -Path $filelist -DestinationPath $zip -Update
Move-Item $zip $qmod -Force

echo "Task Completed"