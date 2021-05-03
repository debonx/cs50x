#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/*
* index = 0.0588 * L - 0.296 * S - 15.8
* L = is the average number of letters per 100 words
* S = the average number of sentences per 100 words in the text
**/
float get_index(string text)
{
    int n_words = 0;
    int n_sentences = 0;
    int n_letters = 0;
    float S = 0;
    float L = 0;
    float index = 0;
    int n = strlen(text);
    for (int i = 0; i < n; i++)
    {
        //counting and storing words
        if (text[i] != ' ' && text[i + 1] == ' ')
        {
            n_words++;
        }
        // counting sentences
        if (text[i] == '!' || text[i] == '.' || text[i] == '?')
        {
            n_sentences++;
        }
        if (isalpha(text[i]))
        {
            n_letters++;
        }
    }
    // final word
    if (text[n - 1] != ' ')
    {
        n_words++;
    }
    L = ((float) n_letters / (float) n_words) * 100;
    S = ((float) n_sentences / (float) n_words) * 100;
    index = (0.0588 * L) - (0.296 * S) - 15.8;
    return round(index);
}

int main(void)
{
    string text = get_string("String: ");
    int grade = (int) get_index(text);
    if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %d\n", (int) grade);
    }
    return 0;
}