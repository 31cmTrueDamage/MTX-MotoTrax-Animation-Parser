#include "AsyncSystem.h"

// Returns 1 on success, 0 on failure
int AsyncStream_Pool_Release(AsyncStream* stream) 
{
    // Initialize to 0 (Ghidra's uVar1)
    uint32_t poolIndex = 0; 

    do {
        // Did we find the stream we want to release?
        if (stream == g_StreamPool[poolIndex]) {
            
            // 1. Shrink the active boundary by 1
            g_ActiveStreamCount--;
            
            // 2. "Swap and Pop": Move the very last active stream into the hole we just made
            g_StreamPool[poolIndex] = g_StreamPool[g_ActiveStreamCount];
            
            // 3. Toss the released stream into the newly vacated "inactive" slot at the end
            g_StreamPool[g_ActiveStreamCount] = stream;
            
            return 1; // Successfully released (This is Ghidra's weird CONCAT31)
        }
        
        poolIndex++;
    } while (poolIndex < 0x10); // Loop through the max 16 streams

    return 0; // Stream wasn't found (This is Ghidra's uVar1 & 0xffffff00)
}