& $PSScriptRoot/build.ps1
if ($?) {
    adb push libs/arm64-v8a/libQuestSounds.so /sdcard/Android/data/com.beatgames.beatsaber/files/mods/libQuestSounds.so
    adb push libs/arm64-v8a/libbeatsaber-hook_1_1_1.so /sdcard/Android/data/com.beatgames.beatsaber/files/libs/libbeatsaber-hook_1_1_1.so
    if ($?) {
        adb shell am force-stop com.beatgames.beatsaber
        adb shell am start com.beatgames.beatsaber/com.unity3d.player.UnityPlayerActivity
        if ($args[0] -eq "--log") {
            $timestamp = Get-Date -Format "MM-dd HH:mm:ss.fff"
            adb logcat -T "$timestamp" main-modloader:W QuestHook[QuestSounds`|v1.0.0]:* AndroidRuntime:E *:S QuestHook[UtilsLogger`|v1.1.1]:E
        }
        if ($args[0] -eq "--debug") {
            $timestamp = Get-Date -Format "MM-dd HH:mm:ss.fff"
            adb logcat -T "$timestamp" main-modloader:W QuestHook[QuestSounds`|v1.0.0]:* AndroidRuntime:E *:S QuestHook[UtilsLogger`|v1.1.1]:*
        }
    }
}
echo "Exiting Copy.ps1"