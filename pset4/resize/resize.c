/**
 * Copies a BMP piece by piece, just because.
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    int f = atoi(argv[1]);

    // ensure proper usage
    if (argc != 4 || f < 0 || f > 100)
    {
        fprintf(stderr, "Usage: ./resize factor infile outfile (factor must be 0.0 to 100.0)\n");
        return 1;
    }

    // remember filenames and resize factor
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // determine old padding & dimensions for scanlines
    int paddingIn = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int widthIn = bi.biWidth;
    int heightIn = bi.biHeight;

    // Determine new dimensions for bitmap
    bi.biWidth *= f;
    bi.biHeight *= f;

    // determine new padding for scanlines
    int paddingOut = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // find new size for header
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + paddingOut) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(heightIn); i < biHeight; i++)
    {
        for(int x = 0; x < f;x++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < widthIn; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                for(int n = 0; n < f; n++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // add output scanline padding
            for (int k = 0; k < paddingOut; k++)
            {
                fputc(0x00, outptr);
            }

            if(x < f-1)
            {
                //return cursor to beginning of input scanline
                fseek(inptr, widthIn * -3, SEEK_CUR);
            } else
            {
                // skip over padding, if any
                fseek(inptr, paddingIn, SEEK_CUR);
            }
        };
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
