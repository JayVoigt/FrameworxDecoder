#include "header.h"

void bmpWrite(FrameInfo Metadata, uint8_t *inputData, char *outputFileName) {
    FILE *bmpOut = fopen(outputFileName, "wb");

    if ( bmpOut == NULL ) {
        fprintf(stderr, "Unable to write BMP file. Exiting.");
        exit(1);
    }

    bmpFileHeader hc;
    hc.headerField = to16('B', 'M');
    hc.sizeOfFile = ((int)sizeof(bmpFileHeader) + (3 * Metadata.resX * Metadata.resY));
    hc.reserved1 = 0;
    hc.reserved2 = 0;
    hc.offsetImageData = (int)sizeof(bmpFileHeader);

    hc.sizeOfHeader = 40;
    hc.width = Metadata.resX;
    hc.height = (-1) * Metadata.resY;
    hc.colorPlanesCount = 1;
    hc.bitsPerPixel = 24;
    hc.compressionMethod = 0;
    hc.imageSize = (hc.width * (-1 * hc.height) * (hc.bitsPerPixel / 8));
    hc.horizontalPPM = 0;
    hc.verticalPPM = 0;
    hc.paletteSize = 0;
    hc.colorsUsedCount = 0;

    fwrite(&hc, sizeof(bmpFileHeader), 1, bmpOut);
    fwrite(inputData, 1, hc.imageSize, bmpOut);

    fclose(bmpOut);
}
