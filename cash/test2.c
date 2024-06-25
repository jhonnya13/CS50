#include <cs50.h>
#include <stdio.h>

int get_cents(void);
int calculate_quarters(int cents);
int calculate_dimes(int cents);
int calculate_nickels(int cents);
int calculate_pennies(int cents);

int main(void)
{
    int cents = 49;
    int quarter = 25;
    int how_many = cents / quarter;
    printf("%i\n", how_many);
}