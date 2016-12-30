#include "StdAfx.h"
#include "Plugin.h"
#include "Interface\ICryFreeVideoSystem.h"

// Included only once per DLL module.
#include <CryCore/Platform/platform_impl.inl>

IEntityRegistrator *IEntityRegistrator::g_pFirst = nullptr;
IEntityRegistrator *IEntityRegistrator::g_pLast = nullptr;

CPlugin::~CPlugin()
{
	gEnv->pSystem->GetISystemEventDispatcher()->RemoveListener(this);

	IEntityRegistrator* pTemp = IEntityRegistrator::g_pFirst;
	while (pTemp != nullptr)
	{
		pTemp->Unregister();
		pTemp = pTemp->m_pNext;
	}
}

bool CPlugin::Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams)
{
	gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this);

	m_pCryFreeVideoSystem = new CCryFreeVideoSystem();

	return (m_pCryFreeVideoSystem != nullptr);
	return true;
}
void CPlugin::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	switch (event)
	{
	case ESYSTEM_EVENT_GAME_POST_INIT:
	{
		// Register entities
		IEntityRegistrator* pTemp = IEntityRegistrator::g_pFirst;
		while (pTemp != nullptr)
		{
			pTemp->Register();
			pTemp = pTemp->m_pNext;
		}
	}
	break;
	}
}

///////////////////////////////////////////////////////////////////////////
ICryFreeVideoSystem* CPlugin::GetICryFreeVideoSystem() const
{
	return m_pCryFreeVideoSystem;
}

CSingletonFactory<CPlugin> CPlugin::s_factory;