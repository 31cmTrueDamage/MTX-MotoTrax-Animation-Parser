#include <cstdint>
#include <cstring>

// This creates the exact same global address space the original .exe had.
// The compiler will put this in the exact same kind of memory segment (e.g., .bss).
uint8_t g_slidingWindow[4096];

void LZSS_Decompress(uint8_t* inputBuffer, uint8_t* outputBuffer, int inputSize) 
{
    // Safety bound check
    if (inputSize <= 0 || inputSize >= 0x2000000) {
        return; 
    }

    // Initialize the first 4078 bytes to 0x20 (ASCII space).
    // This perfectly replicates the *windowClearPtr = 0x20202020 loop.
    memset(g_slidingWindow, 0x20, 4078);
    
    uint32_t windowCursor = 4078; // 0xFEE in Ghidra
    uint32_t bitFlags = 0;

    while (true) {
        bitFlags >>= 1;

        // If the 9th bit (0x100) is 0, we've exhausted our flags and must read a new control byte
        if ((bitFlags & 0x100) == 0) {
            if (inputSize == 0) return;
            
            uint8_t controlByte = *inputBuffer++;
            inputSize--;
            
            // Set the 9th bit to 1 (0xFF00) so we can count exactly 8 shifts
            bitFlags = controlByte | 0xFF00; 
        }

        // Flag bit 1 means this is an uncompressed literal byte
        if ((bitFlags & 1) != 0) {
            if (inputSize == 0) break;
            
            uint8_t literal = *inputBuffer++;
            inputSize--;
            
            *outputBuffer++ = literal;
            
            // Add to dictionary and advance the cursor, wrapping at 4096 (0xFFF)
            g_slidingWindow[windowCursor] = literal;
            windowCursor = (windowCursor + 1) & 0xFFF;
        } 
        // Flag bit 0 means this is a compressed dictionary match
        else {
            if (inputSize == 0) return;
            
            uint8_t byteLow = *inputBuffer++;
            uint8_t byteHigh = *inputBuffer++;
            inputSize -= 2;
            
            // 12-bit match position: lower 8 bits from byteLow, upper 4 bits from byteHigh
            uint32_t matchPos = byteLow | ((byteHigh & 0xF0) << 4);
            
            // A 0-to-(len+2) inclusive loop runs exactly len + 3 times.
            uint32_t matchLen = (byteHigh & 0x0F) + 3;
            
            for (uint32_t i = 0; i < matchLen; i++) {
                uint8_t decodedByte = g_slidingWindow[(matchPos + i) & 0xFFF];
                
                *outputBuffer++ = decodedByte;
                
                g_slidingWindow[windowCursor] = decodedByte;
                windowCursor = (windowCursor + 1) & 0xFFF;
            }
        }
    }
}