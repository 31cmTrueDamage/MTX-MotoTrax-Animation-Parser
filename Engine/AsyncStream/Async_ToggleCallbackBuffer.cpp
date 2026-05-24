#include "AsyncSystem.h"

void Async_ToggleCallbackBuffer(void) {
    g_AsyncCallbackBufferIdx ^= 1;
    g_AsyncFrameState = 0;
}