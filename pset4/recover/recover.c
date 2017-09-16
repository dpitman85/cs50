#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    //check for proper usage
    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./recover infile\n");
        return 1;
    }

    //remember filename
    char *inFile = argv[1];

     //define counters and buffers
    int counter = 0;
    int blockReturn = 0;
    char jpgName[20];
    uint8_t blockStore[512];
    bool writing = false;
    FILE * jpeg;

    //open input file
    FILE *raw = fopen(inFile, "r");
    if(raw == NULL)
    {
        fprintf(stderr, "%s could not be opened\n", inFile);
        return 2;
    }

    do{
        //Read block and remember number of returned bytes
        blockReturn = fread(&blockStore, 1, 512, raw);

        //Check for jpeg
        if (blockStore[0] == 0xff &&
            blockStore[1] == 0xd8 &&
            blockStore[2] == 0xff &&
            (blockStore[3] & 0xf0) == 0xe0)
        {
            if(writing)
            {
                fclose(jpeg);
            }

            //open first output file
            sprintf(jpgName, "%03i.jpg", counter);
            jpeg = fopen(jpgName, "w");
            counter++;

            //write current block to file
            fwrite(&blockStore, 1, 512, jpeg);

            //indicate that a jpeg has been found
            writing = true;
        } else if(writing)
        {
            //write current block to file
            fwrite(&blockStore, 1, 512, jpeg);
        }

    }   while(blockReturn == 512);

    //close files and return success
    fclose(jpeg);
    fclose(raw);
    return 0;
}