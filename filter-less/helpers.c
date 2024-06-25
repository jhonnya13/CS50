#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            double average = (image[h][w].rgbtBlue + image[h][w].rgbtGreen + image[h][w].rgbtRed) / 3.0;
            image[h][w].rgbtBlue = round(average);
            image[h][w].rgbtGreen = round(average);
            image[h][w].rgbtRed = round(average);
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            double sepia_Red = 0.393 * image[h][w].rgbtRed + 0.769 * image[h][w].rgbtGreen + 0.189 * image[h][w].rgbtBlue;
            double sepia_Green = 0.349 * image[h][w].rgbtRed + 0.686 * image[h][w].rgbtGreen + 0.168 * image[h][w].rgbtBlue;
            double sepia_Blue = 0.272 * image[h][w].rgbtRed + 0.534 * image[h][w].rgbtGreen + 0.131 * image[h][w].rgbtBlue;

            if (sepia_Red > 255)
            {
                sepia_Red = 255;
            }
            if (sepia_Green > 255)
            {
                sepia_Green = 255;
            }
            if (sepia_Blue > 255)
            {
                sepia_Blue = 255;
            }
            image[h][w].rgbtRed = round(sepia_Red);
            image[h][w].rgbtBlue = round(sepia_Blue);
            image[h][w].rgbtGreen = round(sepia_Green);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width / 2; w++)
        {
            RGBTRIPLE temp = image[h][w];
            image[h][w] = image[h][width - (w + 1)];
            image[h][width - (w + 1)] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create copy of image to use as unblured version to take pixels from
    RGBTRIPLE copy[height][width];
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            copy[row][column] = image[row][column];
        }
    }

    // Itterate thro each row
    for (int row = 0; row < height; row++)
    {
        // Each pixel in a row
        for (int column = 0; column < width; column++)
        {
            float redsum = 0;
            float bluesum = 0;
            float greensum = 0;
            int counter = 0;

            // Check columns around from -1 to 1 (on left and on the right)
            for (int i = -1; i < 2; i++)
            {
                // Check pixels around from - 1 to 1 (on top and bottom)
                for (int j = -1; j < 2; j++)
                {
                    // Check if outside row
                    if (row + i < 0 || row + i >= height)
                    {
                        continue;
                    }

                    // Check if outside column
                    if (column + j < 0 || column + j >= width)
                    {
                        continue;
                    }

                    // If exists add to sum
                    redsum += copy[row + i][column + j].rgbtRed;
                    bluesum += copy[row + i][column + j].rgbtBlue;
                    greensum += copy[row + i][column + j].rgbtGreen;

                    // Count how many pixels were
                    counter++;
                }
            }

            // Calculate average
            image[row][column].rgbtRed = round(redsum / counter);
            image[row][column].rgbtBlue = round(bluesum / counter);
            image[row][column].rgbtGreen = round(greensum / counter);
        }
    }
    return;
}
