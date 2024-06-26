#include "easyppm.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

static void easyppm_abort(PPM *ppm, const char *fmt, ...);
static int easyppm_is_grey(ppmcolor c);
static int easyppm_is_black_white(ppmcolor c);

/*
 * This file is part of the INFO0902 course given by Pr. Geurts 
 * Creates new PPM from args, aborts if dimensions are invalid
 */
PPM easyppm_create(int width, int height, imagetype itype)
{
    PPM ppm;

    if (width <= 0 || height <= 0)
        easyppm_abort(NULL, "Invalid image dimensions (%d, %d)\n", width, height);

    ppm.width = width;
    ppm.height = height;
    if (itype == IMAGETYPE_PBM || itype == IMAGETYPE_PGM)
    {
        ppm.image = (PPMBYTE *)malloc(sizeof(*ppm.image) * width * height);
    }
    else
    {
        ppm.image = (PPMBYTE *)malloc(sizeof(*ppm.image) * width * height * EASYPPM_NUM_CHANNELS);
    }
    ppm.itype = itype;

    return ppm;
}

/*
 * Fill entire color buffer with specified color
 */
void easyppm_clear(PPM *ppm, ppmcolor c)
{
    int x, y;

    if (!ppm)
        easyppm_abort(ppm, "Passed NULL PPM to easyppm_clear()\n");

    if (ppm->itype == IMAGETYPE_PBM && !easyppm_is_black_white(c))
        easyppm_abort(ppm, "Passed invalid color to easyppm_clear() for PBM image\n");
    if (ppm->itype == IMAGETYPE_PGM && !easyppm_is_grey(c))
        easyppm_abort(ppm, "Passed invalid color to easyppm_clear() for PGM image\n");

    for (y = 0; y < ppm->height; y++)
        for (x = 0; x < ppm->width; x++)
            easyppm_set(ppm, x, y, c);
}

/*
 * Set appropriate color (RGB or greyscale)
 */
void easyppm_set(PPM *ppm, int x, int y, ppmcolor c)
{
    int i;

    if (!ppm)
        easyppm_abort(ppm, "Passed NULL PPM to easyppm_set()\n");
    if (x < 0 || x >= ppm->width || y < 0 || y >= ppm->height)
        easyppm_abort(ppm, "Index out of bounds in easyppm_set (%d, %d)\n", x, y);

    i = x + y * ppm->width;

    if (ppm->itype == IMAGETYPE_PBM || ppm->itype == IMAGETYPE_PGM)
    {
        ppm->image[i] = c.r;
    }
    else
    {
        ppm->image[EASYPPM_NUM_CHANNELS * i + 0] = c.r;
        ppm->image[EASYPPM_NUM_CHANNELS * i + 1] = c.g;
        ppm->image[EASYPPM_NUM_CHANNELS * i + 2] = c.b;
    }
}

/*
 * Get appropriate color (RGB or greyscale)
 */
ppmcolor easyppm_get(PPM *ppm, int x, int y)
{
    ppmcolor c;
    int i;

    if (!ppm)
        easyppm_abort(ppm, "Passed NULL PPM to easyppm_get()\n");
    if (x < 0 || x >= ppm->width || y < 0 || y > ppm->height)
        easyppm_abort(ppm, "Index out of bounds in easyppm_get (%d, %d)\n", x, y);

    i = x + y * ppm->width;

    if (ppm->itype == IMAGETYPE_PBM || ppm->itype == IMAGETYPE_PGM)
    {
        c.r = ppm->image[i];
        c.g = ppm->image[i];
        c.b = ppm->image[i];
    }
    else
    {
        c.r = ppm->image[EASYPPM_NUM_CHANNELS * i + 0];
        c.g = ppm->image[EASYPPM_NUM_CHANNELS * i + 1];
        c.b = ppm->image[EASYPPM_NUM_CHANNELS * i + 2];
    }
    return c;
}

/*
 * Create color from 8-bit RGB values
 */
