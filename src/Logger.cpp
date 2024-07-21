#include "Logger.h"
#include <iostream>
#include <ctime>
#include <format>

inline std::tm localtime_xp_now()
{
    std::time_t timer = std::time(0);
    std::tm bt{};
#if defined(__unix__)
    localtime_r(&timer, &bt);
#elif defined(_MSC_VER)
    localtime_s(&bt, &timer);
#else
    static std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);
    bt = *std::localtime(&timer);
#endif
    return bt;
}

inline std::string get_timestamp() 
{
    std::tm now = localtime_xp_now();
    return std::format("[{0}-{1:02d}-{2:02d} {3:02d}:{4:02d}:{5:02d}]",
        now.tm_year + 1900,
        now.tm_mon + 1,
        now.tm_mday,
        now.tm_hour,
        now.tm_min,
        now.tm_sec
    );
}

void Engine::Logger::Log(LogLevel level, std::string message)
{
    log << get_timestamp() << " [" << LogLevelString[level] << "]: " << message << std::endl;
}


void Engine::Logger::LogVerbose(std::string message) { Log(LogLevel::Verbose, message); }

void Engine::Logger::LogInfo(std::string message) { Log(LogLevel::Info, message); }

void Engine::Logger::LogWarning(std::string message) { Log(LogLevel::Warning, message); }

void Engine::Logger::LogError(std::string message) { Log(LogLevel::Error, message); }

const char* Engine::Logger::LogLevelString[] = { "VERBOSE", "INFO", "WARNING", "ERROR" };

std::ofstream Engine::Logger::log("logfile.txt", std::ios_base::app | std::ios_base::out);