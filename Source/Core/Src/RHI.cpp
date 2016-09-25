#include "RHI.hpp"

#include "Windows.h"
#include <string>

IDynamicRHI* IDynamicRHI::msRHI = NULL;

typedef IDynamicRHI* (__stdcall *CreateDynamicRHIProc)();

/************************************************************************/
IDynamicRHI* IDynamicRHI::DynamicLoadRenderer(const char* pName)
{
#ifdef _DEBUG
	std::string lSuffix(".d.dll");
#else
	std::string lSuffix(".dll");
#endif
	std::string lPrefix("RHI");

	std::string lFilename = lPrefix + std::string(pName) + lSuffix;
	HINSTANCE lLib = LoadLibrary(lFilename.c_str());
	if (lLib)
	{
		CreateDynamicRHIProc lProc = (CreateDynamicRHIProc)GetProcAddress(lLib, "CreateDynamicRHI");
		if (lProc)
		{
			return lProc();
		}
	}

	return NULL;
}

/************************************************************************/
void IDynamicRHI::SetRenderer(IDynamicRHI* pRenderer)
{
	msRHI = pRenderer;
}