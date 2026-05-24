#pragma once
#include "cstdint"

extern void* g_VFSManager;

bool VFS_IsInitialized();
int VFS_OpenFile(char* filepath, const char* mode, uint8_t flag);