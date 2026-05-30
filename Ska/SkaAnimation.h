#pragma once

#include <cstdint>
#include "AsyncSystem.h"

// Forward declarations
struct Matrix4x4;
struct AnimData;
struct AnimationCache;
struct DeltaNode;

// --------------------------------------------------------
// 1. Core Math Structures
// --------------------------------------------------------

struct Matrix4x4 {
    float m[16];
};

struct Vector3 {
    float x, y, z;
};

struct Quaternion {
    float x, y, z, w;
};

// --------------------------------------------------------
// 2. The Runtime Animation Object (.ska mapped in memory)
// --------------------------------------------------------

struct AnimData {
    /* 0x00 */ void* vtable;                // Virtual function table pointer
    /* 0x04 */ uint32_t configFlags;        // Masking checks (e.g., & 0x80000)
    /* 0x08 */ int32_t trackCount;          // Total number of bones/tracks
    /* 0x0C */ uint8_t* rawSkaBuffer;       // Pointer to loaded .ska file bytes
    /* 0x10 */ void* asyncStreamHandle;     // Handle for background streaming
    /* 0x14 */ int32_t durationTicks;       // Animation duration/ticks
    /* 0x18 */ uint16_t* posDataStart;      // Ptr -> Start of compressed Position payload
    /* 0x1C */ uint16_t* rotDataStart;      // Ptr -> Start of compressed Rotation payload
    /* 0x20 */ uint32_t pad1[2];            
    /* 0x28 */ uint32_t* trackActiveMask;   // Ptr -> Bitfield determining active bones
    /* 0x2C */ uint32_t animHashID;         // CRC32 Hash of the lowercase filename
    /* 0x30 */ uint32_t pad2;               
    /* 0x34 */ uint16_t* posTrackHeader;    // Ptr -> Array of Position Strides (sizes)
    /* 0x38 */ uint16_t* rotTrackHeader;    // Ptr -> Array of Rotation Strides (sizes)
    /* 0x3C */ uint32_t pad3;               
    /* 0x40 */ uint16_t pad4;               
    /* 0x42 */ uint8_t stateFlags;          // Bit 2 (|= 4) = Cached in RAM
    
    // Note: 0x43 onwards may contain additional state data not seen in the loader
};

// --------------------------------------------------------
// 3. The Sparse Update Node (Delta Compression)
// --------------------------------------------------------
// Allocated at exactly 0x30 (48 bytes) per moving bone

struct DeltaNode {
    /* 0x00 */ uint32_t nodeSize;           // Usually initialized to 0x18
    /* 0x04 */ uint32_t pad1[2]; 
    /* 0x0C */ DeltaNode* nextNode;         // Linked list pointer for the current frame
    
    // Payload Data begins at 0x10
    /* 0x10 */ float trackID;               // Which track/bone is moving
    /* 0x14 */ float compressedData;        // Unpacked scale/weight/metadata
    /* 0x18 */ float vectorPart1;           // First half of vector/quat evaluation
    /* 0x1C */ float vectorPart2;           // Second half of vector/quat evaluation
    /* 0x20 */ float boneIdx;               // Index in the matrix palette
    /* 0x24 */ uint32_t pad2[3]; 
};

// --------------------------------------------------------
// 4. The Baked Playback Cache (The "Skeleton" output)
// --------------------------------------------------------

struct AnimationCache {
    /* 0x00 */ void* vtable;
    /* 0x04 */ AnimData* sourceAnim;        // Pointer to the source AnimData
    /* 0x08 */ Matrix4x4* frameMatrixCache; // Massive flat array: totalFrames * trackCount * 16 bytes
    /* 0x0C */ Matrix4x4* basePoseBuffer;   // Snapshot of Frame 0 (Bind Pose)
    /* 0x10 */ DeltaNode** deltaFrameList;  // Array of linked lists (one list per frame)
    /* 0x14 */ int32_t totalFrames;         // Total frames baked for playback
    /* 0x18 */ int32_t trackCount;          // Total bones (copied from AnimData)
    /* 0x1C */ int32_t requiredMemory;      // Total bytes allocated for frameMatrixCache
};

// --------------------------------------------------------
// 5. Reconstructed Engine API Functions
// --------------------------------------------------------

// Standard Loader
void __thiscall Ska_LoadAnimFromFile(AnimData* animObj, char* filename, uint32_t loadFlags, bool allowAsync, bool allowCache);

// The Parser (This reads the raw .ska bytes and fills out the pointers at 0x18, 0x1C, 0x34, 0x38)
void __thiscall Ska_InitAnimationInstance(AnimData* animObj);

// The Baker (Converts procedural .ska tracks into flat matrices)
void __thiscall Ska_BuildAnimationCache(AnimationCache* cacheObj, AnimData* animData, float* trackCount);

// The Evaluator (Math engine called by the Baker)
uint32_t __thiscall Ska_EvaluateChannels(AnimData* animData, float* outTranslation, float* outRotation, float animTime, void* evalContext);

// Math Utilities
void __fastcall Math_TransformToMatrix4x4(void* unused, float* inTranslation, float* inRotation, float* outMatrix);
void __fastcall Math_TransformToMatrix_Mirrored(void* unused, float* inTranslation, float* inRotation, bool flipAxisA); // Uses BL register for flipAxisB

void Ska_AsyncLoadCallback(AsyncStream* stream, int status, uint32_t flags, AnimData* animData);