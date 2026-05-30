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

    uint8_t* animDataBuffer;

    strncpy(lowercaseFilePath, filename, sizeof(filename));
    String_ToLower(lowercaseFilePath);

    animData->animHashID = Hash_CRC32_Path(lowercaseFilePath);

    strncpy(originalFilePath, filename, sizeof(filename));

    bool isAsyncEnabled = Engine_IsAsyncEnabled();
    //ASYNC MODE ENABLED
    if (isAsyncEnabled && allowAsync) {
        AsyncStream* asyncStream = AsyncStream_Create(filename, 0, 100);
        animData->asyncStreamHandle = asyncStream;
        if (asyncStream == nullptr) {
            return;
        }
        uint32_t fileSize = AsyncStream_GetFileSize(asyncStream);
        animDataBuffer = (uint8_t*)Engine_AllocAlignedObject(fileSize);
        animData->rawSkaBuffer = animDataBuffer;
        
        AsyncStream_SetCallback(asyncStream, (AsyncCallbackPtr)Ska_AsyncLoadCallback, animData, loadFlags & 0xff);
        AsyncStream_StartRead(asyncStream, animData->rawSkaBuffer, 1, fileSize);
    }
    //WE CAN USE RAM, RAM SO FAST :)))))))))))))))))))))))))))))))))))))))))))
    if (allowCache) {
        animDataBuffer = Engine_LoadFileToCache(filename);
        animData->rawSkaBuffer = animDataBuffer;
        Engine_GetFileSizeFromName(filename);

        animData->stateFlags |= 4;

        Ska_InitAnimationInstance(animData);
        return;
    }

    //OR WE CAN BE USING YOUR SLOW AHH DISK......................

}