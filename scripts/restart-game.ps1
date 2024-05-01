adb shell am force-stop com.beatgames.beatsaber
Start-Sleep -Seconds 0.8
adb shell am start com.beatgames.beatsaber/com.unity3d.player.UnityPlayerActivity
