#pragma once

#include <string>
#include <vector>

class Logger
{
public:
	Logger();
	~Logger();

	static Logger* Instance();
	static void DeleteInstance();

	static void Info(const std::string& message);
	static void Warning(const std::string& message);
	static void Error(const std::string& message);

	static void DrawLogger();

protected:

	inline static Logger* instance = nullptr;

	enum class eMessageType
	{
		Info,
		Warning,
		Error
	};

	struct sMessageInfo
	{
		// DateTime
		eMessageType Type;
		std::string Message;
	};

	void PushNewMessage(const sMessageInfo& message);
	void Draw();
	std::vector<sMessageInfo> Messages;

	bool DisplayErrorMessage = true;
	bool DisplayWarningMessage = true;
	bool DisplayInfoMessage = true;
};