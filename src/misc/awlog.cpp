//
// Created by erico on 5/19/2024.
//

#include "awlog.h"
#include "plugin/agsplugin.h"

namespace AGSWorks
{

// from ags Common/debug/out.h, but not expected to change
enum MessageType
{
    kDbgMsg_None                = 0,
    kDbgMsg_Alert               ,
    kDbgMsg_Fatal               ,
    kDbgMsg_Error               ,
    kDbgMsg_Warn                ,
    kDbgMsg_Info                ,
    kDbgMsg_Debug               ,
};


void AWLog::InitImpl(void *pengine)
{
    _pengine = pengine;
    auto *engine = static_cast<IAGSEngine*>(_pengine);
}

void AWLog::ShutdownImpl()
{
    _pengine = nullptr;
    // do nothing else
}

void AWLog::LogInfoImpl(const std::string &msg) const
{
    if(!_pengine) return;
    auto *engine = static_cast<IAGSEngine*>(_pengine);
    if(engine->version >= 29) {
        engine->Log(kDbgMsg_Info, "%s", msg.c_str());
    } else {
        std::string mmsg = "Info: " + msg;
        engine->PrintDebugConsole(mmsg.c_str());
    }
}

void AWLog::LogDebugImpl(const std::string &msg) const
{
    if(!_pengine) return;
    auto *engine = static_cast<IAGSEngine*>(_pengine);
    if(engine->version >= 29) {
        engine->Log(kDbgMsg_Debug, "%s", msg.c_str());
    } else {
        std::string mmsg = "Debug: " + msg;
        engine->PrintDebugConsole(mmsg.c_str());
    }
}

void AWLog::LogErrorImpl(const std::string &msg) const
{
    if(!_pengine) return;
    auto *engine = static_cast<IAGSEngine*>(_pengine);
    if(engine->version >= 29) {
        engine->Log(kDbgMsg_Error, "%s", msg.c_str());
    } else {
        std::string mmsg = "Error: " + msg;
        engine->PrintDebugConsole(mmsg.c_str());
    }
}


void AWLog::Init(void * pengine)
{
    AWLog::get().InitImpl(pengine);
}

void AWLog::Shutdown()
{
    AWLog::get().ShutdownImpl();
}

void AWLog::LogInfo(const std::string &msg)
{
    AWLog::get().LogInfoImpl(msg);
}

void AWLog::LogDebug(const std::string &msg)
{
    AWLog::get().LogDebugImpl(msg);
}

void AWLog::LogError(const std::string &msg)
{
    AWLog::get().LogErrorImpl(msg);
}

} // namespace AGSWorks