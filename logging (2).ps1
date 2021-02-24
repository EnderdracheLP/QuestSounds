$timestamp = Get-Date -Format "MM-dd HH:mm:ss.fff"
adb logcat -T "$timestamp" main-modloader:W QuestHook[QuestSounds`|v0.2.0]:* AndroidRuntime:E *:S QuestHook[UtilsLogger`|v0.8.4]:E QuestHook[UtilsLogger`|v1.0.12]:E
        if ($args[0] -eq "--debug") {
            $timestamp = Get-Date -Format "MM-dd HH:mm:ss.fff"
            adb logcat -T "$timestamp" main-modloader:W QuestHook[TrickSaber`|v0.3.1]:* AndroidRuntime:E *:S QuestHook[UtilsLogger`|v0.8.4]:* QuestHook[UtilsLogger`|v1.0.12]:*
        }
echo "Exiting logging.ps1"