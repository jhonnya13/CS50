#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);
int get_channels(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    // TODO #1
    if (argc != 3)
    {
        printf("You need to provide input and output name\n");
        return 1;
    }

    // Open input file for reading
    // TODO #2
    FILE *ptr_file = fopen(argv[1], "r");
    if (ptr_file == NULL)
    {
        printf("Can't open file %s \n", argv[1]);
        return 2;
    }

    // Read header
    // TODO #3
    WAVHEADER header;
    fread(&header, 1, sizeof(WAVHEADER), ptr_file);

    // Use check_format to ensure WAV format
    // TODO #4
    if (!check_format(header))
    {
        printf("Not the right format\n");
        return 3;
    }

    // Open output file for writing
    // TODO #5
    FILE *ptr_file_out = fopen(argv[2], "w");
    if (ptr_file_out == NULL)
    {
        printf("Error writing the file\n");
        return 4;
    }

    // Write header to file
    // TODO #6
    fwrite(&header, 1, sizeof(WAVHEADER), ptr_file_out);

    // Use get_block_size to calculate size of block
    // TODO #7
    int block_size = get_block_size(header);

    // Write reversed audio to file
    // TODO #8

    if (fseek(ptr_file, block_size, SEEK_END))
    {
        return 1;
    }

    int *buffer = malloc(block_size);

    while (ftell(ptr_file) - block_size > sizeof(header)) // while current writing output didn't meet header
    {
        if (fseek(ptr_file, -2 * block_size, SEEK_CUR))
        {
            return 1;
        }
        fread(buffer, 1, block_size, ptr_file);      // Read a block (from back)
        fwrite(buffer, 1, block_size, ptr_file_out); // Write a block
    }

    fclose(ptr_file);
    fclose(ptr_file_out);
    free(buffer);

    return 0;
}

int get_channels(WAVHEADER header)
{
    int number = header.numChannels;
    return number;
}

int check_format(WAVHEADER header)
{
    // TODO #4
    if (header.format[0] == 'W' && header.format[1] == 'A' && header.format[2] == 'V' && header.format[3] == 'E')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int get_block_size(WAVHEADER header)
{
    // TODO #7
    // size of each block: number of channels multiplied by bytes per sample
    int block_size = header.numChannels * (header.bitsPerSample / 8);
    return block_size;
}