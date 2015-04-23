#include <iostream>
#include <stdlib.h>
#include "tpx_raw.h"

#define NO_ERROR 0
#define ARGS_ERROR -1
#define READ_ERROR -2

void reorderPixels(i16 *pInput, i16 *pOutput) {
    int b=0;
    for(int r=0; r<256; r++) {
        for(int c=0; c<256; c++) {
            pOutput[b]=pInput[c+r*256];
            pOutput[256+b]=pInput[256*256+c+r*256];
            pOutput[256*512+b]=pInput[256*512+256*256+(255-c)+(255-r)*256];
            pOutput[256*512+256+b]=pInput[256*512+(255-c)+(255-r)*256];
            b++;
        }
        b+=256;
    }
}

int convertRawToBin(FILE *rawFile, FILE *binFile) {
    int frameCnt=0;
    u32 nbytes;
    int lostRows;
    u8 *rawData=NULL;
    i16 *data=NULL;
    i16 *rData=NULL;
    
    u32 rSz=0;
    int ret=NO_ERROR;
    
    MpxModule *mpx=new MpxModule(0, 3, 0, MPIX_ROWS, 0, NULL);

    while (!feof(rawFile)) {
        int ret=fread((void *) &lostRows, sizeof(int), 1, rawFile);
        if (feof(rawFile)) 
            break;

        if (ret!=1) {
            perror("reading lostRows failed");
            ret = READ_ERROR;
            break;
        }
        
        if (1 != fread((void *) &nbytes, sizeof(u32), 1, rawFile)) {
            perror("reading nbytes failed");
            ret = READ_ERROR;
            break;
        }

        
        if (rawData==NULL) {
            rawData=(u8*) calloc(nbytes,sizeof(u8));
            data=(i16*) calloc(nbytes/2,sizeof(i16));
            rData=(i16*) calloc(nbytes/2,sizeof(i16));
            rSz=nbytes;
        } else {
            if(rSz!=nbytes) {
                std::cerr << "Incompatible image sizes in one file" << std::endl;
                ret = READ_ERROR;
                break;
            }
        }
        if (nbytes != fread((void *) rawData, sizeof(u8), nbytes, rawFile)) {
            perror("reading raw data failed");
            ret = READ_ERROR;
            break;
        }
        
        mpx->parStream2Data(rawData, data);
        reorderPixels(data,rData);
        fwrite((void *) rData,sizeof(i16),nbytes/2,binFile);
        frameCnt++;
    }

    if(rawData != NULL) {
        free(rawData);
    }
    if (data != NULL) {
        free(data);
    }
    if (rData != NULL) {
        free(rData);
    }
    
    std::cout << frameCnt << " frame(s) found" << std::endl;
    return ret;
}

