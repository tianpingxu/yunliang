#include "api.h"
#include <stddef.h>


/**
 * @brief get string len
 * @param str higher bits data
 * @return string len
 */
unsigned long xy_strlen(unsigned char *str) {
    unsigned long len = 0;
    if (str == NULL) {
        return 0;
    }

    for (len = 0; *str++ != '\0';) {
        len++;
    }

    return len;
}

/**
 * @brief  assign ch to the first count bytes of the memory address src
 * @param src srouce address
 * @param ch set value
 * @param count length of set address
 * @return void
 */
void *xy_memset(void *src, unsigned char ch, unsigned short count) {
    unsigned char *tmp = (unsigned char *) src;
    if (src == NULL) {
        return NULL;
    }

    while (count--) {
        *tmp++ = ch;
    }
    return src;
}

/**
 * @brief copy count bytes data from src to dest
 * @param dest destination address
 * @param src srouce address
 * @param count length of copy data
 * @return void
 */
void *xy_memcpy(void *dest, const void *src, unsigned short count) {
    unsigned char *pdest = (unsigned char *) dest;
    const unsigned char *psrc = (const unsigned char *) src;
    unsigned short i;

    if (dest == NULL || src == NULL) {
        return NULL;
    }

    if ((pdest <= psrc) || (pdest > psrc + count)) {
        for (i = 0; i < count; i++) {
            pdest[i] = psrc[i];
        }
    } else {
        for (i = count; i > 0; i--) {
            pdest[i - 1] = psrc[i - 1];
        }
    }

    return dest;
}

/**
 * @brief copy string src to string dest
 * @param dest destination address
 * @param src srouce address
 * @return the tail of destination
 */
char *xy_strcpy(char *dest, const char *src) {
    char *p = dest;
    while (*src != '\0') {
        *dest++ = *src++;
    }
    *dest = '\0';
    return p;
}

/**
 * @brief compare string s1 with string s2
 * @param str1 srouce address
 * @param str2 destination address
 * @return compare result
 */
int xy_strcmp(char *str1, const char *str2) {
    while (*str1 && *str2 && *str1 == *str2) {
        str1++;
        str1++;
    }
    return *str1 - *str2;
}

/**
 * @brief 加校验和
 * @param msgPtr
 * @param len
 * @return 校验值
 */
unsigned char xy_frame_calc_xor(unsigned char *msgPtr, unsigned char len) {
    unsigned char x;
    unsigned char xorResult = 0;
    for (x = 0; x < len; x++, msgPtr++) {
        xorResult = xorResult + *msgPtr;
    }
    return (xorResult & 0xFF);
}
