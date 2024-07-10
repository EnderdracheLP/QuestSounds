Param(
    [String] $qmodname="QuestSounds",

    [Parameter(Mandatory=$false)]
    [Switch] $clean,

    [Parameter(Mandatory=$false)]
    [Switch] $help,

    [Parameter(Mandatory=$false)]
    [Switch] $package,

    [Parameter(Mandatory=$false)]
    [Switch] $dev,

    [Parameter(Mandatory=$false)]
    [Switch] $release = $false
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

if (($args.Count -eq 0 -or $dev -eq $true) -And $package -eq $false) {
    echo "Packaging QMod $qmodName"
    & $PSScriptRoot/build.ps1 -clean:$clean -release:$release

    if ($LASTEXITCODE -ne 0) {
        echo "Failed to build, exiting..."
        exit $LASTEXITCODE
    }
}

$qpmsharedJson = Get-Content "./qpm.shared.json" -Raw | ConvertFrom-Json

if ($package -eq $true) {
    $qmodName = "$($env:module_id)_$($env:version)"
    echo "Actions: Packaging QMod $qmodName"
} else {
    $qmodName += "_$($qpmsharedJson.config.info.version)"
}

$qmod = $qmodName + ".qmod"

# if ((-not ($clean.IsPresent)) -and (Test-Path $qmod))
# {
#     echo "Making Clean Qmod"
#     Move-Item $qmod $zip -Force
# }

qpm qmod zip -i ./build/ -i ./extern/libs/ $qmod

# Compress-Archive -Path $filelist -DestinationPath $zip -Update
# Move-Item $zip $qmod -Force

echo "Task Completed"