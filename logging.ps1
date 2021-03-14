$timestamp = Get-Date -Format "MM-dd HH:mm:ss.fff"
adb logcat -T "$timestamp" main-modloader:W QuestHook[QuestSounds`|v0.2.1]:* AndroidRuntime:E *:F *:S QuestHook[UtilsLogger`|v1.1.6]:E QuestHook[UtilsLogger`|v1.1.5]:E QuestHook[UtilsLogger`|v1.1.4]:E
        if ($args[0] -eq "--debug") {
            $timestamp = Get-Date -Format "MM-dd HH:mm:ss.fff"
            adb logcat -T "$timestamp" main-modloader:W  QuestHook[QuestSounds`|v0.2.1]:* AndroidRuntime:F AndroidRuntime:E *:S QuestHook[UtilsLogger`|v1.1.6]:* QuestHook[UtilsLogger`|v1.1.5]:*
        }
                if ($args[0] -eq "--insane") {
            $timestamp = Get-Date -Format "MM-dd HH:mm:ss.fff"
            adb logcat -T "$timestamp" main-modloader:W QuestHook[QuestSounds`|v0.2.1]:* AndroidRuntime:E AndroidRuntime:F *:S QuestHook[UtilsLogger`|v1.1.6]:* QuestHook[UtilsLogger`|v1.1.5]:*
        }

echo "Exiting logging.ps1"