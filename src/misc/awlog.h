//
// Created by erico on 5/19/2024.
//

#ifndef AGSWORKS_AWLOG_H
#define AGSWORKS_AWLOG_H


#include <string>

namespace AGSWorks {
    class AWLog {
        private:
            AWLog() = default;
            void* _pengine = nullptr;

            void LogInfoImpl(const std::string &msg) const;
            void LogDebugImpl(const std::string &msg) const;
            void LogErrorImpl(const std::string &msg) const;
            void InitImpl(void * pengine);
            void ShutdownImpl();
        public:
            static AWLog& get(){
                static AWLog instance;
                return instance;
            }

            static void LogInfo(const std::string &msg);
            static void LogDebug(const std::string &msg);
            static void LogError(const std::string &msg);
            static void Init(void * pengine);
            static void Shutdown();
    };

} // namespace AGSWorks

#define LOG_INFO(msg) AWLog::get().LogInfo(msg)
#define LOG_ERROR(msg) AWLog::get().LogError(msg)
#define LOG_DEBUG(msg) AWLog::get().LogDebug(msg)

#endif //AGSWORKS_AWLOG_H
