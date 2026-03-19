#include "Globals.h"

#include <Engine/Gameplay/GameMgr.h>
#include <Engine/Input/InputMgr.h>
#include <Engine/Render/Ressource/TextureMgr.h>
#include <Engine/Debug/DebugMgr.h>
#include <Engine/Console/LogConsole.h>
#include <Engine/Random/RandomMgr.h>
#include <Engine/Task/TaskMgr.h>

Globals gData;

Globals::Globals() : FrameCount(0)
{
	GameMgr = new ::GameMgr();
	InputMgr = new ::InputMgr();
	TextureMgr = new ::TextureMgr();
	DebugMgr = new ::DebugMgr();
	Console = new ::Logger();
	RandomMgr = new ::RandomMgr();
	TaskMgr = new ::TaskMgr();
}

Globals::~Globals()
{
	Destroy();
}

void Globals::Init()
{
	//GameMgr->Init();
	//InputMgr->Init();
	TextureMgr->Init();
	//DebugMgr->Init();
	Console->Init();
	RandomMgr->Init();
	TaskMgr->Init();
}

void Globals::Shut()
{
	//GameMgr->Shut();
	//InputMgr->Shut();
	TextureMgr->Shut();
	//DebugMgr->Shut();
	Console->Shut();
	RandomMgr->Shut();
	TaskMgr->Shut();
}

void Globals::Destroy()
{
	delete InputMgr;
	InputMgr = nullptr;

	delete DebugMgr;
	DebugMgr = nullptr;

	delete GameMgr;
	GameMgr = nullptr;

	delete TextureMgr;
	TextureMgr = nullptr;

	delete Console;
	Console = nullptr;

	delete RandomMgr;
	RandomMgr = nullptr;

	delete TaskMgr;
	TaskMgr = nullptr;
}
