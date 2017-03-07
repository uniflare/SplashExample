# Splash screen example plugin
#
 Splash Example Plugin is a simple, light-weight splash screen renderering plugin with several features.
  - Drop and play, once added to the csv, just add your textures to see it in action!
  - Renders a Pre-Splash texture to the intial window that spawns on game launch
  - Compatible with engine templates and GameSDK Sample project for 5.3
  - Several CVars to control the plugin without building from source
    - CVar `splash_show` (def 1) Enable/Disable splash plugin
    - CVar `splash_show_initial` (def 1) Controls showing the intial splash image
    - CVar `splash_show_main` (def 1) Controls showing the main (second) splash image
    - CVar `splash_minimumPlaybackTime` (def 10.0) Splash display time in seconds
    - CVar `splash_minimumPlaybackTimeA` (def 5.0) Initial splash display time in seconds
    - CVar `splash_startTimeOffset` (def 0.0) Allows for more accurate splash length
    - CVar `splash_texture_a` (def 'SplashExample/textures/splash_a.dds') Pre splash texture
    - CVar `splash_texture` (def 'SplashExample/textures/splash.dds') Main splash texture
  - Fully documented code available on github for re-use/educational purposes
  - Automatically overrides original splash screens (if any)

### Installing pre built binary
Installing the plugin to an existing project using the pre-built win 64 binaries is easy.
  - Copy the `SplashExample.dll` file next to your own `Game.dll` file (in `bin/win_x64` folder)
  - Merge the `Assets` directory from the plugin with your games own `Assets` folder
  - Add this line to the `cryplugins.csv` (next to your cryproject file):
```
C++;SplashExample;Plugin_SplashExample;bin/win_x64/SplashExample.dll;Assets
```

Take note: For the best effect with the initial splash screen, you should set r_width and r_height to the 
same dimensions as the splash_a.dds texture you supply. Also, make sure r_fullscreen is not set to 1 in any cfg and also make sure r_fullscreenwindow is set to 1 to remove window border.
(This is not required for the plugin to work, but not enforcing this will cause the intial splash to flicker between settings)

An example configuration is found in Assets/SplashExample.cfg

### Building from source
Building is not necessary to use the plugin, however it is very easy to do so.
Take note the supplied `CMakeLists.txt` and `vcxproj.user.in` files have been customized, 
The debugger launch param is automatically set to the specified game project on generating the solution, 
whilst upon building CMake will copy the built `SplashExample.dll` to the correct location automatically.

To make use of this customization simply modify the variables `TestPlatformRoot`, `TestPlatformPath` and `TestPlatformProject` in the `CMakeLists.txt`. 
This is highly advised before generating a solution as a copied dll may just end up cluttering your root drive.

### More information
For help or more information please visit the associated thread on the forums below.
Cry Engine &reg; Forum Thread: https://www.cryengine.com/community_archive/viewtopic.php?f=314&t=135972
GitHub &reg; Repository: https://github.com/uniflare/SplashExample