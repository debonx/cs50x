#include <stdio.h>
#include <cs50.h>

/**
 * Print spaces if > 1
 * @param int curr - current value
 * @param int lenght - max lenght of line and piramid
 */
void print_spaces(int curr, int length)
{
    if (curr < 1)
    {
        return;
    }
    int spaces = length - curr;
    for (int i = 0; i < spaces; i++)
    {
        printf(" ");
    }
}

/**
 * Print the right hashes
 * @param n - number of hashes to print
 */
void print_hashes(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("#");
    }
}

/**
 * Print the middle spaces
 */
void print_middle(void)
{
    printf("  ");
}

/**
 * Print end of line
 */
void print_endline(void)
{
    printf("\n");
}

/**
 * Compose piramid
 * @param int height - height of piramid
 */
void piramid(int height)
{
    for (int i = 1; i <= height; i++)
    {
        print_spaces(i, height);
        print_hashes(i);
        print_middle();
        print_hashes(i);
        print_endline();
    }
}

/**
 * Initiating program
 */
int main(void)
{
    int height = get_int("Height:");
    if (height < 1 || height > 8)
    {
        main();
    }
    else
    {
        piramid(height);
    }
}