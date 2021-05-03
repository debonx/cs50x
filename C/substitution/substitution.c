#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int key_evaluation(string key)
{
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    int count = 0;
    for (int i = 0; i < strlen(alphabet); i++)
    {
        for (int n = 0; n < strlen(key); n++)
        {
            // If invalid character
            if (isalpha(key[n]) == 0)
            {
                return 1;
            }
            if (tolower(key[n]) == alphabet[i])
            {
                count++;
            }
        }
        // if duplicate character
        if (count > 1)
        {
            return 2;
        }
        count = 0;
    }
    return 0;
}

int main(int argc, string argv[])
{
    // Bail if not 2 args
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Bail if not 26 chharacters
    if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // Key evaluation
    int evaluate = key_evaluation(argv[1]);

    // Bail for invalid characters
    if (evaluate == 1)
    {
        printf("There are invalid characters in key.\n");
        return 1;
    }

    // Bail for duplicates
    if (evaluate == 2)
    {
        printf("There are duplicate characters in key.\n");
        return 1;
    }

    // Get the text
    string text = get_string("plaintext: ");

    // Set cipher text + 1 char for null and key
    char cipher[strlen(text) + 1];
    string key = argv[1];

    // statically define lower and upper alphabet
    string lowAlphabet = "abcdefghijklmnopqrstuvwxyz";
    string upAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // perform comparision and fill the cipher wihtout mutating the original
    for (int i = 0; i < (strlen(text) + 1); i++)
    {
        if (isalpha((char) text[i]) != 0)
        {
            for (int a = 0; a < strlen(lowAlphabet); a++)
            {
                if ((char) text[i] == (char) lowAlphabet[a])
                {
                    cipher[i] = (char) tolower(key[a]);
                }
                else if ((char) text[i] == (char) upAlphabet[a])
                {
                    cipher[i] = (char) toupper(key[a]);
                }
                continue;
            }
        }
        else
        {
            cipher[i] = (char) text[i];
        }
        continue;

        // Null character at last
        if (i == strlen(text))
        {
            cipher[i] = '\0';
        }
    }
    printf("ciphertext: %s\n", cipher);
    return 0;
}