#pragma once
#include "TaskMgr.h"

class InputMgr;
class TextureMgr;
class RandomMgr;
class DebugMgr;
class GameMgr;
class Logger;

class Globals
{
public:
	Globals();
	~Globals();

	void Init();
	void Shut();
	void Destroy();

	unsigned int FrameCount;

	GameMgr* GameMgr;
	TextureMgr* TextureMgr;
	TaskMgr* TaskMgr;
	DebugMgr* DebugMgr;
	Logger* Console;
	RandomMgr* RandomMgr;

	bool ExipApp = false;
};

extern Globals gData;