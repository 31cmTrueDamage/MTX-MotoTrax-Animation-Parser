#include "Utils.h"
#include "cstdint"

uint32_t __cdecl Hash_CRC32_Path(const char* filePath) {
    if (filePath == nullptr) {
        return 0;
    }

    uint32_t CRCHash = 0xffffffff;
    uint8_t currentChar = *filePath;

    while(currentChar != 0) {
        uint32_t processedChar = currentChar;
        filePath++;
    }

}