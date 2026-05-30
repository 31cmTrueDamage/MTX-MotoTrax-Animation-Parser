#pragma once
#include "cstdint"
#include "Archive.h"

extern void* g_VFSManager;

extern uint32_t g_VFS_LastResult;

struct VFS_Context {
    void* currentArchive;
    int* fileEntry; // 0x04
    int resolvedAddress; // 0x08
    int* failedFileHandle;   //0x0C
    ArchiveIterator* iterator; // 0x10
};

extern VFS_Context* VFS_Address;

bool VFS_IsInitialized();
int VFS_OpenFile(char* filepath, const char* mode, uint8_t flag);
int VFS_FindEntry(VFS_Context* vfsContext, char* filepath);
int VFS_GetFileSize(int* fileHandle);
int VFS_ValidateHandle(VFS_Context* vfs, int* fileHandle);
int VFS_Read(uint8_t* buffer, int elementSize, int count, int* fileHandle);
int VFS_RawRead(Archive* archive, uint8_t* buffer, uint32_t totalBytes);