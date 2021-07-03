Clear-Host
$NDKPath = Get-Content $PSScriptRoot/ndkpath.txt
$ndkstackScript = "$NDKPath/ndk-stack"

if (-not ($PSVersionTable.PSEdition -eq "Core")) {
    $ndkstackScript += ".cmd"
}

if ($args[0] -eq "--stack") {
$timestamp = Get-Content $PSScriptRoot/log_timestamp
#& $ndkstackScript -sym ./obj/local/arm64-v8a/ -dump ./logcat.log | Tee-Object -FilePath $PSScriptRoot/log_unstripped.log
adb logcat -b crash -d -T "$timestamp" | & $ndkstackScript -sym ./obj/local/arm64-v8a/ | Tee-Object -FilePath $PSScriptRoot/log_unstripped.log
break Script
}

if ($args[0] -eq "--stack-all") {
#& $ndkstackScript -sym ./obj/local/arm64-v8a/ -dump ./logcat.log | Tee-Object -FilePath $PSScriptRoot/log_unstripped.log
adb logcat -b crash -d | & $ndkstackScript -sym ./obj/local/arm64-v8a/ | Tee-Object -FilePath $PSScriptRoot/log_unstripped.log
break Script
}

$YourModsToLog = "QuestHook[`|v]:* QuestHook[streamer-tools`|v0.1.0]:* QuestHook[QuestSounds`|v0.3.0]:* QuestHook[QuestSounds`|v1.0.0]:* QuestHook[clockmod`|v1.4.0]:*"
$OtherModsToLog = "QuestHook[chroma`|v1.0.0]:* QuestHook[moresongs`|v1.0.0]:* QuestHook[config-utils`|v0.2.2]:* QuestHook[mplighting`|v0.1.6]:* QuestHook[BeatTogether`|v1.1.1]:* QuestHook[multiquestensions`|v0.1.0]:*"
$LibsToLogError = "QuestHook[UtilsLogger`|v1.1.6]:E QuestHook[UtilsLogger`|v1.2.4]:E QuestHook[UtilsLogger`|v1.3.5]:E QuestHook[codegen`|v0.9.0]:E QuestHook[questui`|v0.7.0]:E"
$LibsToLogAll = "QuestHook[UtilsLogger`|v1.1.6]:* QuestHook[UtilsLogger`|v1.2.4]:* QuestHook[UtilsLogger`|v1.3.5]:* QuestHook[codegen`|v0.9.0]:* QuestHook[questui`|v0.7.0]:*"

#echo "Logging following mods: $YourModsToLog" "Following Other Mods: $OtherModsToLog" "Following Libs on Error: $LibsToLogError" "Following Libs: $LibsToLogAll"
#Wait-Event -Timeout 5
#Clear-Host
if ($args.Count -eq 0) {
$timestamp = Get-Date -Format "MM-dd HH:mm:ss.fff"
adb logcat -T "$timestamp" main-modloader:W AndroidRuntime:E *:S $YourModsToLog $OtherModsToLog $LibsToLogError
}
        if ($args[0] -eq "--debug") {
            echo "Debug Logging"
            $timestamp = Get-Date -Format "MM-dd HH:mm:ss.fff"
            adb logcat -T "$timestamp" main-modloader:W  DEBUG:* AndroidRuntime:* *:S $YourModsToLog $OtherModsToLog $LibsToLogAll
        }
        if ($args[0] -eq "--insane") {
            echo "INSANE Logging"
            $timestamp = Get-Date -Format "MM-dd HH:mm:ss.fff"
            adb logcat -T "$timestamp" main-modloader:* DEBUG:* AndroidRuntime:* *:S $YourModsToLog $OtherModsToLog $LibsToLogAll
        }
        if ($args[0] -eq "--file") {
            echo "Running with parameter '--file'"
            $timestamp = Get-Date -Format "MM-dd HH:mm:ss.fff"
            $AllModsToLog = $YourModsToLog + $OtherModsToLog + $LibsToLogAll
            $timestamp > log_timestamp
            $logger = Start-Job -Arg "$timestamp", $PSScriptRoot -scriptblock {
                param($timestamp, $Path)
                adb logcat -v color -T "$timestamp" main-modloader:W libmodloader:W libmain:W AndroidRuntime:E QuestHook[`|v]:* QuestHook[streamer-tools`|v0.1.0]:* QuestHook[QuestSounds`|v0.3.0]:* QuestHook[QuestSounds`|v1.0.0]:* QuestHook[clockmod`|v1.4.0]:* *:S | Tee-Object -FilePath "$Path/logcat.log"
            }
            [console]::TreatControlCAsInput = $true
            while ($true)
            {
                $buffer = Receive-Job -Job $logger
                $PermaBuffer += $buffer
                echo $buffer
                if ([console]::KeyAvailable)
                {
                    $key = [system.console]::readkey($true)
                    if (($key.modifiers -band [consolemodifiers]"control") -and ($key.key -eq "C"))
                    {
                        Add-Type -AssemblyName System.Windows.Forms
                        if ([System.Windows.Forms.MessageBox]::Show("Are you sure you want to exit?", "Exit Script?", [System.Windows.Forms.MessageBoxButtons]::YesNo) -eq "Yes")
                        {
                            echo "Terminating logcat and getting crash dumps..."
                            get-job | remove-job -Force
                            adb logcat -b crash -d -T "$timestamp" | & $ndkstackScript -sym ./obj/local/arm64-v8a/ | Tee-Object -FilePath $PSScriptRoot/log_unstripped.log
                            & Pause
                            echo "Exiting logging.ps1"
                            break
                        }
                    }
                }
            }
        }
echo "Exiting logging.ps1"