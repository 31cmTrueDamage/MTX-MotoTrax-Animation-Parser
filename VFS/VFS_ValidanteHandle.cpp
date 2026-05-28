#include "VFS.h"

int VFS_ValidateHandle(VFS_Context* vfs , int* fileHandle) {

    int resolvedAddress = 0;

    if (vfs->failedFileHandle == fileHandle) {
        return 0;
    }

    if (vfs->fileEntry != fileHandle) {

        ArchiveIterator* iter = vfs->iterator;
        iter->bucketIndex = -1;
        iter->currentNode = nullptr;
        
        Archive* currentArchive = (Archive*)Archive_IterateEntries(iter, nullptr);

        while (true) {
            if (currentArchive == nullptr) {
                vfs->failedFileHandle = fileHandle;
                return 0;
            }
            resolvedAddress = Archive_ResolveEntry(currentArchive, fileHandle);
            if (resolvedAddress != 0) {
                break;
            }
            currentArchive = (Archive*)Archive_IterateEntries(iter, nullptr);
        }
        vfs->currentArchive = currentArchive;
        vfs->fileEntry = fileHandle;
        vfs->resolvedAddress = resolvedAddress;
        return resolvedAddress;
    }
    return vfs->resolvedAddress;
}