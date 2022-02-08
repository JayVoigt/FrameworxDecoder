#include "header.h"

uint16_t to16(uint8_t byte1, uint8_t byte2) {
    return (byte2 << 8) | byte1;
}

uint32_t to32Auto(uint8_t *data, uint32_t offset) {
    return  ( *(data + offset + 3) << 24 ) |
            ( *(data + offset + 2) << 16 ) |
            ( *(data + offset + 1) << 8 ) |
            ( *(data + offset + 0) );
}
