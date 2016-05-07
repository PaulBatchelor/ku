#include <stdio.h>
#include "s7.h"
#include <dlfcn.h>
#include "lodepng.h"

#define WIDTH 256
#define HEIGHT 128

static s7_pointer cload(s7_scheme *sc, s7_pointer args);

static void *library = NULL;

typedef struct ku_color {
    unsigned char r, g, b, a;
} ku_color;

typedef struct ku_image {
    unsigned char img[WIDTH * HEIGHT * 4];
    unsigned int width, height;
    ku_color colors[16];
} ku_image;

static ku_image ku;

ku_color rgba(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    ku_color clr;
    clr.r = r;
    clr.g = g;
    clr.b = b;
    clr.a = a;
    return clr;
}

static void set_rgba(ku_image *ku, int index, ku_color color)
{
    ku->colors[index] = color;
}

static void set_color(ku_image *ku, int x, int y, int color)
{
    ku_color *clr = &ku->colors[color];
    unsigned char *img = ku->img;
    if(x > ku->width || y > ku->height) {
        fprintf(stderr, "Warning: (%d, %d) out of bounds (%d, %d)\n", 
                x, y, ku->width, ku->height);
    } 
    img[y * ku->width * 4 + x * 4] = clr->r;
    img[y * ku->width * 4 + x * 4 + 1] = clr->g;
    img[y * ku->width * 4 + x * 4 + 2] = clr->b;
    img[y * ku->width * 4 + x * 4 + 3] = clr->a;
}

static void set_pico8_pallete(ku_image *ku)
{
    ku_color pico [] = {
        {0, 0, 0, 255},
        {29, 43, 83, 255},
        {126, 37, 83, 255},
        {0, 135, 81, 255},
        {171, 82, 54, 255},
        {95, 87, 79, 255},
        {194, 195, 199, 255},
        {255, 241, 232, 255},
        {255, 0, 77, 255},
        {255, 163, 0, 255},
        {255, 255, 39, 255},
        {0, 231, 86, 255},
        {41, 173, 255, 255},
        {131, 118, 156, 255},
        {255, 204, 170, 255},
        {255, 255, 255, 255},
    };
    int c;
    for(c = 0; c < 16; c++) {
        set_rgba(ku, c, pico[c]);
    }
}

static void write_png(ku_image *ku, const char *filename)
{
    unsigned error = lodepng_encode32_file(filename, 
            ku->img, ku->width, ku->height);
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
}

static void ku_init(ku_image *my_ku)
{
    int x, y; 
    set_pico8_pallete(my_ku);

    my_ku->width = WIDTH;
    my_ku->height = HEIGHT;
    for(y = 0; y < HEIGHT; y++) {
        for(x = 0; x < WIDTH; x++) {
            set_color(my_ku, x, y, 0);
        }
    }
    my_ku->width = 128;
    my_ku->height = 128;
}

static s7_pointer f_set_color(s7_scheme *sc, s7_pointer args)
{
    int x = s7_integer(s7_list_ref(sc, args, 0));
    int y = s7_integer(s7_list_ref(sc, args, 1));
    int color = s7_integer(s7_list_ref(sc, args, 2));
    set_color(&ku, x, y, color);
    return s7_nil(sc);
}

static s7_pointer f_write_png(s7_scheme *sc, s7_pointer args)
{
    const char *str = s7_string(s7_list_ref(sc, args, 0));
    write_png(&ku, str);
    return NULL;
}

void init_ku(s7_scheme *sc)
{
    ku_init(&ku);
    s7_define_function(sc, "set-color", f_set_color, 3, 0, false, NULL);
    s7_define_function(sc, "write-png", f_write_png, 1, 0, false, NULL);
    s7_define_variable(sc, "width", s7_make_integer(sc, ku.width));
    s7_define_variable(sc, "height", s7_make_integer(sc, ku.height));
}

