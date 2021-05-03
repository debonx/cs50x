#include "helpers.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float avgGrey;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            avgGrey = round((image[j][i].rgbtRed + image[j][i].rgbtGreen + image[j][i].rgbtBlue) / 3.000);
            image[j][i].rgbtRed = avgGrey;
            image[j][i].rgbtGreen = avgGrey;
            image[j][i].rgbtBlue = avgGrey;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int sepiaRed;
    int sepiaGreen;
    int sepiaBlue;
    int max = 255;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            sepiaRed = round(0.393 * image[j][i].rgbtRed + 0.769 * image[j][i].rgbtGreen + 0.189 * image[j][i].rgbtBlue);
            sepiaGreen = round(0.349 * image[j][i].rgbtRed + 0.686 * image[j][i].rgbtGreen + 0.168 * image[j][i].rgbtBlue);
            sepiaBlue = round(0.272 * image[j][i].rgbtRed + 0.534 * image[j][i].rgbtGreen + 0.131 * image[j][i].rgbtBlue);
            image[j][i].rgbtRed = (sepiaRed > max) ? max : sepiaRed;
            image[j][i].rgbtGreen = (sepiaGreen > max) ? max : sepiaGreen;
            image[j][i].rgbtBlue = (sepiaBlue > max) ? max : sepiaBlue;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE *tmp = malloc(sizeof(RGBTRIPLE));
    if (tmp != NULL)
    {
        for (int i = 0; i < width / 2; i++)
        {
            for (int j = 0; j < height; j++)
            {
                *tmp = image[j][i];
                image[j][i] = image[j][width - i - 1];
                image[j][width - i - 1] = *tmp;
            }
        }
        free(tmp);
    }
}

bool pixelExists(int y, int x, int height, int width)
{
    if (x < 0 || y < 0)
    {
        return false;
    }
    if (x > width - 1)
    {
        return false;
    }
    if (y > height - 1)
    {
        return false;
    }
    return true;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    float pixels;
    int sumRed;
    int sumGreen;
    int sumBlue;
    RGBTRIPLE original[height][width];

    //copy image
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            original[j][i].rgbtRed = image[j][i].rgbtRed;
            original[j][i].rgbtGreen = image[j][i].rgbtGreen;
            original[j][i].rgbtBlue = image[j][i].rgbtBlue;
        }
    }

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            pixels = 0.00;
            sumRed = 0;
            sumGreen = 0;
            sumBlue = 0;

            if (pixelExists(j - 1, i - 1, height, width)) // Check top left
            {
                sumRed += original[j - 1][i - 1].rgbtRed;
                sumGreen += original[j - 1][i - 1].rgbtGreen;
                sumBlue += original[j - 1][i - 1].rgbtBlue;
                pixels++;
            }

            if (pixelExists(j - 1, i, height, width)) // Check top center
            {
                sumRed += original[j - 1][i].rgbtRed;
                sumGreen += original[j - 1][i].rgbtGreen;
                sumBlue += original[j - 1][i].rgbtBlue;
                pixels++;
            }

            if (pixelExists(j - 1, i + 1, height, width)) // Check top right
            {
                sumRed += original[j - 1][i + 1].rgbtRed;
                sumGreen += original[j - 1][i + 1].rgbtGreen;
                sumBlue += original[j - 1][i + 1].rgbtBlue;
                pixels++;
            }

            if (pixelExists(j, i - 1, height, width)) // Check center left
            {
                sumRed += original[j][i - 1].rgbtRed;
                sumGreen += original[j][i - 1].rgbtGreen;
                sumBlue += original[j][i - 1].rgbtBlue;
                pixels++;
            }

            if (pixelExists(j, i + 1, height, width)) // Check center right
            {
                sumRed += original[j][i + 1].rgbtRed;
                sumGreen += original[j][i + 1].rgbtGreen;
                sumBlue += original[j][i + 1].rgbtBlue;
                pixels++;
            }

            if (pixelExists(j + 1, i - 1, height, width)) // Check bottom left
            {
                sumRed += original[j + 1][i - 1].rgbtRed;
                sumGreen += original[j + 1][i - 1].rgbtGreen;
                sumBlue += original[j + 1][i - 1].rgbtBlue;
                pixels++;
            }

            if (pixelExists(j + 1, i, height, width)) // Check bottom center
            {
                sumRed += original[j + 1][i].rgbtRed;
                sumGreen += original[j + 1][i].rgbtGreen;
                sumBlue += original[j + 1][i].rgbtBlue;
                pixels++;
            }

            if (pixelExists(j + 1, i + 1, height, width)) // Check bottom right
            {
                sumRed += original[j + 1][i + 1].rgbtRed;
                sumGreen += original[j + 1][i + 1].rgbtGreen;
                sumBlue += original[j + 1][i + 1].rgbtBlue;
                pixels++;
            }

            // Current pixel
            sumRed += original[j][i].rgbtRed;
            sumGreen += original[j][i].rgbtGreen;
            sumBlue += original[j][i].rgbtBlue;
            pixels++;


            image[j][i].rgbtRed = round(sumRed / pixels);
            image[j][i].rgbtGreen = round(sumGreen / pixels);
            image[j][i].rgbtBlue = round(sumBlue / pixels);
        }
    }
}