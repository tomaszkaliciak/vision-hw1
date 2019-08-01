#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    float sum = 0;
    int maxIndex = im.w * im.h * im.c;

    for (int i = 0; i < maxIndex; ++i)
    {
        sum += im.data[i];
    }

    for (int i = 0; i < maxIndex; ++i)
    {
        im.data[i] /= sum;
    }
}

image make_box_filter(int w)
{
    image new_image = make_image(w, w, 1);

    for (int i = 0; i < new_image.h; ++i)
    {
        for(int j = 0; j < new_image.w; ++j)
        {
            set_pixel(new_image, j, i, 0, 1);
        }
    }

    l1_normalize(new_image);

    return new_image;
}

image convolve_image(image im, image filter, int preserve)
{
    assert(im.c == filter.c || filter.c == 1);

    image output_image;

    if (preserve)
    {
        output_image = make_image(im.w, im.h, im.c);
    }
    else
    {
        output_image = make_image(im.w, im.h, 1);
    }

    for (int row = 0; row < im.h; ++row)
    {
        for(int col = 0; col < im.w; ++col)
        {
            int shift = filter.w / 2;

            for (int channel = 0; channel < im.c; ++channel)
            {
                float sum = 0;
                for (int f_row = 0; f_row < filter.w; ++f_row)
                {
                    for (int f_col = 0; f_col < filter.w; ++f_col)
                    {
                        sum += get_pixel(filter, f_col, f_row, 0) *
                                get_pixel(im, col + f_col - shift, row + f_row - shift, channel);
                    }
                }

                if (preserve)
                {
                    set_pixel(output_image, col, row, channel, sum);
                }
                else
                {
                    float current_value = get_pixel(output_image, col, row, 0);
                    set_pixel(output_image, col, row, 0, current_value + sum);
                }
            }
        }
    }
    return output_image;
}

image make_highpass_filter()
{
    image filter = make_image(3 , 3 , 1);

    set_pixel(filter, 0, 0 , 0, 0);
    set_pixel(filter, 1, 0 , 0, -1);
    set_pixel(filter, 2, 0 , 0, 0);
    set_pixel(filter, 0, 1 , 0, -1);
    set_pixel(filter, 1, 1 , 0, 4);
    set_pixel(filter, 2, 1 , 0, -1);
    set_pixel(filter, 0, 2 , 0, 0);
    set_pixel(filter, 1, 2 , 0, -1);
    set_pixel(filter, 2, 2 , 0, 0);

    return filter;
}

image make_sharpen_filter()
{
    image filter = make_image(3 , 3 , 1);

    set_pixel(filter, 0, 0 , 0, 0);
    set_pixel(filter, 1, 0 , 0, -1);
    set_pixel(filter, 2, 0 , 0, 0);
    set_pixel(filter, 0, 1 , 0, -1);
    set_pixel(filter, 1, 1 , 0, 5);
    set_pixel(filter, 2, 1 , 0, -1);
    set_pixel(filter, 0, 2 , 0, 0);
    set_pixel(filter, 1, 2 , 0, -1);
    set_pixel(filter, 2, 2 , 0, 0);

    return filter;
}

image make_emboss_filter()
{
    image filter = make_image(3 , 3 , 1);

    set_pixel(filter, 0, 0 , 0, -2);
    set_pixel(filter, 1, 0 , 0, -1);
    set_pixel(filter, 2, 0 , 0, 0);
    set_pixel(filter, 0, 1 , 0, -1);
    set_pixel(filter, 1, 1 , 0, 1);
    set_pixel(filter, 2, 1 , 0, 1);
    set_pixel(filter, 0, 2 , 0, 0);
    set_pixel(filter, 1, 2 , 0, 1);
    set_pixel(filter, 2, 2 , 0, 2);

    return filter;
}

image make_gaussian_filter(float sigma)
{
    float multiplier = 1 / (TWOPI * pow(sigma, 2));
    int six_sigma = 6 * sigma;

    int size = (six_sigma % 2 ? six_sigma : six_sigma + 1);

    image filter = make_image(size, size, 1);

    for (int x = 0; x < size; ++x)
    {
        for (int y = 0; y < size; ++y)
        {
            float exponent = - (pow(x, 2) + pow(y, 2)) / (2 * pow(sigma, 2));
            float value = multiplier * exp(exponent);
            set_pixel(filter, x, y, 0, value);
        }
    }

    l1_normalize(filter);
    return filter;
}

image add_image(image a, image b)
{
    // TODO
    return make_image(1,1,1);
}

image sub_image(image a, image b)
{
    // TODO
    return make_image(1,1,1);
}

image make_gx_filter()
{
    // TODO
    return make_image(1,1,1);
}

image make_gy_filter()
{
    // TODO
    return make_image(1,1,1);
}

void feature_normalize(image im)
{
    // TODO
}

image *sobel_image(image im)
{
    // TODO
    return calloc(2, sizeof(image));
}

image colorize_sobel(image im)
{
    // TODO
    return make_image(1,1,1);
}
