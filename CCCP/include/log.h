#pragma once

#define _DEBUG 1

#include <string>
#include <chrono>

namespace Soviet
{
    enum LOG_LEVELS { INFO, WARNING, ERROR, DEBUG };

    struct LogConfig
    {
        bool showTime;
        bool showLevel;
        std::string timeFormat = "%H:%M:%S";
    };

    class Log // idk or use this instead https://github.com/apache/logging-log4cxx
    {
    public:
        Log(LogConfig config)
            : m_Config(config) {}

        template<LOG_LEVELS type, class... Args>
        inline void Print(const std::string& fmt, Args&&... args)
        {
            if(type == DEBUG && !_DEBUG) return;

            std::string finalMessage = "";

            if(m_Config.showTime)
            {
                auto t = std::time(nullptr);
                auto tm = *std::localtime(&t);

                std::ostringstream oss;
                oss << std::put_time(&tm, m_Config.timeFormat.c_str());
                auto str = oss.str();
                finalMessage += str + " ";
            }

            if(m_Config.showLevel)
                finalMessage += "[" + GetLogLevelString(type) + "] ";
            finalMessage += fmt;
            std::printf(finalMessage.c_str(), args...);
        }

    private:

        std::string GetLogLevelString(LOG_LEVELS level)
        {
            switch (level)
            {
            case INFO:
                return "INFO";
            case WARNING:
                return "WARNING";
            case ERROR:
                return "ERROR";
            case DEBUG:
                return "DEBUG";
            }
            return "";
        }

        const LogConfig m_Config;
    };
}