ppmcolor easyppm_rgb(PPMBYTE r, PPMBYTE g, PPMBYTE b)
{
    ppmcolor c;

    c.r = r;
    c.g = g;
    c.b = b;

    return c;
}

/*
 * Create color from 8-bit greyscale value
 */
ppmcolor easyppm_grey(PPMBYTE gr)
{
    ppmcolor c;

    c.r = gr;
    c.g = gr;
    c.b = gr;

    return c;
}

/*
 * Create color from binary value: 0 for white
 * pixel, 1 for black pixel
 */
ppmcolor easyppm_black_white(int bw)
{
    ppmcolor c;

    bw = (bw == 0 ? 255 : 0);

    c.r = bw;
    c.g = bw;
    c.b = bw;

    return c;
}

/*
 * Gamma-correct entire image by specified amount
 */
void easyppm_gamma_correct(PPM *ppm, float gamma)
{
    int x, y;
    float r, g, b;
    float exp = 1 / gamma;
    ppmcolor c;

    if (!ppm)
        easyppm_abort(ppm, "Passed NULL PPM to easyppm_gamma_correct()\n");

    for (y = 0; y < ppm->height; y++)
    {
        for (x = 0; x < ppm->width; x++)
        {
            c = easyppm_get(ppm, x, y);

            r = (PPMBYTE)(powf(c.r / (float)EASYPPM_MAX_CHANNEL_VALUE, exp) * 255);
            g = (PPMBYTE)(powf(c.g / (float)EASYPPM_MAX_CHANNEL_VALUE, exp) * 255);
            b = (PPMBYTE)(powf(c.b / (float)EASYPPM_MAX_CHANNEL_VALUE, exp) * 255);

            easyppm_set(ppm, x, y, easyppm_rgb(r, g, b));
        }
    }
}

/*
 * Invert image y-axis for applications that assume an origin
 * in the lower left corner (easyppm defaults to an origin in
 * the upper left corner)
 */
void easyppm_invert_y(PPM *ppm)
{
    int x, yt, yb;

    if (!ppm)
        easyppm_abort(ppm, "Passed NULL PPM to easyppm_invert_y()\n");

    for (yt = 0, yb = ppm->height - 1; yt <= yb; yt++, yb--)
    {
        for (x = 0; x < ppm->width; x++)
        {
            ppmcolor tmp = easyppm_get(ppm, x, yb);
            easyppm_set(ppm, x, yb, easyppm_get(ppm, x, yt));
            easyppm_set(ppm, x, yt, tmp);
        }
    }
}

/*
 * Read image from file. Aborts if file could not be opened
 * or the dimensions are invalid.
 */
void easyppm_read(PPM *ppm, const char *path)
{
    FILE *fp;
    char itypestr[3];
    int width, height, dummy;
    int x, y;
    int gr;
    int r, g, b;
    ppmcolor c;

    if (!ppm)
        easyppm_abort(ppm, "Passed NULL PPM to easyppm_read()\n");

    easyppm_destroy(ppm);

    fp = fopen(path, "r");
    if (!fp)
        easyppm_abort(NULL, "Could not open file %s for reading\n", path);

    dummy = fscanf(fp, "%2s\n", itypestr);
    if (strcmp(itypestr, "P1") == 0)
    {
        ppm->itype = IMAGETYPE_PBM;
        dummy = fscanf(fp, "%d %d\n", &width, &height);
    }
    else if (strcmp(itypestr, "P2") == 0)
    {
        ppm->itype = IMAGETYPE_PGM;
        dummy = fscanf(fp, "%d %d %d\n", &width, &height, &dummy);
    }
    else
    {
        ppm->itype = IMAGETYPE_PPM;
        dummy = fscanf(fp, "%d %d %d\n", &width, &height, &dummy);
    }

    ppm->width = width;
    ppm->height = height;
    if (width <= 0 || height <= 0)
    {
        fclose(fp);
        easyppm_abort(NULL, "Passed invalid dimensions to easyppm_read() (%d, %d)\n", width, height);
    }

    if (ppm->itype == IMAGETYPE_PBM || ppm->itype == IMAGETYPE_PGM)
    {
        ppm->image = (PPMBYTE *)malloc(sizeof(*ppm->image) * width * height);
    }
    else
    {
        ppm->image = (PPMBYTE *)malloc(sizeof(*ppm->image) * width * height * EASYPPM_NUM_CHANNELS);
    }

    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            if (ppm->itype == IMAGETYPE_PBM)
            {
                dummy = fscanf(fp, "%d\n", &gr);
                gr = (gr == 0 ? 1 : 0);
                c.r = gr;
                c.g = gr;
                c.b = gr;
            }
            else if (ppm->itype == IMAGETYPE_PGM)
            {
                dummy = fscanf(fp, "%d\n", &gr);
                c.r = gr;
                c.g = gr;
                c.b = gr;
            }
            else
            {
                dummy = fscanf(fp, "%d %d %d\n", &r, &g, &b);
                c.r = r;
                c.g = g;
                c.b = b;
            }
            easyppm_set(ppm, x, y, c);
        }
    }

    fclose(fp);
}

