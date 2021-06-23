Clear-Host
$YourModsToLog = "QuestHook[`|v]:* QuestHook[streamer-tools`|v0.1.0]:* QuestHook[QuestSounds`|v0.3.0]:* QuestHook[QuestSounds`|v1.0.0]:* QuestHook[clockmod`|v1.4.0]:*"
$OtherModsToLog = "QuestHook[chroma`|v1.0.0]:* QuestHook[moresongs`|v1.0.0]:* QuestHook[config-utils`|v0.2.2]:* QuestHook[mplighting`|v0.1.6]:* QuestHook[BeatTogether`|v1.1.1]:* QuestHook[multiquestensions`|v0.1.0]:*"
$LibsToLogError = "QuestHook[UtilsLogger`|v1.1.6]:E QuestHook[UtilsLogger`|v1.2.4]:E QuestHook[UtilsLogger`|v1.2.3]:E QuestHook[codegen`|v0.7.0]:E QuestHook[questui`|v0.5.0]:E"
$LibsToLogAll = "QuestHook[UtilsLogger`|v1.1.6]:* QuestHook[UtilsLogger`|v1.2.4]:* QuestHook[UtilsLogger`|v1.2.3]:* QuestHook[codegen`|v0.7.0]:* QuestHook[questui`|v0.5.0]:*"

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
            adb logcat -T "$timestamp" DEBUG:* main-modloader:W AndroidRuntime:* *:S $YourModsToLog $OtherModsToLog $LibsToLogAll | Tee-Object -FilePath $PSScriptRoot/logcat.log
        }

echo "Exiting logging.ps1"