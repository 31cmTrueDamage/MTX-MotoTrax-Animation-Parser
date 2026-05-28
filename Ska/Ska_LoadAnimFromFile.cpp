#include <string.h>
#include <iostream>
#include "SkaAnimation.h"
#include "AsyncSystem.h"
#include "Utils.h"
#include "Engine.h"
// Loads .ska file from folder

void __thiscall Ska_LoadAnimFromFile(AnimData* animData,char* filename,uint32_t loadFlags,bool allowAsync,bool allowCache) {

    char originalFilePath[256];
    char lowercaseFilePath[256];

    strncpy(lowercaseFilePath, filename, sizeof(filename));
    String_ToLower(lowercaseFilePath);

    animData->animHashID = Hash_CRC32_Path(lowercaseFilePath);

    strncpy(originalFilePath, filename, sizeof(filename));

    bool isAsyncEnabled = Engine_IsAsyncEnabled();
    //PATH 1 -> ASYNC MODE ENABLED
    if (isAsyncEnabled && allowAsync) {
        AsyncStream* asyncStream = AsyncStream_Create(filename, 0, 100);
        animData->asyncStreamHandle = asyncStream;
        if (asyncStream == nullptr) {
            return;
        }
        uint32_t fileSize = AsyncStream_GetFileSize(asyncStream);
        uint8_t* animDataBuffer = (uint8_t*)Engine_AllocAlignedObject(fileSize);
        animData->rawSkaBuffer = animDataBuffer;
        
    }
}