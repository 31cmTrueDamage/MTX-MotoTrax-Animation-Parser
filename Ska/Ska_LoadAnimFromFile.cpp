#include <string.h>
#include <iostream>
#include "SkaAnimation.h"
#include "Utils.h"
// Loads .ska file from folder

void __thiscall Ska_LoadAnimFromFile(AnimData* this_ptr,const char* filename,uint32_t loadFlags,bool allowAsync,bool allowCache) {

    char originalFilePath[256];
    char lowercaseFilePath[256];

    strncpy(lowercaseFilePath, filename, sizeof(filename));
    String_ToLower(lowercaseFilePath);

}