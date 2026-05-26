#include <cstdlib>
#include <cstdint>
#include <cstring>

void* Engine_AllocAlignedObject(uint32_t requestedSize)
{
    if (requestedSize == 0) {
        return nullptr;
    }

    // Allocate size + 24 bytes (0x18) to guarantee space for alignment and headers
    uint8_t* rawBuffer = (uint8_t*)malloc(requestedSize + 0x18);
    if (rawBuffer == nullptr) {
        return nullptr;
    }

    // Store the requested size at the very start of the raw buffer
    *(uint32_t*)rawBuffer = requestedSize;

    // Calculate 16-byte alignment.
    // Ghidra's "rawBuffer + 2" on a uint* pointer means +8 bytes.
    // So this calculates a 16-byte aligned boundary with at least 12 bytes of padding.
    uintptr_t alignedBase = ((uintptr_t)rawBuffer + 8) & ~0xF;
    
    // The actual returned object is 16 bytes after that base boundary
    uint8_t* alignedObject = (uint8_t*)(alignedBase + 0x10);

    // Store the original raw pointer exactly 4 bytes behind the returned pointer.
    // This allows the engine to later call: free(*(void**)(alignedObject - 4))
    void** originalPtrStorage = (void**)(alignedObject - 4);
    *originalPtrStorage = rawBuffer;

    // The original assembly manually zeroes out the memory (DWORD loop, then BYTE loop).
    // memset handles this cleanly and optimally.
    memset(alignedObject, 0, requestedSize);

    return alignedObject;
}