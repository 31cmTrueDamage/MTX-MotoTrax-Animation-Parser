#pragma once
#include <cstdint>

struct CallbackEntry {
    void* stream;
    uint32_t status;
    void* userData;
};

struct AsyncStreamVTable {
    void* padding_00;                                                // Offset 0x00
    void (*StartRead)(uint32_t state, uint32_t field_4, uint32_t);   // Offset 0x04
    void (*Reset)();                                                 // Offset 0x08
    bool (*HandleError)(char* filepath);                             // Offset 0x0C
};

struct AsyncStream {
    AsyncStreamVTable* vtable; // 0x00
    
    uint32_t field_04;         // 0x04 (stream[1]) - Success flag
    uint32_t field_08;         // 0x08 (stream[2])
    uint32_t field_0C;         // 0x0C (stream[3])
    uint32_t field_10;         // 0x10 (stream[4])
    uint32_t state;            // 0x14 (stream[5]) - Status
    uint32_t field_18;         // 0x18 (stream[6])
    
    uint8_t  field_1C;         // 0x1C (*(stream + 7))
    uint8_t  priority;         // 0x1D
    uint16_t padding_1E;       // 0x1E Alignment
    
    uint32_t field_20;         // 0x20 (stream[8]) - initialized to 0x10000
    int32_t  userData;         // 0x24 (stream[9]) - initialized to 100
    int32_t  field_28;         // 0x28 (stream[10]) - initialized to -1
    int32_t  field_2C;         // 0x2C (stream[0xb]) - initialized to -1
    
    int32_t  fileHandle;       // 0x30 (stream[0xc])
    int32_t  isBusy;           // 0x34 (stream[0xd])
};

// These store the state of the async system.
extern uint32_t g_AsyncCallbackBufferIdx;
extern uint32_t g_AsyncFrameState;
extern uint32_t g_PendingCallbackCount[2];
extern CallbackEntry g_PendingCallbacks[8];
extern uint32_t g_ActiveStreamCount;
extern AsyncStream* g_StreamPool[16];

void Async_ToggleCallbackBuffer(void);
void Async_ProcessCallbacks(void);
AsyncStream* AsyncStream_Pool_Acquire(void);
bool AsyncStream_Initialize(AsyncStream* stream, char* filepath, uint8_t priority, int userData);