#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static uint8_t *FileData;
extern uint8_t *targetBMP;

struct Header {
    uint8_t rawData[118];
    uint16_t version;
    uint16_t frameCount;
    uint16_t offsetColorPalette;
    uint16_t offsetBlock1;
    uint16_t offsetBlock2;
    uint16_t offsetBlock3;
    uint16_t expectedResX;
    uint16_t expectedResY;
};

struct FrameInfo {
    uint32_t index;
    uint32_t resX;
    uint32_t resY;
    uint32_t startOffsetByte;
    uint32_t size;
    uint32_t block1Data;
};

struct Color {
    uint8_t b;
    uint8_t g;
    uint8_t r;
};

struct bmpFileHeader {
    // Standard 14-byte BMP file header
    // See BITMAPCOREHEADER for reference
    uint16_t headerField;
    uint32_t sizeOfFile;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offsetImageData;

    // 40-byte DIB header
    // See BITMAPINFOHEADER for reference
    uint32_t sizeOfHeader;
    uint32_t width;
    uint32_t height;
    uint16_t colorPlanesCount;
    uint16_t bitsPerPixel;
    uint32_t compressionMethod;
    uint32_t imageSize;
    uint32_t horizontalPPM;
    uint32_t verticalPPM;
    uint32_t paletteSize;
    uint32_t colorsUsedCount;
} __attribute__((packed, aligned(1)));

typedef struct FrameInfo FrameInfo;
typedef struct Header Header;
typedef struct Color Color;
typedef struct bmpFileHeader bmpFileHeader;

static Header inputHeader;
extern Color palette[256];

uint16_t to16(uint8_t byte1, uint8_t byte2);
uint32_t to32Auto(uint8_t *data, uint32_t offset);

uint8_t normalizeColor(uint8_t colorValue);
void readColorPalette(uint16_t colorPaletteOffset, uint8_t *FileData);
void readFrame(uint8_t *targetBMP, uint8_t *FileData, uint32_t offset, FrameInfo Metadata, uint8_t isInitialFrame);
void bmpWrite(FrameInfo Metadata, uint8_t *inputData, char *outputFileName);
