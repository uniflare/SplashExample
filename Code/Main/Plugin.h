#pragma once

#include <CrySystem/ICryPlugin.h>

#include <CryGame/IGameFramework.h>

#include <CryEntitySystem/IEntityClass.h>

#include <Interface\ICryFreeVideoPlugin.h>
#include <System/CryFreeVideoSystem.h>

class CCryFreeVideoSystem;

class CPlugin : public ICryFreeVideoPlugin
	, public ISystemEventListener
{
	// CRYGENERATE_SINGLETONCLASS(CPlugin, "Plugin_Sample", 0xF77494CD6D0E44FE, 0xBEFBAA9E2CA27CB5)

private:
	typedef TL::BuildTypelist < ICryUnknown
		, ICryFreeVideoPlugin
		, ICryPlugin
	> ::Result _UserDefinedPartialInterfaceList;

protected:
	typedef TL::NoDuplicates<_UserDefinedPartialInterfaceList>::Result FullInterfaceList;

	friend struct CFactory<CPlugin>::CustomDeleter;

private:
	friend class CFactory<CPlugin>;
	static CSingletonFactory<CPlugin> s_factory;
	template<class Dst, class Src> friend Dst * InterfaceCastSemantics::cryinterface_cast(Src*);
	template<class Dst, class Src> friend Dst * InterfaceCastSemantics::cryinterface_cast(const Src*);
	template<class Dst, class Src> friend std::shared_ptr<Dst> InterfaceCastSemantics::cryinterface_cast(const std::shared_ptr<Src> &);
	template<class Src> friend std::shared_ptr<ICryUnknown> CompositeQuerySemantics::crycomposite_query(Src*, const char*, bool*);
	template<class Src> friend std::shared_ptr<const ICryUnknown> CompositeQuerySemantics::crycomposite_query(const Src*, const char*, bool*);
	template<class Src> friend std::shared_ptr<ICryUnknown> CompositeQuerySemantics::crycomposite_query(const std::shared_ptr<Src> &, const char*, bool*);
	template<class Src> friend std::shared_ptr<const ICryUnknown> CompositeQuerySemantics::crycomposite_query(const std::shared_ptr<const Src> &, const char*, bool*);

public:
	virtual ICryFactory* GetFactory() const override
	{
		return &s_factory;
	}

protected:
	virtual void* QueryInterface(const CryInterfaceID &iid) const override
	{
		return CW::InterfaceCast<FullInterfaceList>::Op(this, iid);
	}

	template<class TList> friend struct CW::Internal::CompositeQuery;

	virtual void* QueryComposite(const char* name) const override
	{
		return CW::CompositeQuery::Op(*this, name);
	}
public:
	static const char* GetCName()
	{
		return "Plugin_Sample";
	}

	static const CryClassID& GetCID()
	{
		static const CryClassID cid = { (uint64)0xF77494CD6D0E44FE, (uint64)0xBEFBAA9E2CA27CB5 };
		return cid;
	}

	static std::shared_ptr<CPlugin> CreateClassInstance()
	{
		ICryUnknownPtr p = s_factory.CreateClassInstance();
		return std::shared_ptr<CPlugin>(*static_cast<std::shared_ptr<CPlugin>*>(static_cast<void*>(&p)));
	}

	CPlugin() {};
	virtual ~CPlugin();
	
	//! Retrieve name of plugin.
	virtual const char* GetName() const override { return "Plugin_Sample"; }

	//! Retrieve category for the plugin.
	virtual const char* GetCategory() const override { return "Game"; }

	//! This is called to initialize the new plugin.
	virtual bool Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams) override;

	virtual void OnPluginUpdate(EPluginUpdateType updateType) override {}

	// ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
	// ~ISystemEventListener

	virtual ICryFreeVideoSystem*	GetICryFreeVideoSystem() const;
	static  CPlugin*				GetCryFreeVideoPlugin() { return m_pThis; }

protected:
	static CPlugin* m_pThis;

private:
	CCryFreeVideoSystem* m_pCryFreeVideoSystem;
};

struct IEntityRegistrator
{
	IEntityRegistrator()
	{
		if (g_pFirst == nullptr)
		{
			g_pFirst = this;
			g_pLast = this;
		}
		else
		{
			g_pLast->m_pNext = this;
			g_pLast = g_pLast->m_pNext;
		}
	}

	virtual void Register() = 0;
	virtual void Unregister() = 0;

public:
	IEntityRegistrator *m_pNext;

	static IEntityRegistrator *g_pFirst;
	static IEntityRegistrator *g_pLast;
};