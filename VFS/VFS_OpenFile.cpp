#include "VFS.h"
#include "stdlib.h"

int VFS_OpenFile(char* filepath, const char* mode, uint8_t flags)
{
    // The handle is the resolved address
    int fileHandle = VFS_FindEntry(VFS_Address, filepath); 
    if (fileHandle == 0) {
        g_VFS_LastResult = 0;
        return 0;
    }

    // Safely cast to our expanded internal Archive struct
    Archive* currentArchive = (Archive*)(VFS_Address->currentArchive);

    if (flags != '\0') {
        currentArchive->refCount++; 
    }

    // Check for invalid modes (anything besides 'r' or 'b')
    bool hasInvalidMode = false;
    const char* m = mode;
    while (*m != '\0') {
        if (*m != 'r' && *m != 'b') {
            hasInvalidMode = true;
        }
        m++;
    }

    if (hasInvalidMode) {
        // Handle cleanup on the internal archive state
        EntryState* entryState = currentArchive->state;
        uint8_t* cachedBuffer = entryState->cachedBuffer;
        
        if (cachedBuffer != 0) {
            // Memory layout points to a header 4 bytes behind the buffer block
            free(*(void**)(cachedBuffer - 4));
        }
        entryState->cachedBuffer = 0;

        if (flags == '\0') {
            g_VFS_LastResult = 0;
            return 0;
        }
        
        currentArchive->refCount--;
        g_VFS_LastResult = 0;
        return 0;
    }

    // Clear seek position for successful read
    currentArchive->state->seekPosition = 0; 

    // Return status flag to globals and handle back to caller
    g_VFS_LastResult = 1;
    return fileHandle; 
}