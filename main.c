/// @file main.c
#include <stdio.h>
#include <math.h>
#define ACCURACY 162
#define GREEN_DIFF 30

#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"

unsigned char *uc_arrayNew_1d(int _size) {
    return (unsigned char *)calloc(_size, sizeof(unsigned char));
}

unsigned char *image_subtraction(unsigned char *background_image, unsigned char *foreground_image, int width, int height, int channel) {
    unsigned char *result = uc_arrayNew_1d(width * height * channel);
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int index = i * width * channel + j * channel;
            int sum = 0;
            for (int k = 0; k < channel; k++) {
                sum += abs((int)(foreground_image[index + k] - background_image[index + k]));
            }
            
            if (sum > ACCURACY && (foreground_image[index + 1] - GREEN_DIFF) < (foreground_image[index]/2 + foreground_image[index+2]/2)) {
                for (int k = 0; k < channel; k++) {
                    result[index+k] = foreground_image[index+k];
                }
            }
        }
    }


    return result;
}

unsigned char *image_overlay(unsigned char *image, unsigned char *background, int width, int height, int channel) {
    unsigned char *result = uc_arrayNew_1d(width * height * channel);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < channel; k++) {
                int index = i * width * channel + j * channel + k;
                if (image[index] == 0) {
                    result[index] = background[index];
                } else {
                    result[index] = image[index];
                }
            }
        }
    }

    return result;
}

int main() {
    //declare variables
    int width, height, channel;
    char background_path[] = "./images/background.jpg";
    char foreground_path[] = "./images/foreground.jpg";
    char new_background_path[] = "./images/new_background.jpg";
    char save_path[] = "./images/test.jpg";

    //read images
    printf("Loading background image...\n");
    unsigned char *bg_image = stbi_load(background_path, &width, &height, &channel, 0);
    if (bg_image == NULL) {
        printf("\nError in loading the image\n");
        exit(1);
    }
    printf("Width = %d\nHeight = %d\nChannel = %d\n\n", width, height, channel);

    printf("Loading foreground image...\n");
    unsigned char *fg_image = stbi_load(foreground_path, &width, &height, &channel, 0);
    if (fg_image == NULL) {
        printf("\nError in loading the image\n");
        exit(1);
    }
    printf("Width = %d\nHeight = %d\nChannel = %d\n\n", width, height, channel);

    printf("Loading new background image...\n");
    unsigned char *nbg_image = stbi_load(new_background_path, &width, &height, &channel, 0);
    if (nbg_image == NULL) {
        printf("\nError in loading the image\n");
        exit(1);
    }
    printf("Width = %d\nHeight = %d\nChannel = %d\n\n", width, height, channel);

    //substract foreground from background
    unsigned char *substracted_image = image_subtraction(bg_image, fg_image, width, height, channel);

    //insert new background
    unsigned char *result = image_overlay(substracted_image, nbg_image, width, height, channel);

    //save image
    stbi_write_png(save_path, width, height, channel, result, width * channel);
    printf("New image successfully saved to %s\n", save_path);

    return 0;
}