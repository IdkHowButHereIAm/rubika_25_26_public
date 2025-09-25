#include "Debugs.h"


void Debugs::DrawDebugWindow()
{
	GetInstance()->DrawDebug();
}

Logger& Debugs::GetLogger()
{
	return GetInstance()->Logger;
}

Debugs* Debugs::GetInstance()
{
	if (!Instance)
	{
		Instance = new Debugs();
	}

	return Instance;
}

void Debugs::DeleteInstance()
{
	delete Instance;
	Instance = nullptr;
}

void Debugs::DrawDebug()
{
	// Draw your debug window here
}
