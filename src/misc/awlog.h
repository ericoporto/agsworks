//
// Created by erico on 5/19/2024.
//

#ifndef AGSWORKS_AWLOG_H
#define AGSWORKS_AWLOG_H


#include <string>

namespace AGSWorks {

    using SCAPI_SYSTEM_LOG = void (*)(void *chaa, int message_type, const char *texx, ...);

    class AWLog {
        private:
            AWLog() = default;
            SCAPI_SYSTEM_LOG _system_log = nullptr;

        public:
            static AWLog& get(){
                static AWLog instance;
                return instance;
            }

            void LogInfo(const std::string &msg);
            void LogDebug(const std::string &msg);
            void LogError(const std::string &msg);
            void Init(void * pengine);
            void Shutdown();
    };

} // namespace AGSWorks

#define LOG_INFO(msg) AWLog::get().LogInfo(msg)
#define LOG_ERROR(msg) AWLog::get().LogError(msg)
#define LOG_DEBUG(msg) AWLog::get().LogDebug(msg)

#endif //AGSWORKS_AWLOG_H
