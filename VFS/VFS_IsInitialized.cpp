#include "VFS.h"

bool VFS_IsInitialized() {
    return g_VFSManager != nullptr;
}