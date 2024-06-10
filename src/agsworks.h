#ifndef AGS_WORKS_H
#define AGS_WORKS_H

#include "plugin/agsplugin.h"

namespace agsworks
{
  const char* AGS_GetPluginName();
  void AGS_EngineStartup(IAGSEngine *lpEngine);
  void AGS_EngineShutdown();
  int AGS_EngineOnEvent(int event, int data);
  int AGS_EngineDebugHook(const char *scriptName, int lineNum, int reserved);
  void AGS_EngineInitGfx(const char *driverID, void *data);
}

#endif // AGS_WORKS_H