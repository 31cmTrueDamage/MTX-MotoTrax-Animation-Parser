#include "AsyncSystem.h"

AsyncStream* AsyncStream_Pool_Acquire(void) {
    int currentSlotIdx = g_ActiveStreamCount;
    
    // Bounds check
    if (g_ActiveStreamCount > 0xF) {
        return nullptr;
    }

    // Check if preferred slot is busy using struct member directly
    if (g_StreamPool[currentSlotIdx]->isBusy > 0) {
        int searchIdx = 0;
        
        // Find a free slot
        for (searchIdx = g_ActiveStreamCount + 1; searchIdx < 0x10; searchIdx++) {
            if (g_StreamPool[searchIdx]->isBusy < 1) {
                // Swap
                AsyncStream* tempStreamPtr = g_StreamPool[searchIdx];
                g_StreamPool[searchIdx] = g_StreamPool[currentSlotIdx];
                g_StreamPool[currentSlotIdx] = tempStreamPtr;
                break;
            }
        }
        
        // If pool is full, wait for callbacks to process
        if (searchIdx == 0x10) {
            AsyncStream* lockedStream = g_StreamPool[currentSlotIdx];
            do {
                Async_ProcessCallbacks();
                currentSlotIdx = g_ActiveStreamCount;
            } while (lockedStream->isBusy > 0);
        }
    }

    AsyncStream* stream = g_StreamPool[currentSlotIdx];

    // Initialize all fields using the correct byte-aligned names
    stream->asyncSize   = -1;
    stream->pad2   = -1;
    stream->isBusy     = 0;
    stream->callback   = 0;
    stream->callbackContext   = 0;
    stream->loadFlags   = 0;
    stream->state      = 0;
    stream->field_18   = 0;
    stream->field_1C   = 0;
    stream->priority   = 1;
    stream->field_20   = 0x10000;
    stream->userData   = 100;
    stream->fileSize   = 0;
    stream->fileHandle = 0;

    // Call vtable Reset (+0x08)
    stream->vtable->Reset();

    g_ActiveStreamCount++;
    return stream;
}