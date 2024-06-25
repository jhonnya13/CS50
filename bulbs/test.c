#include <cs50.h>
#include <stdio.h>
#include <string.h>

const int BITS_IN_BYTE = 8;

void print_bulb(int bit);

int main(void)
{
    // TODO
    string message = get_string (" Message: ");
    int array[8];
    for (int i = 0; i < strlen(message); i++)
    {
        int x = message[i];
        // x = 97
        for (int j = 8; j > 0; j--)
        {
            if (x % 2 == 1)
            {
                array[j-1] = 1;
            }
            if (x % 2 == 0)
            {
                array[j-1] = 0;
            }
        x = x / 2;
        }
        for (int k = 0; k < 8; k++)
        {
            print_bulb(array[k]);
        }
    printf("\n");
    }
}

void print_bulb(int bit)
{
    if (bit == 0)
    {
        // Dark emoji
        printf("\U000026AB");
    }
    else if (bit == 1)
    {
        // Light emoji
        printf("\U0001F7E1");
    }
}
