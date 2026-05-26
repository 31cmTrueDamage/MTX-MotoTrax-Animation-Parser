#include "VFS.h"
#include "Archive.h"

int VFS_FindEntry(VFS_Context* ctx, char* filepath)
{
    char filepathBuffer[128];
    filepathBuffer[0] = *filepath;
    
    // Normalize path separators (replace '/' with '\')
    if (*filepath != 0) {
        char* dst = filepathBuffer;
        char ch;
        do {
            if (*dst == '/') *dst = '\\';
            ch = filepath[(dst - filepathBuffer) + 1];
            *++dst = ch;
        } while (ch != 0);
    }

    // Reset iterator state
    ArchiveIterator* iter = ctx->iterator;
    iter->bucketIndex = -1;
    iter->currentNode = nullptr;

    Archive* archive = (Archive*)Archive_IterateEntries(iter, nullptr);

    while (archive != nullptr) {
        int* fileEntry = Archive_LookupFile(archive, filepathBuffer);
        
        if (fileEntry != nullptr) {
            // Update Context
            ctx->currentArchive  = archive;
            ctx->fileEntry       = fileEntry;
            ctx->resolvedAddress = Archive_ResolveEntry(archive, fileEntry);
            
            // Return the resolved physical address as the handle
            return ctx->resolvedAddress; 
        }
        archive = (Archive*)Archive_IterateEntries(iter, nullptr);
    }
    
    return 0;
}