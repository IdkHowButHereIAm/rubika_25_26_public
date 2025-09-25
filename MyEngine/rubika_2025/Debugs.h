#pragma once

#include "Logger.h"

class Debugs
{
#pragma region Singleton

public:
	static Debugs* GetInstance();
	static void DeleteInstance();
private:
	static inline Debugs* Instance = nullptr;

#pragma endregion

public:
	static void DrawDebugWindow();
	static Logger& GetLogger();

	void DrawDebug();

private:
	Logger Logger;

	// You can add any method, member... you need in this file.
};