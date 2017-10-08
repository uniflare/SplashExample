Copyright (c) 2017, uniflare, see LICENSE.txt for details
# Splash Screen Example Plugin
#
 Splash Example Plugin is a simple, light-weight splash screen rendering plugin with several features such as;
  - Drop and play!*
  - Compatible with engine templates and GameZero/GameSDK Sample project**
  - Renders an 'Initial' Splash texture in windowed mode immediately (first)
  - Renders a 'Main' Splash texture in windowed or fullscreen mode
  - Several CVars to control the plugin without building from source
    - CVar `splash_plugin_enable` (def 1) Enable/Disable splash plugin
    - CVar `splash_show` (def 1) Controls showing the main (second) splash image
    - CVar `splash_minimumPlaybackTime` (def 3.0) 'Main' splash display time in seconds
    - CVar `splash_texture` (def 'SplashExample/Textures/Splash.dds') Main splash texture
    - CVar `splash_show_initial` (def 1) Enable/Disable the intial splash image
    - CVar `splash_minimumPlaybackTime_initial` (def 3.0) 'Initial' splash display time in seconds
    - CVar `splash_texture_initial` (def 'SplashExample/Textures/InitialSplash.dds') Pre splash texture
    - CVar `splash_startTimeOffset` (def 0.0) Allows for more accurate splash length in some cases
  - Extra documentation for learning/understanding the example plugin' concept
  - Verbosely commented source code (available on github)
  - Automatically overrides original splash screen routine (if any)
  
    *   Installation can vary between projects, be sure to read the INSTALL.txt document in the 'Docs/' folder.
    **  Assumes that the example plugin package is compatible with the target engine.

### Quick Start
If you want to see it in action immediately, right-click and select 'Launch Game' on any cryproject file*
	
    *   Assumes compatible engines have been installed via the CRYENGINE Launcher,
	        also assumes the necessary assets folder exists in the projects root and contains the splash 
			example textures.
			
	Note for 5.4: CryGameSDK does not seem to be lnicluded with the downloaded engine via the CRYENGINE Launcher.
	    For this reason, an extra step is required to test the CryGameSDK; Simply copy CryGameSDK.dll from 
		the GameSDK (5.4) bin folder to the launcher engine bin folder, by default, this means;
		    C:\Program Files (x86)\Crytek\CRYENGINE Launcher\Crytek\gamesdk_5.4\GameSDK\bin\win_x64\CryGameSDK.dll
			to
			C:\Program Files (x86)\Crytek\CRYENGINE Launcher\Crytek\CRYENGINE_5.4\bin\win_x64\CryGameSDK.dll

### Integration with a project
Integration consists of a several requierd steps to combine the splash plugin with an external project. For 
more information consult the INSTALL.txt document in the 'Docs/' folder supplied with this package.

### Building the plugin using Visual Studio
Building is not necessary to use the plugin, however it is very easy to do so.
Consult the INSTALL.txt document in the 'Docs/' folder supplied with this package.

### More information
For help or more information please visit the associated thread on the forums below.
CRYENGINE &reg; Forum Thread: https://forum.cryengine.com/viewtopic.php?f=55&t=4201
CRYENGINE &reg; (Archived) Forum Thread: https://www.cryengine.com/community_archive/viewtopic.php?f=314&t=135972
GitHub &reg; Repository: https://github.com/uniflare/SplashExample