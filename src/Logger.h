#include <fstream>
#include <format>

namespace Engine
{
	enum LogLevel
	{
		Verbose = 0,
		Info = 1,
		Warning = 2,
		Error = 3
	};

	class Logger
	{
	public:
		static void Log(LogLevel level, std::string message);
		static void LogVerbose(std::string message);
		static void LogInfo(std::string message);
		static void LogWarning(std::string message);
		static void LogError(std::string message);


	private:
		// TODO: enum LogLevel to string
		static const char* LogLevelString[];
		static std::ofstream log;
	};
}