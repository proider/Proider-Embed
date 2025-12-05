//
// Created by proider on 25-12-5.
//

#ifndef GETUTF8BYTEOFFSET_H
#define GETUTF8BYTEOFFSET_H


#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#define U0000 0
#define U0080 128
#define U0800 2048
#define U10000 65536

uint32_t UTF8_Switch_Unicode(uint32_t character);
void GetUTF8StringUnicode(char* character,uint32_t length,uint32_t Unicode[]);
void GetByteOffSet(uint32_t Unicode[],uint32_t length,uint32_t offset[],uint8_t BytePerCharacter);

#endif //GETUTF8BYTEOFFSET_H
