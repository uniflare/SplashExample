# Splash screen example plugin
#
 Splash Example Plugin is a simple, light-weight splash screen renderering plugin with several features.
  - Drop and play, once added to the csv, just add your textures to see it in action!
  - Renders a Pre-Splash texture to the intial window that spawns on game launch
  - Compatible with engine templates and GameSDK Sample project for 5.3
  - Several CVars to control the plugin without building from source
    - CVar `splash_show` (def 1) Enable/Disable splash plugin
    - CVar `splash_minimumPlaybackTime` (def 10.0) Splash display time in seconds
    - CVar `splash_startTimeOffset` (def 0.0) Allows for more accurate splash length
    - CVar `splash_texture_a` (def 'SplashExample/textures/splash_a.dds') Pre splash texture
    - CVar `splash_texture` (def 'SplashExample/textures/splash.dds') Main splash texture
  - Fully documented code available on github for re-use/educational purposes
  - Automatically overrides original splash screens (if any)

### Installing pre built binary
Installing the plugin to an existing project is easy, just drop the plugin next to your `.cryproject` file and add this line to the `cryplugins.csv` file:
```
C++;SplashExample;Plugin_SplashExample;SplashExample.dll;Assets
```

### Building from source
Once you clone the git source to a local repo you may want to modify the `CMakeLists.txt` file in `code/`. Specifically `TestPlatformPath` and `TestPlatformProject`, setting these properly will allow you to build and debug without having to move a fresh dll or setting the debugging properties on every build.

##### Take note when building;
The supplied `CMakeLists.txt` will automatically copy the project output dll to the specified `TestPlatformPath` on `PostBuild`. It will also modify the `vcxproj.user.in` file from `code/` to set the debugger launch target to the specified `TestPlatformProject` when generating the solution.

### More information
For help or more information please look for the associated thread, or pm me (name `Uni Bliss`) on the Cry Engine &reg; forums.