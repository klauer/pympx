#ifndef _H_TPX_RAW
#define _H_TPX_RAW

#include <common.h>
#include <mpxmodule.h>

#define NO_ERROR 0
#define ARGS_ERROR -1
#define READ_ERROR -2
#define WRITE_ERROR -3

void reorderPixels(i16 *pInput, i16 *pOutput);
int convertRawToBin(FILE *rawFile, FILE *binFile);
int convertRawToTiff(FILE *rawFile, const char *fn);

#endif
