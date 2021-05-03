#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Please used as following: ./recover card-to-recover-filename.raw");
        return 1;
    }

    // Card to open and read
    FILE *cardFile = fopen(argv[1], "r");
    if (!cardFile)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 1;
    }

    // hoisting
    FILE *outputImage = NULL;
    unsigned char bytes[512];
    int imageCount = 0;
    char imageFullName[8];
    int found = 0;

    while (fread(bytes, 512, 1, cardFile))
    {
        // check if it's image
        if (bytes[0] == 0xff
            && bytes[1] == 0xd8
            && bytes[2] == 0xff
            && (bytes[3] & 0xe0) == 0xe0)
        {
            // if found close it
            if (1 == found)
            {
                fclose(outputImage);
                found = 0;
            }

            // name it
            sprintf(imageFullName, "%03d.jpg", imageCount);
            outputImage = fopen(imageFullName, "w");
            if (outputImage == NULL)
            {
                fclose(outputImage);
                fprintf(stderr, "Could not create %s.\n", imageFullName);
                return 2;
            }

            // so far so good
            found = 1;
            imageCount++;
        }

        // if found start writing
        if (1 == found)
        {
            fwrite(&bytes, 512, 1, outputImage);
        }
    }

    // wrap up
    fclose(outputImage);
    fclose(cardFile);
    return 0;
}
