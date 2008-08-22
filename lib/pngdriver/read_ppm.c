
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <grass/gis.h>
#include "pngdriver.h"

void read_ppm(void)
{
    FILE *input;
    int x, y;
    int i_width, i_height, maxval;
    unsigned int rgb_mask = get_color(255, 255, 255, 0);
    unsigned int *p;

    if (!png.true_color)
	G_fatal_error("PNG: cannot use PPM/PGM with indexed color");

    input = fopen(png.file_name, "rb");
    if (!input)
	G_fatal_error("PNG: couldn't open input file %s", png.file_name);

    if (fscanf(input, "P6 %d %d %d", &i_width, &i_height, &maxval) != 3)
	G_fatal_error("PNG: invalid input file %s", png.file_name);

    fgetc(input);

    if (i_width != png.width || i_height != png.height)
	G_fatal_error
	    ("PNG: input file has incorrect dimensions: expected: %dx%d got: %dx%d",
	     png.width, png.height, i_width, i_height);

    for (y = 0, p = png.grid; y < png.height; y++) {
	for (x = 0; x < png.width; x++, p++) {
	    unsigned int c = *p;

	    int r = fgetc(input);
	    int g = fgetc(input);
	    int b = fgetc(input);

	    r = r * 255 / maxval;
	    g = g * 255 / maxval;
	    b = b * 255 / maxval;

	    c &= ~rgb_mask;
	    c |= get_color(r, g, b, 0);

	    *p = c;
	}
    }

    fclose(input);
}

void read_pgm(void)
{
    char *mask_name = G_store(png.file_name);
    FILE *input;
    int x, y;
    int i_width, i_height, maxval;
    unsigned int rgb_mask = get_color(255, 255, 255, 0);
    unsigned int *p;

    if (!png.true_color)
	G_fatal_error("PNG: cannot use PPM/PGM with indexed color");

    mask_name[strlen(mask_name) - 2] = 'g';

    input = fopen(mask_name, "rb");
    if (!input)
	G_fatal_error("PNG: couldn't open input mask file %s", mask_name);

    if (fscanf(input, "P5 %d %d %d", &i_width, &i_height, &maxval) != 3)
	G_fatal_error("PNG: invalid input mask file %s", mask_name);

    fgetc(input);

    if (i_width != png.width || i_height != png.height)
	G_fatal_error
	    ("PNG: input mask file has incorrect dimensions: expected: %dx%d got: %dx%d",
	     png.width, png.height, i_width, i_height);

    G_free(mask_name);

    for (y = 0, p = png.grid; y < png.height; y++) {
	for (x = 0; x < png.width; x++, p++) {
	    unsigned int c = *p;

	    int k = fgetc(input);

	    k = k * 255 / maxval;

	    c &= rgb_mask;
	    c |= get_color(0, 0, 0, 255 - k);

	    *p = c;
	}
    }

    fclose(input);
}
