#pragma once
#include <Interface/ICryFreeVideoSystem.h>
#include <CrySystem/ISystem.h>

//class RenderListener;

class CCryFreeVideoSystem : public ICryFreeVideoSystem
	//, public ISystemEventListener
{
public:
	CCryFreeVideoSystem();
	~CCryFreeVideoSystem();

	//virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;

	virtual void CryFreeVideoStart() override;
	virtual void CryFreeVideoEnd() override;

	virtual ICryFreeVideoSystem * GetICryFreeVideoSystem() override {
		return this;
	};

private:
	void    Shutdown();

	bool m_bDoIntroMovies;
	bool m_bPlayingVideo;
	//RenderListener * m_pRenderListener;
};