#include "header.h"

Color palette[256];

uint8_t normalizeColor(uint8_t colorValue) {
    return (colorValue * 255) / 63;
}

void readColorPalette(uint16_t colorPaletteOffset, uint8_t *FileData) {
    for ( int i = 0 ; i < 256 ; i++ ) {
        palette[i].r = normalizeColor( *(FileData + colorPaletteOffset + 0 + (3*i)) );
        palette[i].g = normalizeColor( *(FileData + colorPaletteOffset + 1 + (3*i)) );
        palette[i].b = normalizeColor( *(FileData + colorPaletteOffset + 2 + (3*i)) );
    }
}

void readFrame(uint8_t *targetBMP, uint8_t *FileData, uint32_t offset, FrameInfo Metadata, uint8_t isInitialFrame) {
    
    uint8_t currentByte, colorByte;
    uint32_t repeatCount, literalCount;
    uint32_t readPos = 0, writePos = 0;
    
    while ( readPos <= Metadata.size ) {

        currentByte = *(FileData + offset + readPos);

        if ( (currentByte & 0b11000000) == 0b01000000 ) {
            repeatCount = ( (currentByte & 0b00111111) << 8 ) + *(FileData + offset + readPos + 1);
            readPos += 2;
            colorByte = *(FileData + offset + readPos);

            for ( int i = 0 ; i < repeatCount ; i++ ) {
                if ( colorByte != 0b00000000 ) {
                    *(targetBMP + 0 + (3*writePos)) = palette[colorByte].b;
                    *(targetBMP + 1 + (3*writePos)) = palette[colorByte].g;
                    *(targetBMP + 2 + (3*writePos)) = palette[colorByte].r;
                    writePos++;
                }
                else {
                    writePos++;
                }
            }
            readPos++;
        }

        else if ( (currentByte & 0b11000000) == 0b11000000 ) {
            literalCount = ( (currentByte & 0b00111111) << 8 ) + *(FileData + offset + readPos + 1);
            readPos += 2;

            for ( int i = 0 ; i < literalCount ; i++ ) {
                colorByte = *(FileData + offset + readPos);
                if ( colorByte != 0b00000000 ) {
                    *(targetBMP + 0 + (3*writePos)) = palette[colorByte].b;
                    *(targetBMP + 1 + (3*writePos)) = palette[colorByte].g;
                    *(targetBMP + 2 + (3*writePos)) = palette[colorByte].r;
                    writePos++;
                    readPos++;
                }
                else {
                    writePos++;
                    readPos++;
                }
            }
        }

        else if ( (currentByte & 0b11000000) == 0b10000000 ) {
            literalCount = (currentByte & 0b00111111);
            readPos++;
            for ( int i = 0 ; i < literalCount ; i++ ) {
                colorByte = *(FileData + offset + readPos);
                if ( colorByte != 0b00000000 ) {
                    *(targetBMP + 0 + (3*writePos)) = palette[colorByte].b;
                    *(targetBMP + 1 + (3*writePos)) = palette[colorByte].g;
                    *(targetBMP + 2 + (3*writePos)) = palette[colorByte].r;
                    writePos++;
                    readPos++;
                }
                else {
                    writePos++;
                    readPos++;
                }
            }
        }

        else {
            repeatCount = currentByte;

            readPos++;
            for ( int i = 0 ; i < repeatCount ; i++ ) {
                colorByte = *(FileData + offset + readPos);
                if ( colorByte != 0b00000000 ) {
                    *(targetBMP + 0 + (3*writePos)) = palette[colorByte].b;
                    *(targetBMP + 1 + (3*writePos)) = palette[colorByte].g;
                    *(targetBMP + 2 + (3*writePos)) = palette[colorByte].r;
                    writePos++;
                }
                else {
                    writePos++;
                }
            }
            readPos++;
        }
        
    }

}
