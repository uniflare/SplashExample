#pragma once

#include <CrySystem/ICryPlugin.h>

struct ICryFreeVideoSystem;

struct ICryFreeVideoPlugin
	: public ICryPlugin
{
	//CRYINTERFACE_DECLARE(ICryFreeVideoPlugin, 0xC97AD475FD95416D, 0x9C3048B2D2C5B7F6);
	template<class T> friend const CryInterfaceID &InterfaceCastSemantics::cryiidof();
	friend struct std::default_delete<ICryFreeVideoPlugin>;
private:
	static const CryInterfaceID& IID()
	{
		static const CryInterfaceID iid = { (uint64)0xC97AD475FD95416D, (uint64)0x9C3048B2D2C5B7F6 };
		return iid;
	}
public:

	virtual ICryFreeVideoSystem*	GetICryFreeVideoSystem() const = 0;
	static  ICryFreeVideoPlugin*	GetCryFreeVideoPlugin() { return m_pThis; }

protected:
	static ICryFreeVideoPlugin* m_pThis;
};

ICryFreeVideoPlugin* ICryFreeVideoPlugin::m_pThis = nullptr;
