#include "VFS.h"    

int VFS_GetFileSize(int* fileHandle) {
    int entryIndex;

    entryIndex = VFS_ValidateHandle(VFS_Address, fileHandle);
    if (entryIndex == 0) {
        g_VFS_LastResult = 0;
    } else {
        g_VFS_LastResult = 1;
        Archive* currentArchive = (Archive*)VFS_Address->currentArchive;
        if(currentArchive->state != nullptr) {
            return currentArchive->state->allocSize;
        }
    }
    return 0;
}