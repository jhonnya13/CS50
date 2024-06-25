#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int convert(string input);

int main(void)
{
    string input = get_string("Enter a positive integer: ");

    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (!isdigit(input[i]))
        {
            printf("Invalid Input!\n");
            return 1;
        }
    }

    // Convert string to int
    printf("%i\n", convert(input));
}

int convert(string input)
{
    // base case
    if (input[0] == '\0')
        return 0;

    // Start by getting the index of the last char in the string (the char before the \0).
    int index_last_char = strlen(input) - 1;

    // Convert this char into its numeric value. Can you subtract some char to do this?
    int digit = input[index_last_char] - '0';

    // Remove the last char from the string by moving the null terminator one position to the left.
    input[index_last_char] = '\0';

    // Return this value plus 10 times the integer value of the new shortened string.
    digit = convert(input) * 10 + digit;
    return digit;
}