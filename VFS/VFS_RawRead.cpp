#include "VFS.h"
#include <cstring>

int VFS_RawRead(Archive* archive, uint8_t* destBuffer, uint32_t totalBytes) {

    uint32_t bytesActuallyRead = archive->state->totalFileSize - archive->state->readCursor;

    if (totalBytes <= bytesActuallyRead) {
        bytesActuallyRead = totalBytes;
    }

    uint8_t* sourceBasePtr;
    if (archive->state->dataLocationFlag == 0) {
        sourceBasePtr = archive->state->rawDataPtr;    // Offset 0x04
    } else {
        sourceBasePtr = archive->state->cachedDataPtr; // Offset 0x08
    }

    uint8_t* srcPtr = sourceBasePtr + archive->state->readCursor;

    memcpy(destBuffer, srcPtr, bytesActuallyRead);

    archive->state->readCursor = archive->state->readCursor + bytesActuallyRead;

    return bytesActuallyRead;
}