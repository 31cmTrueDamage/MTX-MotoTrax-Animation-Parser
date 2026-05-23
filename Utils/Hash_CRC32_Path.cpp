#include "Utils.h"
#include "crc32_table.h"
#include "cstdint"

uint32_t __cdecl Hash_CRC32_Path(const char* filePath) {
    if (filePath == nullptr) {
        return 0;
    }

    uint32_t crcHash = 0xffffffff;
    uint8_t currentChar = *filePath;

    while(currentChar != 0) {
        uint32_t processedChar = currentChar;
        filePath++;

        if (currentChar > '@' && currentChar < '[') {
            processedChar = currentChar + 0x20;
        }
        if (processedChar == '/') {
            processedChar = '\\';
        }

        uint32_t crcIndexTableIndex = (processedChar ^ crcHash) & 0xFF;
        crcHash = (crcHash >> 8) ^ crc32LookupTable[crcIndexTableIndex];

        currentChar = *filePath;
    }

    return crcHash;

}