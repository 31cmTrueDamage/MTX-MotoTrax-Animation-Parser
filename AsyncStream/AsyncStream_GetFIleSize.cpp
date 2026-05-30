#include "AsyncSystem.h"
#include "VFS.h"

uint32_t AsyncStream_GetFileSize(AsyncStream* stream) {
    // Path A: The VFS Path (Offset 0x30)
    // If the stream has a valid VFS file handle, we can get the size instantly.
    if(stream->fileHandle != 0) {
        uint32_t fileSize = VFS_GetFileSize((int*)stream->fileHandle);
        stream->fileSize = fileSize;
        return stream->fileSize;
    }
    // Path B: The Raw Async Path (Offset 0x28)
    // If there is no VFS handle, it means the background thread is directly 
    // asking the Windows OS for the file size. 
    
    // This is a Spinlock! It pauses the main thread and waits in an infinite loop 
    // as long as the size is less than 0 (e.g. -1 means "Still Loading").
    do {

    } while (stream->asyncSize < 0);
    return stream->asyncSize;
}