#include "AsyncSystem.h"
#include "VFS.h"

int AsyncStream_StartRead(AsyncStream* stream, uint8_t* buffer, int elementSize, int count) {

    stream->state = 3;

    if (stream->fileHandle != nullptr) {

        int bytesRead = VFS_Read(buffer, elementSize, count, stream->fileHandle);

        stream->fileSize = bytesRead;
        g_ActiveStreamCount++;

        stream->isBusy++;

        stream->vtable->StartRead(stream->state, stream->fileSize, 0);

        return stream->fileSize;
    }
    int callbackResult = stream->vtable->PlatformAsyncRead(buffer, elementSize, count);
    return callbackResult;
}