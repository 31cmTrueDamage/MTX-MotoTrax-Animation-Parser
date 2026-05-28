#include "Archive.h"
#include <cstdint>

int Archive_ResolveEntry(Archive* archive, int* fileMetadata) 
{
    // The engine reuses the ArchiveIterator logic for the ArchiveFileTable.
    // We can safely cast the fileTable to an ArchiveIterator to use its traversal functions.
    ArchiveIterator* iter = (ArchiveIterator*)archive->fileTable;

    // Reset the iterator state to begin a fresh search
    iter->bucketIndex = -1;
    iter->currentNode = nullptr;

    // IterateEntries returns the 'archive' void* from the node.
    // In the context of an internal file table, this pointer is our EntryState metadata.
    EntryState* state = (EntryState*)Archive_IterateEntries(iter, nullptr);

    while (true) {
        if (state == nullptr) {
            return 0; // File entry not found in the hash map
        }
        
        // vfsState[1] in Ghidra maps to state->compressedData (+0x04).
        // It checks if it has a valid offset/pointer, and if it matches the requested file.
        if (state->compressedData != nullptr && state == (EntryState*)fileMetadata) {
            break;
        }
        
        // Fetch the next entry in the collision chain/bucket
        state = (EntryState*)Archive_IterateEntries(iter, nullptr);
    }

    // Cache the active state in the archive object (offset +0x0C)
    archive->state = state;

    // Return the physical file offset (or memory handle if already loaded).
    // If uncompressedSize != 0, it means it's compressed, so we return the cached buffer offset (+0x08).
    if (state->uncompressedSize != 0) {
        return (int)(uintptr_t)state->cachedBuffer;
    }
    
    // Otherwise, return the direct compressed data offset (+0x04).
    return (int)(uintptr_t)state->compressedData;
}