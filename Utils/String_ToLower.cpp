#include <cctype>

void String_ToLower(char* str) {
    for(int i = 0; str[i] != '\0'; i++) {
        str[i] = (char)tolower(str[i]);
    }
}