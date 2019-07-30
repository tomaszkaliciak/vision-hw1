#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"
#include <stdlib.h>

float get_pixel(image im, int x, int y, int c)
{
    int width = x >= 0 ? (x < im.w ? x : im.w -1) : 0;
    int height = y >= 0 ? (y < im.h ? y : im.h -1) : 0;
    return im.data[im.w * height + width + im.w * im.h *c];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    if (x >= 0 && x < im.w && y>=0 && y< im.w && c >=0 && c < im.c)
    {
        im.data[im.w * y + x + im.w * im.h * c] = v;
    }
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    memcpy(copy.data, im.data, sizeof(float) * im.w * im.h * im.c);
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);

    int currIndex = 0;

    for(int i = 0; i < im.h; ++i)
    {
        for(int j = 0; j < im.w; ++j)
        {
            currIndex = im.w * i + j;
            gray.data[currIndex] = 
                0.299 * im.data[currIndex] +
                0.587 * im.data[currIndex + im.h * im.w * 1]  +
                0.114 * im.data[currIndex + im.h * im.w * 2]; 
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    for(int i = 0; i < im.h; ++i)
    {
        for(int j = 0; j < im.w; ++j)
        {
            im.data[im.w * i + j + im.h * im.w * c] += v;
        }
    }
}

void scale_image(image im, int c, float v)
{
    for(int i = 0; i < im.h; ++i)
    {
        for(int j = 0; j < im.w; ++j)
        {
            im.data[im.w * i + j + im.h * im.w * c] *= v;
        }
    }
}

void clamp_image(image im)
{
    int max_index = im.h*im.w*im.c;
    for(int i = 0; i < max_index ; ++i)
    {
        if (im.data[i] > 1)
        {
            im.data[i] = 1;
        }
        else if (im.data[i] < 0)
        {
            im.data[i] = 0;
        }
    }
}

float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    assert(im.c == 3);

    int currIndex = 0;
    float v = 0;
    float m = 0;
    float c = 0;
    float s = 0;
    float h_prim = 0;
    float h = 0;

    for(int i = 0; i < im.h; ++i)
    {
        for(int j = 0; j < im.w; ++j)
        {
            currIndex = im.w * i + j;

            v = three_way_max(
                im.data[currIndex],
                im.data[currIndex + im.h * im.w],
                im.data[currIndex + im.h * im.w * 2]
            );

            m = three_way_min(
                im.data[currIndex],
                im.data[currIndex + im.h * im.w],
                im.data[currIndex + im.h * im.w * 2]
            );

            if (v == m)
            {
                s = 0;
                h = 0;
            }
            else 
            {
                c = v - m;
                s = c/v;

                if (v == im.data[currIndex])
                {
                    h_prim = im.data[currIndex + im.h * im.w] -
                        im.data[currIndex + im.h * im.w * 2];
                    h_prim /= c;
                }
                else if (v == im.data[currIndex + im.h * im.w])
                {
                    h_prim = im.data[currIndex + im.h * im.w * 2] -
                        im.data[currIndex];
                    h_prim /= c;
                    h_prim += 2;
                }
                else
                {
                    h_prim = im.data[currIndex] - im.data[currIndex + im.h * im.w];
                    h_prim /= c;
                    h_prim += 4;
                }
                
                if (h_prim < 0)
                {
                    h = (h_prim / 6) + 1;
                }
                else
                {
                    h = h_prim / 6;
                }
            }
                set_pixel(im, j, i, 0, h);
                set_pixel(im, j, i, 1, s);
                set_pixel(im, j, i, 2, v);
        }
    }
}

void hsv_to_rgb(image im)
{
    int currIndex = 0;

    float h_prim = 0;
    float x = 0;
    float c = 0;
    float m = 0;

    float h = 0;
    float s = 0;
    float v = 0;

    float r = 0;
    float g = 0;
    float b = 0;

    for(int i = 0; i < im.h; ++i)
    {
        for(int j = 0; j < im.w; ++j)
        {
            currIndex = im.w * i + j;
            h = im.data[currIndex];
            s = im.data[currIndex + im.w * im.h];
            v = im.data[currIndex + im.w * im.h * 2];

            c = v * s;
            h_prim = h * 6;
            x = c * (1 - fabs(fmod(h_prim, 2) -1));

            if (h_prim >= 0 && h_prim < 1)
            {
                r = c;
                g = x;
                b = 0;
            }
            else if (h_prim >= 1 && h_prim < 2)
            {
                r = x;
                g = c;
                b = 0;
            }
            else if (h_prim >= 2 && h_prim < 3)
            {
                r = 0;
                g = c;
                b = x;
            }
            else if (h_prim >= 3 && h_prim < 4)
            {
                r = 0;
                g = x;
                b = c;
            }
            else if (h_prim >= 4 && h_prim < 5)
            {
                r = x;
                g = 0;
                b = c;
            }
            else if (h_prim >= 5 && h_prim < 6)
            {
                r = c;
                g = 0;
                b = x;
            }

            m = v - c;
            r += m;
            g += m;
            b += m;

            set_pixel(im, j, i, 0, r);
            set_pixel(im, j, i, 1, g);
            set_pixel(im, j, i, 2, b);
        }
    }
}