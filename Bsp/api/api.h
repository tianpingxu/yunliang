#ifndef __API_H__
#define __API_H__

#include <n32l40x.h>
#include <stdio.h>


int xy_strcmp(char *str1, const char *str2);
char *xy_strcpy(char *dest, const char *src);
unsigned long xy_strlen(unsigned char *str);
void *xy_memset(void *src, unsigned char ch, unsigned short count); 
void *xy_memcpy(void *dest, const void *src, unsigned short count); 
unsigned char xy_frame_calc_xor(unsigned char *msgPtr, unsigned char len);

#endif