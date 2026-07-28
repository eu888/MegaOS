#include "string.h"

/**
 * @brief Compares two strings for equality.
 * @param s1 The first string.
 * @param s2 The second string.
 * @return 0 if the strings are equal, a non-zero value otherwise.
 */
uint8_t strcmp(const char* s1, const char* s2){
    while(*s1 && (*s1 == *s2)){
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

/**
 * @brief Copies a string from the source to the destination buffer.
 * @param dest The destination buffer.
 * @param src The source string.
 * @return A pointer to the destination buffer.
 */
char* strcpy(char* dest, const char* src){
    char* ret = dest;
    while((*dest++ = *src++));
    return ret;
}

/**
 * @brief Calculates the length of a string.
 * @param str The string.
 * @return The length of the string.
 */
size_t strlen(const char* str){
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

/**
 * @brief Compares the first n characters of two strings.
 * @param s1 The first string.
 * @param s2 The second string.
 * @param index The number of characters to compare.
 * @return 0 if the strings are equal, a non-zero value otherwise.
 */
uint8_t strncmp(const char* s1, const char* s2, size_t index){
    while(index > 0){
        if(*s1 != *s2){
            return *(const unsigned char*)s1 - *(const unsigned char*)s2;
        }
        if(*s1 == '\0'){
            return 0;
        }
        s1++;
        s2++;
        index--;
    }
    return 0;
}

void *memset(void *dest, int val, size_t len){
    uint8_t *ptr = (uint8_t *)dest;
    while (len-- > 0) {
        *ptr++ = (uint8_t)val;
    }
    return dest;
}

void *memcpy(void *dest, const void *src, size_t n) {  
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
   
    return dest;
}