#include "VFS.h"

int VFS_Read(uint8_t* buffer, int elementSize, int count, int* fileHandle) {

    int resolvedAddress = VFS_ValidateHandle(VFS_Address, fileHandle);

    if (resolvedAddress != 0) {

        g_VFS_LastResult = 1;

        Archive* currentArchive = (Archive*)VFS_Address->currentArchive;
        int bytesRead = VFS_RawRead(currentArchive, buffer, elementSize * count);

        return bytesRead;
    }

    g_VFS_LastResult = 0;
    return 0;
}