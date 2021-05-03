#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_card_length(long card)
{
    int length = 0;
    while (card > 0)
    {
        card /= 10;
        length++;
    }
    return length;
}

int get_card_first_n(long card, bool only_first)
{ 
    long n1 = card;
    long n2 = card;
    while (card)
    {
        n2 = n1; //last iteration has first 2 digits
        n1 = card; //last iteration has first digit
        card /= 10;
    }
    return (only_first) ? n1 : n2;
}

bool is_known_brand(string brand, long card)
{
    bool valid;
    int starting;

    //checking about starting numbers
    if (strcmp(brand, "AMEX") == 0)
    {
        starting = get_card_first_n(card, false);
        valid = (starting == 34 || starting == 37) ? true : false;
    }
    else if (strcmp(brand, "VISA") == 0)
    {
        starting = get_card_first_n(card, true);
        valid = (starting == 4) ? true : false;
    }
    else if (strcmp(brand, "MASTERCARD") == 0)
    {
        starting = get_card_first_n(card, false);
        valid = (starting == 51 
                 || starting == 52 
                 || starting == 53
                 || starting == 54 
                 || starting == 55) ? true : false;
    } 
    else
    {
        valid = false;
    }

    return valid;
}

string maybe_card_brand(long card)
{
    string maybe;

    int length = get_card_length(card);
    if (length == 15) //American Express
    {
        maybe = is_known_brand("AMEX", card) ? "AMEX" : "UKNOWN";
    }
    else if (length == 13) //Visa
    {
        maybe = is_known_brand("VISA", card) ? "VISA" : "UKNOWN";
    }
    else if (length == 16) //Mastercard or VISA
    {
        maybe = is_known_brand("MASTERCARD", card) ? "MASTERCARD" : "VISA";
        maybe = strcmp(maybe, "VISA") == 0
                ? is_known_brand("VISA", card)
                ? "VISA" 
                : "UKNOWN"
                : maybe;
    }
    else //Unknown
    {
        maybe = "UKNOWN";
    }

    return maybe;
}

int concat(int a, int b) 
{ 
  
    char s1[20]; 
    char s2[20]; 
  
    // Convert both the integers to string 
    sprintf(s1, "%d", a); 
    sprintf(s2, "%d", b); 
  
    // Concatenate both strings 
    strcat(s1, s2); 
  
    // Convert the concatenated string 
    // to integer 
    int c = atoi(s1); 
  
    // return the formed integer 
    return c;
}

int is_valid_sequence(long card)
{
    int sum = 0;

    for (int i = 0; card != 0; i++)
    {
        if (i % 2 == 0)
        {
            sum = sum + card % 10;
        }

        else
        {
            int num = 2 * (card % 10);
            sum = sum + num / 10 + num % 10;
        }
        card /= 10;
    }

    return (sum % 10) == 0 ? 1 : 0;
}

string get_card_type(long card)
{
    string type;
    string maybe;
    bool pass = false;

    maybe = maybe_card_brand(card);
    pass = is_valid_sequence(card);

    if (pass && strcmp(maybe, "UKNOWN") != 0)
    {
        type = maybe;
    }
    else
    {
        type = "INVALID";
    }

    return type;
}

int main(void)
{
    long card = get_long("Number: ");
    if (card == 0) //prevent 0 digit
    {
        main();
    }
    else
    {
        string card_type = get_card_type(card);
        printf("%s\n", card_type);
    }
    return 0;
}
