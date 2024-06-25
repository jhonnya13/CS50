#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // TODO: Prompt for start size
    int start_size;
    do
    {
        printf("Start size can't be less than 9\n");
        start_size = get_int("Start size: ");
    }
    while (start_size < 9);

    // TODO: Prompt for end size
    int end_size;
    do
    {
        printf("End size can't be less than start size\n");
        end_size = get_int("End size: ");
    }
    while (end_size < start_size);

    // TODO: Calculate number of years until we reach threshold
    int years = 0;
    int original_size = start_size;

    if (start_size == end_size)
    {
        years = 0;
    }
    while (start_size < end_size)
    {
        int gain = (start_size / 3);
        int lose = (start_size / 4);
        start_size = start_size + gain - lose;
        years++;
    }
    printf("Years: %i", years);

    // TODO: Print number of years
}
