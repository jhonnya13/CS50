#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    // Check command line argument
    if (argc != 2)
    {
        printf("Wrong command. Please provide name of the file\n");
        return 1;
    }

    // Open memory card
    FILE *raw_file = fopen(argv[1], "r");
    if (raw_file == NULL)
    {
        printf("Can't read %s\n", argv[1]);
        return 1;
    }

    // Establish variables
    uint8_t buffer[BLOCK_SIZE]; // from notes
    bool jpeg_found = false;    // to return true if found
    int jpeg_count = 0;         // count jpegs
    char jpeg_name[8];          // array to store file name of current jpeg
    FILE *out_pointer = NULL;   // Pointer to the current jpeg file

    // Read file block by block
    while (fread(buffer, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        // Look for JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0) // 4th byte in a range from 0xf0 to 0xe0
        {
            // Close JPEG if open;
            if (jpeg_found)
            {
                fclose(out_pointer);
            }
            else
            {
                jpeg_found = true;
            }

            // Generate JPEG name and open jpeg
            sprintf(jpeg_name, "%03d.jpg", jpeg_count); // Generate a name
            out_pointer = fopen(jpeg_name, "w");        // Open jpeg

            // Print an error if JPEG can't be created
            if (out_pointer == NULL)
            {
                fclose(raw_file);
                printf("Jpeg %s couldn't be created\n", jpeg_name);
                return 3;
            }
            jpeg_count++;
        }

        // Write JPEG
        if (jpeg_found == true)
        {
            fwrite(buffer, BLOCK_SIZE, 1, out_pointer);
        }
    }

    // Close forenstic image
    fclose(raw_file);
    if (jpeg_found)
    {
        fclose(out_pointer);
    }

    // Exit
    return 0;
}