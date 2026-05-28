#include "Archive.h"
#include "Utils.h"
#include "Engine.h"

int* Archive_LookupFile(Archive* archive, char* filepath)
{
    // archiveHeader in Ghidra -> our ArchiveFileTable at +0x08
    ArchiveFileTable* table = archive->fileTable; 
    
    uint32_t targetHash = Hash_CRC32_Path(filepath);
    
    // Calculate bucket index using the config bits (low 5 bits)
    uint32_t bucketMask = (1 << (table->configBits & 0x1F)) - 1;
    
    // table->buckets is an array of FileNodes (stride is 0xC / 12 bytes)
    FileNode* hashEntry = &table->buckets[targetHash & bucketMask];
    
    if (hashEntry != nullptr) {
        
        // Walk the collision chain
        while (hashEntry->nameHash != targetHash || hashEntry->metadata == nullptr) {
            hashEntry = hashEntry->next;
            if (hashEntry == nullptr) {
                return nullptr;
            }
        }
        
        int* fileMetadata = hashEntry->metadata;
        
        if (fileMetadata != nullptr) {
            
            // Resolve physical offsets for this file
            Archive_ResolveEntry(archive, fileMetadata);
            
            // activeEntry in Ghidra -> our EntryState at +0x0C
            EntryState* state = archive->state; 
            
            // If the file needs decompression (size > 0) but isn't in memory yet (buffer == null)
            if (state->cachedBuffer == nullptr && state->uncompressedSize != 0) {
                
                //FUN_00547070(); // Mutex Lock
                
                // The if/else in Ghidra checks archive->useAltMem (+0x14), but both branches 
                // do the exact same allocation in the compiled output.
                state->cachedBuffer = (uint8_t*)Engine_AllocAlignedObject(state->allocSize);
                
                //FUN_00547070(); // Mutex Unlock
                
                // Decompress from source data into our newly allocated buffer
                LZSS_Decompress(state->compressedData, state->cachedBuffer, state->uncompressedSize);
            }
            
            return fileMetadata;
        }
    }
    
    return nullptr;
}