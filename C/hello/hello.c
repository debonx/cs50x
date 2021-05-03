#include <stdio.h>
#include <cs50.h>

/**
 * @param void
 * @return int
 **/
int main(void)
{
    string name = get_string("What's your name?\n");
    printf("hello, %s\n", name);
}