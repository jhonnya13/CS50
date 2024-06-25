#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text: ");
    float letters = count_letters(text);
    float words = count_words(text);
    float sentences = count_sentences(text);

    float L = (letters / words) * 100.0;
    float S = (sentences / words) * 100.0;
    float index = 0.0588 * L - 0.296 * S - 15.8;
    int grade = round(index);

    // Print Grade
    if (grade > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        if (grade < 1)
        {
            printf("Before Grade 1\n");
        }
        else
        {
            printf("Grade %i\n", grade);
        }
    }
}

int count_letters(string text)
{
    int letters = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        char x = text[i];
        if (isalpha(x))
        {
            letters++;
        }
    }
    return letters;
}

int count_words(string text)
{
    int words = 1;
    for (int i = 0; i < strlen(text); i++)
    {
        char x = text[i];
        if (isspace(x))
        {
            words++;
        }
    }
    return words;
}

int count_sentences(string text)
{
    int sentences = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        char x = text[i];
        if (x == '.' || x == '!' || x == '?')
        {
            sentences++;
        }
    }
    return sentences;
}