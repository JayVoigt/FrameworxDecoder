#include "header.h"

int main ( int argc, char *argv[] ) {
    
    if ( argc < 3 ) {
        fprintf(stderr, "Not enough parameters. Usage: ./FrameworxDecoder <input> <output> <output.bmp>\n");
        exit(1);
    }

    FILE *sourceFile = fopen(argv[1], "rb");
    if ( sourceFile == NULL ) {
        fprintf(stderr, "Unable to read file.\n");
        exit(1);
    }

    fseek(sourceFile, 0, SEEK_END);
    uint32_t fileSize = ftell(sourceFile);
    fseek(sourceFile, 0, SEEK_SET);

    fread(inputHeader.rawData, 1, 118, sourceFile);

    if ( memcmp(inputHeader.rawData, "*ANI", 4) != 0 ) {
        fprintf(stderr, "File is not of valid type .ANI/.SLD.\n");
        fclose(sourceFile);
        exit(1);
    }

    FileData = malloc(fileSize);
    fseek(sourceFile, 0, SEEK_SET);
    fread(FileData, 1, fileSize, sourceFile);
    
    uint8_t *hd = inputHeader.rawData;
    inputHeader.version =               to16(hd[0x4], hd[0x5]);
    inputHeader.frameCount =            to16(hd[0xC], hd[0xD]);
    inputHeader.offsetColorPalette =    to16(hd[0x14], hd[0x15]);
    inputHeader.offsetBlock1 =          to16(hd[0x10], hd[0x11]);
    inputHeader.offsetBlock2 =          to16(hd[0x18], hd[0x19]);
    inputHeader.offsetBlock3 =          to16(hd[0x1C], hd[0x1D]);
    inputHeader.expectedResX =          to16(hd[0x28], hd[0x29]);
    inputHeader.expectedResY =          to16(hd[0x2C], hd[0x2D]);

    readColorPalette(inputHeader.offsetColorPalette, FileData);

    FrameInfo Metadata;
    Metadata.resX = inputHeader.expectedResX;
    Metadata.resY = inputHeader.expectedResY;

    FrameInfo FrameList[inputHeader.frameCount];

    for ( int i = 0 ; i < inputHeader.frameCount ; i++ ) {
        FrameList[i].index = i;
        FrameList[i].resX = inputHeader.expectedResX;
        FrameList[i].resY = inputHeader.expectedResY;
        FrameList[i].startOffsetByte = to32Auto( FileData, (inputHeader.offsetBlock2 + (4*i)) );
        FrameList[i].size = 
            to32Auto( FileData, (inputHeader.offsetBlock2 + (4 * (i+1))) ) - 
            to32Auto( FileData, (inputHeader.offsetBlock2 + (4 * i)) );
        FrameList[i].block1Data = to32Auto( FileData, (inputHeader.offsetBlock1 + (4*i)));
    }

    uint8_t *targetBMP = malloc(Metadata.resX * Metadata.resY);
    char outputFileName[32];

    for ( int i = 0 ; i < inputHeader.frameCount ; i++ ) {
        readFrame(targetBMP, FileData, FrameList[i].startOffsetByte, FrameList[i], 1);
        sprintf(outputFileName, "%s-%02d.bmp", argv[2], i);
        bmpWrite(Metadata, targetBMP, outputFileName);
    }

    free(targetBMP);
    free(FileData);
    fclose(sourceFile);
}
