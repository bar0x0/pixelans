# Pixelans - Pixel Art Converter

Pixelans is a command-line tool that converts images into ASCII art, with options for color output, different character densities, and customizable dimensions.

## Installation

### Prerequisites

PIXELANS requires:
- A C compiler (gcc recommended)
- The stb_image and stb_image_resize libraries (included in the project)
- A terminal that supports ANSI color codes for full color output

### Building from Source

 #### **Using Make**
 ```bash
     git clone https://github.com/yourusername/pixelans.git
     cd pixelans
     make
```

 #### **Using GCC**
 ```bash
        mkdir -p build
        gcc -Wall -Wextra -Iinc -std=c11 -c src/main.c -o build/main.o
        gcc -Wall -Wextra -Iinc -std=c11 -c src/img.c -o build/img.o
        gcc build/main.o build.o -o pixelans -lm
 ```


## Usage

```
./pixelans <image_file> [options]
```

### Options

```
Options:
  -w, --width WIDTH     Width of ASCII output (default: 50)
  -p, --padding SPACES  Left padding spaces (default: 8)
  -c, --charset TYPE    Character set: dense (d), medium (m), sparse (s) (default: d)
  -i, --invert          Invert the ASCII mapping
  -n, --no-color        Disable color output
  -h, --help            Display this help message
```

### Examples

```bash
# Basic usage with default settings
./pixelans image.jpg

# Use a width of 100 characters and medium density charset
./pixelans image.png -w 100 -c m

# Create inverted ASCII art with 20 spaces of left padding
./pixelans landscape.jpg --invert --padding 20

# Generate black and white ASCII art (no color)
./pixelans portrait.png --no-color
```

## Character Sets

- **Dense** (`-c d`): `$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,"^'. `
- **Medium** (`-c m`): `@%#*+=-:. `
- **Sparse** (`-c s`): `@. `

## Project Structure

```
pixelans/
├── src/
│   ├── main.c                # Command-line interface and option parsing
│   └── img.c                 # Image processing and ASCII conversion
├── inc/
│   ├── img.h                 # Function declarations and constants
│   ├── stb_image.h           # External library for image loading
│   └── stb_image_resize.h    # External library for image resizing
├── build/                    # Generated during compilation
├── .clang-format             # Generated during compilation
└── Makefile
```
## Acknowledgments

- [stb](https://github.com/nothings/stb)
