//
//  logger.hpp
//
//  Created by yuancunkuan on 2025/2/5.
//

#ifndef logger_hpp
#define logger_hpp

#include <base.hpp>

class Logger {
public:
    enum LogLevel {
        INFO,
        WARNING,
        ERROR
    };

    Logger() {
        std::cout << "Log start" << std::endl;
    }

    ~Logger() {
        std::cout << "Log end" << std::endl;
    }
    inline void printArgs() {
        std::cout << std::endl;
    }
    
    template<typename T, typename... Args>
    void printArgs(const T& value, Args&&... args)
    {
        std::cout << value << " ";
        printArgs(args...);
    }
    template<typename... Args >
    void log(LogLevel level, Args&&... args) {
        std::cout << "[" << currentDateTime() << "] "
            << "[" << logLevelToString(level) << "] ";
        printArgs(args...);
    }

private:
    std::string logLevelToString(LogLevel level) {
        switch (level) {
            case INFO:      return "INFO";
            case WARNING:   return "WARNING";
            case ERROR:     return "ERROR";
        }
        return "UNKNOWN";
    }

    std::string currentDateTime() {
        std::time_t now = std::time(nullptr);
        char buf[128];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        return buf;
    }
};

extern Logger LogPrint;

#endif /* logger_hpp */