/*
 * Write image to file. Aborts if file could not be opened or
 * the file extension on the path doesn't match the image type
 * (.pbm for PBM files, .pgm for PGM files, and .ppm for PPM
 * files).
 */
void easyppm_write(PPM *ppm, const char *path)
{
    FILE *fp;
    int x, y;

    if (!ppm)
        easyppm_abort(ppm, "Passed NULL PPM to easyppm_write()\n");

    fp = fopen(path, "w");
    if (!fp)
        easyppm_abort(ppm, "Could not open file %s for writing\n", path);

    if (ppm->itype == IMAGETYPE_PBM)
    {
        fprintf(fp, "P1\n");
        fprintf(fp, "%d %d\n", ppm->width, ppm->height);
    }
    else if (ppm->itype == IMAGETYPE_PGM)
    {
        fprintf(fp, "P2\n");
        fprintf(fp, "%d %d %d\n", ppm->width, ppm->height, EASYPPM_MAX_CHANNEL_VALUE);
    }
    else
    {
        fprintf(fp, "P3\n");
        fprintf(fp, "%d %d %d\n", ppm->width, ppm->height, EASYPPM_MAX_CHANNEL_VALUE);
    }

    for (y = 0; y < ppm->height; y++)
    {
        for (x = 0; x < ppm->width; x++)
        {
            ppmcolor c = easyppm_get(ppm, x, y);
            if (ppm->itype == IMAGETYPE_PBM)
            {
                fprintf(fp, "%d\n", c.r == 0 ? 1 : 0);
            }
            else if (ppm->itype == IMAGETYPE_PGM)
            {
                fprintf(fp, "%d\n", c.r);
            }
            else
            {
                fprintf(fp, "%d %d %d\n", c.r, c.g, c.b);
            }
        }
    }

    fclose(fp);
}

/*
 * Free image buffer
 */
void easyppm_destroy(PPM *ppm)
{
    if (ppm && ppm->image)
        free(ppm->image);
}

/*
 * Cleanup resources and abort program
 */
static void easyppm_abort(PPM *ppm, const char *fmt, ...)
{
    va_list argptr;
    va_start(argptr, fmt);
    vfprintf(stderr, fmt, argptr);
    va_end(argptr);
    fprintf(stderr, "Aborting\n");

    easyppm_destroy(ppm);

    exit(EXIT_FAILURE);
}

/*
 * Determine if color is greyscale
 */
static int easyppm_is_grey(ppmcolor c)
{
    return c.r == c.g && c.g == c.b;
}

/*
 * Determine if color is black or white
 */
static int easyppm_is_black_white(ppmcolor c)
{
    return easyppm_is_grey(c) && (c.r == 255 || c.r == 0);
}

/*
 * draw a line using Brehensam's algorithm
 */

