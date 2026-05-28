#pragma once
#include <cstdint>

// ==========================================
// PART 1: THE VFS LEVEL (What we already built)
// Used by Archive_IterateEntries
// ==========================================

struct ArchiveNode {
    uint32_t     key;      // +0x00
    void* archive;  // +0x04 
    ArchiveNode* next;     // +0x08
};

struct ArchiveBucket {
    uint32_t     unused;      
    ArchiveNode* head;        
    ArchiveNode* headAlt;     
};

struct ArchiveIterator {
    uint8_t        log2BucketCount; 
    uint8_t        _pad[3];         
    ArchiveBucket* buckets;         
    uint32_t       _unk08;          
    int32_t        bucketIndex;     
    ArchiveNode* currentNode;     
};

// ==========================================
// PART 2: THE INTERNAL ARCHIVE LEVEL (The new stuff)
// Used by Archive_LookupFile
// ==========================================

// Structurally identical to ArchiveNode, but holds File Metadata!
struct FileNode {
    uint32_t  nameHash;    // +0x00
    int* metadata;    // +0x04 (Ghidra's hashEntry[1])
    FileNode* next;        // +0x08 (Ghidra's hashEntry[2])
};

struct ArchiveFileTable {
    uint32_t  configBits;    // +0x00 (low 5 bits = log2 of bucket count)
    FileNode* buckets;       // +0x04 (Array of FileNodes)
    uint32_t  _unk08;        // +0x08
    int32_t   searchIndex;   // +0x0C (Used dynamically by Iterator)
    FileNode* activeNode;    // +0x10 (Used dynamically by Iterator)
};

struct EntryState {
    uint32_t uncompressedSize; // +0x00 
    uint8_t* compressedData;   // +0x04 
    uint8_t* cachedBuffer;     // +0x08 
    uint32_t seekPosition;     // +0x0C 
    uint32_t allocSize;        // +0x10 
};

// The actual Archive object itself
struct Archive {
    void* pad00[2];  
    ArchiveFileTable* fileTable; // +0x08 (This is 'archiveHeader' from Ghidra)
    EntryState* state;     // +0x0C
    int               refCount;  // +0x10
    bool              useAltMem; // +0x14 
};

// ==========================================
// Function Declarations
// ==========================================
void* Archive_IterateEntries(ArchiveIterator* iterator, uint32_t* outKey);
int* Archive_LookupFile(Archive* currentArchive, char* filepathBuffer);
int   Archive_ResolveEntry(Archive* currentArchive, int* fileEntry);