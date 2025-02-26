// img.h
#ifndef IMG_H
#define IMG_H

// Return codes
#define IMG_SUCCESS 0
#define IMG_ERROR_MEM 1
#define IMG_ERROR_FILE 2
#define IMG_ERROR_PARAM 3

// Default config values
#define DEFAULT_WIDTH 50
#define PADDING_LEFT 8
#define GAMMA 2.2
#define MAX_HEIGHT 300

// ASCII sets
#define CHARSET_DENSE                                                          \
    "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. "
#define CHARSET_MEDIUM "@%#*+=-:. "
#define CHARSET_SPARSE "@. "

// typedefs
typedef enum { FALSE = 0, TRUE = 1 } bool_t;

typedef struct {
    unsigned char *data, *grayscale;
    int width, height;
} img_t;

typedef struct {
    int width;
    int padding_left;
    bool_t invert;
    bool_t no_color;
    char charset_type;
    const char *image_file;
} config_t;

// funciton prototypes
int convert_to_ascii(const config_t *config);
void print_usage(const char *program_name);

#endif // !IMG_H
