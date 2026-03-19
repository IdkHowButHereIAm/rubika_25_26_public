#pragma once

class InputMgr;
class TextureMgr;
class RandomMgr;
class DebugMgr;
class GameMgr;
class Logger;
class TaskMgr;

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
	InputMgr* InputMgr;
	TextureMgr* TextureMgr;
	DebugMgr* DebugMgr;
	Logger* Console;
	RandomMgr* RandomMgr;
	TaskMgr* TaskMgr;

	bool ExitApp = false;
};

extern Globals gData;