void easyppm_draw_line(PPM *ppm, int x0, int y0, int x1, int y1, ppmcolor c)
{

    if (ppm == NULL)
    {
        easyppm_abort(ppm, "Passed NULL PPM to easyppm_draw_line()\n");
    }

    if (x0 < 0 || x0 > ppm->width)
        easyppm_abort(ppm, "ERROR : invalid x1 value in easyppm_draw_line()");
    if (x1 < 0 || x1 > ppm->width)
        easyppm_abort(ppm, "ERROR : invalid x2 value in easyppm_draw_line()");
    if (y0 < 0 || y0 > ppm->height)
        easyppm_abort(ppm, "ERROR : invalid y1 value in easyppm_draw_line()");
    if (y1 < 0 || y1 > ppm->height)
        easyppm_abort(ppm, "ERROR : invalid y2 value in easyppm_draw_line()");

    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2, e2;

    for (;;)
    {
        easyppm_set(ppm, x0, y0, c);
        if (x0 == x1 && y0 == y1)
            break;
        e2 = err;
        if (e2 > -dx)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dy)
        {
            err += dx;
            y0 += sy;
        }
    }
}

/*
 * draw a polygon from a list of points. The first and last points are connected.
 */

void easyppm_draw_polygon(PPM *ppm, int nbpoints, int *xcoordinates, int *ycoordinates, ppmcolor c)
{
    if (ppm == NULL)
    {
        easyppm_abort(ppm, "Passed NULL PPM to easyppm_draw_polygon()\n");
    }

    if (nbpoints <= 1)
        return;

    for (int i = 0; i < nbpoints - 1; i++)
    {
        easyppm_draw_line(ppm, xcoordinates[i], ycoordinates[i], xcoordinates[i + 1], ycoordinates[i + 1], c);
    }
    easyppm_draw_line(ppm, xcoordinates[nbpoints - 1], ycoordinates[nbpoints - 1], xcoordinates[0], ycoordinates[0], c);
}

/*
 * draw a filled polygon from a list of points. The first and last points are connected.
 * (Algorithm described here: http://alienryderflex.com/polygon_fill/)
 */

void easyppm_fill_polygon(PPM *ppm, int nbpoints, int *xcoordinates, int *ycoordinates, ppmcolor cin, ppmcolor cborder)
{

    int *polyX = xcoordinates;
    int *polyY = ycoordinates;
    int polyCorners = nbpoints;

    int nodes, nodeX[nbpoints], pixelX, pixelY, i, j, swap;

    //  Loop through the rows of the image.
    for (pixelY = 0; pixelY < ppm->height; pixelY++)
    {

        //  Build a list of nodes.
        nodes = 0;
        j = polyCorners - 1;
        for (i = 0; i < polyCorners; i++)
        {
            if (((polyY[i] < pixelY) && (polyY[j] >= pixelY)) || ((polyY[j] < pixelY) && (polyY[i] >= pixelY)))
            {
                nodeX[nodes++] = (int)ceilf((double)polyX[i] +
                                            (double)(pixelY - polyY[i]) / (double)(polyY[j] - polyY[i]) * (double)(polyX[j] - polyX[i]));
            }
            j = i;
        }

        //  Sort the nodes, via a simple “Bubble” sort.
        i = 0;
        while (i < nodes - 1)
        {
            if (nodeX[i] > nodeX[i + 1])
            {
                swap = nodeX[i];
                nodeX[i] = nodeX[i + 1];
                nodeX[i + 1] = swap;
                if (i)
                    i--;
            }
            else
            {
                i++;
            }
        }

        //  Fill the pixels between node pairs.
        for (i = 0; i < nodes; i += 2)
        {
            if (nodeX[i] >= ppm->width)
                break;
            if (nodeX[i + 1] > 0)
            {
                if (nodeX[i] < 0)
                    nodeX[i] = 0;
                if (nodeX[i + 1] > ppm->width)
                    nodeX[i + 1] = ppm->width;
                for (pixelX = nodeX[i]; pixelX < nodeX[i + 1]; pixelX++)
                    easyppm_set(ppm, pixelX, pixelY, cin);
            }
        }
    }

    // draw the borders
    easyppm_draw_polygon(ppm, nbpoints, xcoordinates, ycoordinates, cborder);
}
