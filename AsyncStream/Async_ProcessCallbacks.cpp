#include "AsyncSystem.h"

void Async_ProcessCallbacks(void) {
    Async_ToggleCallbackBuffer();
    uint32_t bufferIdx = g_AsyncCallbackBufferIdx ^ 1;
    int callbackCount = g_PendingCallbackCount[bufferIdx];

    if (callbackCount > 0) {
        int* callbackEntry = reinterpret_cast<int*>(&g_PendingCallbacks + (bufferIdx * 0x30));

        do {
            int streamPtr = callbackEntry[-1];
            typedef void (*CallbackFunc)(int, int, int, int, int);
            CallbackFunc onComplete = *(CallbackFunc*)(*(int*)streamPtr + 8);

            onComplete(streamPtr, 
                       callbackEntry[0], 
                       callbackEntry[1], 
                       *(int*)(streamPtr + 0xc), 
                       *(int*)(streamPtr + 0x10));

            g_ActiveStreamCount--;
            
            int* busyCounter = (int*)(streamPtr + 0x34);
            *busyCounter = *busyCounter - 1;

            if (*busyCounter == 0) {
                *(int*)(streamPtr + 0x14) = 0;
            }
            
            callbackEntry += 3;
            callbackCount--;
        } while (callbackCount != 0);
    }
    g_PendingCallbackCount[bufferIdx] = 0;
}