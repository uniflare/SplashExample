// 05/01/2017 by Uniflare.

#pragma once
#include <CryRenderer/ITexture.h>
#include <IPlayerProfiles.h>

// CSplashExample implements ISystemEventListener
//
//! Handles main functionality of plugin Splash Example
class CSplashExample 
	: public ISystemEventListener
{
	//! Struct to hold screen resolution details
	struct SScreenResolution
	{
		// Copied from GameSDK - For using profile saved resolution
		int iWidth;
		int iHeight;
		unsigned int nDepthPerPixel;
		string sResolution;

		SScreenResolution(unsigned int _iWidth, unsigned int _iHeight, unsigned int _nDepthPerPixel, const char* _sResolution) :
			iWidth(_iWidth)
			, iHeight(_iHeight)
			, nDepthPerPixel(_nDepthPerPixel)
			, sResolution(_sResolution)
		{}

		SScreenResolution()
		{}
	};

	//! The intial splash texture (before we can go fullscreen)
	ITexture * m_pSplashTextureA;

	//! The loaded texture we will use to render to the screen
	ITexture * m_pSplashTexture;

	//! Simple flag to make sure we don't register/unregister our listener too many times
	bool m_bListenerRegistered;

	//! Store the user/game or system cfg resolution to restore it after
	SScreenResolution m_sOriginalResolution;

	//! Store the user/game or system cfg fullscreen value to restore it after
	bool m_bOriginalFullscreen;

private:

	//! Draws the supplied texture in stretched mode to the main viewport
	void Draw2DImageScaled(const ITexture * tex, bool bUseTextureSize = false) const;

	//! Wrapper to stall the current thread for LengthTime whilst drawing pTex
	bool DrawAndStall(float StartTime, float LengthTime, ITexture * pTex, bool bUseTextureSize = false, bool bUpdateInput = true, bool bUpdateMouse = true, bool bDrawConsole = true, bool bPumpWinMsg = true);

	//! Creates an integer from the player profile attribute requested
	const int GetAttributeIValue(const string attName, const IPlayerProfile * pProfile) const;

	//! Gets the currently used player profile
	const IPlayerProfile * GetCurrentPlayerProfile() const;

	//! Gets the requested screen resolution from GetScreenResolutions()
	const SScreenResolution GetScreenResolution(const int idx) const;

	//! Generates a list of supported screen resolutions
	const std::vector<SScreenResolution> GetScreenResolutions() const;

	//! Handles our event listener (makes sure we only set/remove once each)
	void RegisterListener(const bool bRegister);

	//! Sets the r_width and r_height CVars if they are set properly in sResolution
	void SetResolutionCVars(const SScreenResolution &sResolution) const;

	// ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
	// ~ISystemEventListener

public:

	CSplashExample();
	~CSplashExample();
};