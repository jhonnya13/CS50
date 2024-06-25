#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Get user height for the pyramid
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // Define width
    int width = height;
    //string gap_size = "  ";

    // Print first smallest row
    // Print next row (height - 1)
    // Print row

    int i = 1;
    for (int x = 0; x < height; x++)
    {
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }
        printf("\n");
        i ++;
    }


}