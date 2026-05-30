#pragma once
#include "cstdint"

int Engine_IsAsyncEnabled();
void* Engine_AllocAlignedObject(uint32_t requestedSize);
uint8_t* Engine_LoadFileToCache(char* filename);
void Engine_GetFileSizeFromName(char* filename);