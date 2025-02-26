// main.c
#include "../inc/img.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage(const char *program_name) {
    printf("\n     \033[1;33mPIXELANS - Pixel Art Converter\033[0m\n\n");
    printf("Usage: %s <image_file> [options]\n\n", program_name);
    printf("Options:\n");
    printf("  -w, --width WIDTH     Width of ASCII output (default: %d)\n",
           DEFAULT_WIDTH);
    printf("  -p, --padding SPACES  Left padding spaces (default: %d)\n",
           PADDING_LEFT);
    printf("  -c, --charset TYPE    Character set: dense (d), medium (m), "
           "sparse (s) (default: d)\n");
    printf("  -i, --invert          Invert the ASCII mapping\n");
    printf("  -n, --no-color        Disable color output\n");
    printf("  -h, --help            Display this help message\n\n");
}

int main(int argc, char *argv[]) {
    config_t config = {.width = DEFAULT_WIDTH,
                       .padding_left = PADDING_LEFT,
                       .invert = FALSE,
                       .no_color = FALSE,
                       .charset_type = 'd',
                       .image_file = NULL};

    static struct option long_options[] = {
        {"width", required_argument, NULL, 'w'},
        {"padding", required_argument, NULL, 'p'},
        {"charset", required_argument, NULL, 'c'},
        {"invert", no_argument, NULL, 'i'},
        {"no-color", no_argument, NULL, 'n'},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}};

    int opt;
    while ((opt = getopt_long(argc, argv, "w:p:c:ino:h", long_options, NULL)) !=
           -1) {

        switch (opt) {
        case 'w':
            config.width = atoi(optarg);
            if (config.width <= 0) {
                fprintf(stderr, "Error: width must be postive\n");
                print_usage(argv[0]);
                return IMG_ERROR_PARAM;
            }
            break;

        case 'p':
            config.padding_left = atoi(optarg);
            if (config.padding_left <= 0) {
                fprintf(stderr, "Error: padding must be postive\n");
                print_usage(argv[0]);
                return IMG_ERROR_PARAM;
            }
            break;

        case 'c':
            if (strlen(optarg) != 1 ||
                (optarg[0] != 'd' && optarg[0] != 'm' && optarg[0] != 's')) {
                fprintf(stderr, "Error: charset myt be d, m, or s\n");
                print_usage(argv[0]);
                return IMG_ERROR_PARAM;
            }
            config.charset_type = optarg[0];
            break;

        case 'i':
            config.invert = TRUE;
            break;

        case 'n':
            config.no_color = TRUE;
            break;

        case 'h':
            print_usage(argv[0]);
            return IMG_SUCCESS;

        default:
            print_usage(argv[0]);
            return IMG_ERROR_PARAM;
        }
    }

    if (optind < argc) {
        config.image_file = argv[optind];
    }

    if (config.image_file == NULL) {
        fprintf(stderr, "Error: no image file specified\n");
        print_usage(argv[0]);
        return IMG_ERROR_PARAM;
    }

    return convert_to_ascii(&config);
}
