$timestamp = Get-Date -Format "MM-dd HH:mm:ss.fff"
adb logcat -T "$timestamp" main-modloader:W QuestHook[QuestSounds`|v1.0.0]:* AndroidRuntime:E *:S QuestHook[UtilsLogger`|v1.1.1]:E QuestHook[UtilsLogger`|v1.0.12]:E
        if ($args[0] -eq "--debug") {
            $timestamp = Get-Date -Format "MM-dd HH:mm:ss.fff"
            adb logcat -T "$timestamp" main-modloader:W QuestHook[QuestSounds`|v1.0.0]:* AndroidRuntime:E *:S QuestHook[UtilsLogger`|v1.1.1]:* QuestHook[UtilsLogger`|v1.0.12]:*
        }
                if ($args[0] -eq "--insane") {
            $timestamp = Get-Date -Format "MM-dd HH:mm:ss.fff"
            adb logcat -T "$timestamp" main-modloader:W QuestHook[QuestSounds`|v1.0.0]:* AndroidRuntime:E *:S QuestHook[UtilsLogger`|v1.1.1]:* QuestHook[UtilsLogger`|v1.0.12]:*
        }

echo "Exiting logging.ps1"