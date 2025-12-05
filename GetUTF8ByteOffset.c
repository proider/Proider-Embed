//
// Created by proider on 25-12-5.
//

#include "GetUTF8ByteOffset.h"

#include "RGB_LCD.h"


uint32_t UTF8_Switch_Unicode(uint32_t character) {
    uint8_t unicode_byte[4] = { 0 };
    uint32_t unicode = 0;
    uint8_t Encode_byte = 0;
    if ( character<128) {
        return character;
    }
    else {
        if (character<0xDFC0) {
            Encode_byte = 2;
        }
        if (character>0xE0807F&&character<0xEFBFC0) {
            Encode_byte = 3;
        }
         if (character>0xF080807F&&character<0xF7BFBFC0) {
            Encode_byte = 4;
        }
    }
    switch (Encode_byte) {
        case 2:
            unicode_byte[0] = 0x1F&(character>>8);
            unicode_byte[1] =  0X3F& character ;
            unicode = (unicode_byte[0] << 6) | unicode_byte[1];
            break;
        case 3:
            unicode_byte[0] = 0x0F& (character >> 16);
            unicode_byte[1] = 0X3F & (character >> 8);
            unicode_byte[2] = 0X3F & character;
            unicode = (unicode_byte[0] << 12) | (unicode_byte[1]<<6)| unicode_byte[2];
            break;
        case 4:
            unicode_byte[0] = 0x07 & (character >>24);
            unicode_byte[1] = 0X3F & (character >> 16);
            unicode_byte[2] = 0X3F & (character >> 8);
            unicode_byte[3] = 0X3F & character;
            unicode = (unicode_byte[0] << 18) | (unicode_byte[1] << 12) | (unicode_byte[2]<<6)| unicode_byte[3];
            break;
        default:
            break;
    }

    return unicode;
}


void GetUTF8StringUnicode(char* character,uint32_t length,uint32_t Unicode[]) {
    uint32_t cnt = 0;
    for (uint32_t i=0;i<length;i++) {
        if (character[i]<128) {
            Unicode[cnt++] =UTF8_Switch_Unicode(character[i]);
            continue;
        }
        if (character[i]<0xE0&&character[i]>0xBF) {
            Unicode[cnt++] =UTF8_Switch_Unicode(character[i]<<8|character[i+1]);
            i++;continue;
        }
        if (character[i]<0xF0&&character[i]>0xDF) {
            Unicode[cnt++] =UTF8_Switch_Unicode(character[i]<<16|character[i+1]<<8|character[i+2]);
            i+=2;continue;
        }
        if (character[i]<0xF8&&character[i]>0xEF) {
             Unicode[cnt++] = UTF8_Switch_Unicode(character[i]<<24|character[i+1]<<16|character[i+2]<<8|character[i+3]);
            i+=3;
        }
    }
}

void GetByteOffSet(uint32_t Unicode[],uint32_t length,uint32_t offset[],uint8_t BytePerCharacter) {
    for (uint32_t i=0;i<length;i++) {
        offset[i] =Unicode[i]*BytePerCharacter;
    }
}