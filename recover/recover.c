#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    if (argc != 2) // Check argc valid
    {
        printf("You need to provide card name\n");
    }

    // Open memory card
    FILE *ptr_file = fopen(argv[1], "r"); // Open file with pointer to read
    if (ptr_file == NULL) // Check if adress not NULL
    {
        printf("Can't read file\n");
        return 1;
    }

    // Decalre variables
    int jpeg_count = 0;
    bool jpeg_found = false;
    FILE *ptr_image = NULL;
    char *jpeg_name = malloc(8 * sizeof(char));  // create array to store the name of file
    uint8_t *buffer = malloc(BLOCK_SIZE);      // create an array to store read bytes
    if (buffer == NULL)
    {
        fclose(ptr_file);
        printf("Couldn't allocate memory");
        return 2;
    }




    while (fread(buffer, 1, BLOCK_SIZE, ptr_file) == BLOCK_SIZE) // While not end of the file Read 512 bytes using fread into buffer
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0) // If start of new JPEG
        {

            // Close JPEG if open;
            if (jpeg_found)
            {
                fclose(ptr_image);
            }
            else
            {
                jpeg_found = true;
            }

            sprintf(jpeg_name, "%03i.jpg", jpeg_count);    // generate a name
            ptr_image = fopen(jpeg_name, "w");    // create jpeg
            jpeg_count ++;
        }

        if (jpeg_found == true)
        {
            fwrite(buffer, BLOCK_SIZE, 1, ptr_image);  //write
        }

    }

    fclose(ptr_image);
    fclose(ptr_file);
    free(jpeg_name);
    free(buffer);

    return 0;
}
