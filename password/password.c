// Check that a password has at least one lowercase letter, uppercase letter, number and symbol
// Practice iterating through a string
// Practice using the ctype library

#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

bool valid(string password);

int main(void)
{
    string password = get_string("Enter your password: ");
    if (valid(password))
    {
        printf("Your password is valid!\n");
    }
    else
    {
        printf("Your password needs at least one uppercase letter, lowercase letter, number and symbol\n");
    }
}

// TODO: Complete the Boolean function below
bool valid(string password)
{
    int lenght = strlen(password);
    bool isUpper, isLower, isLetter, isNumber, isSymbol = false;

    for (int i = 0; i < lenght; i++)
    {
        // check if Upper case exists
        char x = password[i];
        if (isupper(x))
        {
            isUpper = true;
        }
        if (islower(x))
        {
            isLower = true;
        }
        if (isalpha(x))
        {
            isLetter = true;
        }
        if (isdigit(x))
        {
            isNumber = true;
        }
        if (ispunct(x))
        {
            isSymbol = true;
        }
    }
    if (isUpper && isLower && isLetter && isNumber && isSymbol == true)
    {
        return true;
    }
    return false;
}
