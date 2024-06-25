#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

int main(void)

{
    float L = 100/3;
    float S = 100/2;
    double index = (0.0588 * L - 0.296 * S - 15.8);
    index = round(index);
    if (index > 16)
    {
        index = 16;
        sprintf(s_grade, "%i+", index);
    }


    float L = (count_letters(text)/count_words(text)) * 100.0 ;
    printf("%f L\n", L);
    float S = (count_sentences(text)/count_words(text)) * 100.0;
    printf("%f S\n", S);
    float index = 0.0588 * L - 0.296 * S - 15.8;
    printf("%f INDEX\n", index);
    int grade = round(index);
    printf("%i GRRRADE\n", grade);
    if (grade > 16)
    {
        grade = 16;
    }
    return grade;
}
