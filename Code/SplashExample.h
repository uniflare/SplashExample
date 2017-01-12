// 05/01/2017 Copyright by Uniflare. All rights reserved.
// See bundled license.txt for more information.

#pragma once
#include "SplashExampleCVars.h"

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

		SScreenResolution() :
			iWidth(-1)
			, iHeight(-1)
			, nDepthPerPixel(-1)
			, sResolution("")
		{}
	};

	SScreenResolution m_sFallbackResolution;

	// Console variables provided by this object
	SSplashExampleCVars * m_sCVars;

	//! The loaded texture we will use to render to the screen
	ITexture * m_pSplashTexture;

	//! The intial splash texture (before we can go fullscreen)
	ITexture * m_pInitialSplashTexture;

	//! Simple flag to make sure we don't initialize more than once
	bool m_bConstructed;

	//! Simple flag to make sure we don't register/unregister our listener too many times
	bool m_bListenerRegistered;

private:

	//! Handles init of Splash Example. (Can only init once)
	void TryConstruct();

	//! Draws the supplied texture in stretched mode to the main viewport
	void Draw2DImage(const ITexture * tex, bool bUseTextureSize = false) const;

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

	//! Sets the r_width and r_height CVars if they are set properly in sResolution
	void SetScreenResolution(const SScreenResolution &sResolution) const;

	//! Self explanatory
	void SafeReleaseSplashTextures();

	// ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
	// ~ISystemEventListener

public:

	CSplashExample(SSplashExampleCVars * sCVars);

	//! Only used to TryConstruct()
	bool Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams);

	~CSplashExample();
};