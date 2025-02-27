// img.c
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "../inc/stb_image.h"
#include "../inc/stb_image_resize.h"

#include "../inc/img.h"
#include <string.h>
#include <strings.h>

const char *ASCII_CHARS =
    "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";

static const char *get_charset(char charset_type) {
    switch (charset_type) {
    case 'd':
        return CHARSET_DENSE;
    case 'm':
        return CHARSET_MEDIUM;
    case 's':
        return CHARSET_SPARSE;
    default:
        return CHARSET_MEDIUM;
    }
}

static int load_image(const char *filename, img_t *img) {
    if (!filename || !img)
        return IMG_ERROR_PARAM;

    const int channels = STBI_rgb;
    img->data = stbi_load(filename, &img->width, &img->height, NULL, channels);
    if (img->data == NULL) {
        fprintf(stderr, "Error: failed to load the image: %s\n",
                stbi_failure_reason());
        return IMG_ERROR_FILE;
    }

    img->grayscale = NULL;
    return IMG_SUCCESS;
}

static int resize_img(img_t *img, int new_width) {
    if (new_width <= 0)
        return IMG_ERROR_PARAM;

    float aspect_ratio = (float)img->height / (float)img->width;
    int new_height = (int)(new_width * aspect_ratio * 0.55);

    if (new_height < 1)
        new_height = 1;

    if (new_height > MAX_HEIGHT) {
        new_height = MAX_HEIGHT;
        new_width = (int)(new_height / (aspect_ratio * 0.55));
    }

    // printf("Resizing image to %d x %d characters...\n", new_width,
    // new_height);

    unsigned char *resized_data =
        (unsigned char *)malloc(new_height * new_width * 3);
    if (resized_data == NULL) {
        fprintf(stderr, "Error: failed to allocate memory for resized image\n");
        return IMG_ERROR_MEM;
    }

    stbir_resize_uint8(img->data, img->width, img->height, 0, resized_data,
                       new_width, new_height, 0, 3);

    stbi_image_free(img->data);
    img->data = resized_data;
    img->width = new_width;
    img->height = new_height;

    return IMG_SUCCESS;
}

static int calculate_grayscale(img_t *img, bool_t invert) {
    img->grayscale = (unsigned char *)malloc(img->width * img->height);
    if (img->grayscale == NULL) {
        fprintf(stderr, "Error: failed to allocate memory for grayscale\n");
        return IMG_ERROR_MEM;
    }

    int pixels = img->width * img->height;
    int max_gray = 0, min_gray = 255;

    for (int i = 0; i < pixels; i++) {
        int idx = i * 3;
        unsigned char r = img->data[idx];
        unsigned char g = img->data[idx + 1];
        unsigned char b = img->data[idx + 2];

        unsigned char gray = (unsigned char)(0.299 * r + 0.587 * g + 0.114 * b);
        img->grayscale[i] = gray;

        if (gray < min_gray)
            min_gray = gray;
        if (gray > max_gray)
            max_gray = gray;
    }

    if (max_gray > min_gray) {
        for (int i = 0; i < pixels; i++) {
            img->grayscale[i] =
                (img->grayscale[i] - min_gray) * 255 / (max_gray - min_gray);

            if (invert) {
                img->grayscale[i] = 255 - img->grayscale[i];
            }
        }
    }

    // printf("Grayscale conversion complete. Range: %d - %d\n", min_gray,
    // max_gray);
    return IMG_SUCCESS;
}

static void print_ascii(img_t *img, const config_t *config) {
    const char *charset = get_charset(config->charset_type);
    int num_chars = strlen(charset);
    if (num_chars == 0)
        return;

    printf("\n\n");
    for (int y = 0; y < img->height; y++) {

        for (int p = 0; p < config->padding_left; p++) {
            printf("  ");
        }

        for (int x = 0; x < img->width; x++) {
            int pixel_idx = y * img->width + x;
            int rbg_idx = pixel_idx * 3;

            unsigned char grayscale_val = img->grayscale[pixel_idx];
            int char_idx = (grayscale_val * (num_chars - 1)) / 255;
            char ascii_char = charset[char_idx];

            if (config->no_color) {
                printf("%c", ascii_char);
            } else {
                unsigned char r = img->data[rbg_idx];
                unsigned char g = img->data[rbg_idx + 1];
                unsigned char b = img->data[rbg_idx + 2];

                printf("\033[38;2;%d;%d;%dm%c\033[0m", r, g, b, ascii_char);
            }
        }
        printf("\n");
    }

    printf("\n\n");
}

static void cleanup_img(img_t *img) {
    if (img == NULL)
        return;

    if (img->data) {
        free(img->data);
        img->data = NULL;
    }

    if (img->grayscale) {
        free(img->grayscale);
        img->grayscale = NULL;
    }
}

int convert_to_ascii(const config_t *config) {
    if (!config || !config->image_file)
        return IMG_ERROR_PARAM;

    img_t *img = (img_t *)malloc(sizeof(img_t));
    if (img == NULL) {
        fprintf(stderr, "Error: failed to allocate memory for img struct\n");
        return IMG_ERROR_MEM;
    }

    img->data = NULL;
    img->grayscale = NULL;

    int result;

    result = load_image(config->image_file, img);
    if (result != IMG_SUCCESS) {
        free(img);
        return result;
    }

    result = resize_img(img, config->width);
    if (result != IMG_SUCCESS) {
        cleanup_img(img);
        free(img);
        return result;
    }

    result = calculate_grayscale(img, config->invert);
    if (result != IMG_SUCCESS) {
        cleanup_img(img);
        free(img);
        return result;
    }

    print_ascii(img, config);

    cleanup_img(img);
    free(img);
    return IMG_SUCCESS;
}
