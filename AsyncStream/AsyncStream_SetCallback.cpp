#include "AsyncSystem.h"

void __thiscall AsyncStream_SetCallback(AsyncStream* stream, AsyncCallbackPtr callbackFunc, void* userData, uint32_t flags) {
    stream->callback = callbackFunc;
    stream->callbackContext = userData;
    stream->loadFlags = flags;
}