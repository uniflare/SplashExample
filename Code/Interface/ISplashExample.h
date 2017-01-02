#pragma once

// Public plugin interface
struct ISplashExample
{
	virtual ~ISplashExample() {}

	//! public
	//! Signals the plugin to begin playback
	virtual void BeginPlayback() = 0;

	//! public
	//! Signals the plugin to stop immediately
	virtual void StopPlayback(bool force = false) = 0;

	//! public
	//! Internally uses EF_LoadTexture()
	virtual void SetOverlayTexture(string path) = 0;

	//! public
	//! Used to get the instance for the public interface
	virtual ISplashExample * GetISplashExample() = 0;
};
