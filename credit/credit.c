#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long credit_card = get_long("Type you credit card number: \n");
    printf("%li \n", credit_card);
}