#include "AsyncSystem.h"

AsyncStream* AsyncStream_Create(char* filename, uint8_t priority, int flags) {
    AsyncStream* stream;
    bool isInitialized;

    stream = AsyncStream_Pool_Acquire();
    if (stream != nullptr) {
        isInitialized = AsyncStream_Initialize(stream, filename, priority, flags);
        if (isInitialized) {
            return stream;
        }
        AsyncStream_Pool_Release(stream);
    }
    return nullptr;
}