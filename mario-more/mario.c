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

    // for each row
    for (int row = 1; row <= height; row++)
    {
        // print dots
        for (int dots = height; dots > row; dots--)
        {
            printf(" ");
        }

        // print first pyramid
        for (int hash = 0; hash < row; hash++)
        {
            printf("#");
        }

        // print spaces
        for (int spaces = 2; spaces > 0; spaces--)
        {
            printf(" ");
        }

        // print hashes
        // number of hashes == on what row
        for (int hash = 0; hash < row; hash++)
        {
            printf("#");
        }
        printf("\n");
    }
}