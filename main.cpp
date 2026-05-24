#include "AsyncSystem.h"
#include "VFS.h"

int main () {
    g_AsyncCallbackBufferIdx = 0;
    g_AsyncFrameState = 0;
    uint32_t g_PendingCallbackCount[2] = { 0, 0 };
    CallbackEntry g_PendingCallbacks[8] = { 0 };
    uint32_t g_ActiveStreamCount = 0;
    AsyncStream* g_StreamPool[16] = { nullptr };

    int dummyVFS = 1; 
    g_VFSManager = &dummyVFS;

    return 0;
}