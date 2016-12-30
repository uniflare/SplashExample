#pragma once

struct ICryFreeVideoSystem
{
	virtual ~ICryFreeVideoSystem() {}

	virtual void CryFreeVideoStart() = 0;
	virtual void CryFreeVideoEnd() = 0;
	virtual ICryFreeVideoSystem * GetICryFreeVideoSystem() = 0;
};