#include "AsyncSystem.h"
#include "VFS.h"

bool AsyncStream_Initialize(AsyncStream* stream, char* filepath, uint8_t priority, int userData) {
    stream->userData = userData;
    stream->priority = priority;
    stream->state = 1;

    bool isVFSReady = VFS_IsInitialized();
    if (isVFSReady) {
        int fileHandle = VFS_OpenFile(filepath, "rb", 1);
        stream->fileHandle = (int*)fileHandle;
        stream->fileSize = 1;
        stream->priority = 1;
        g_ActiveStreamCount++;
        stream->isBusy++;
        stream->vtable->StartRead(stream->state, stream->fileSize, 0);
        return (stream->fileSize != 0);
    }   
}