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

    int centre = size/2;

    for (int x = 0; x < size; ++x)
    {
        for (int y = 0; y < size; ++y)
        {
            float exponent = - (pow(x-centre, 2) + pow(y- centre, 2)) / (2 * pow(sigma, 2));
            float value = multiplier * exp(exponent);
            set_pixel(filter, x, y, 0, value);
        }
    }

    l1_normalize(filter);
    return filter;
}

image add_image(image a, image b)
{
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image output_image = make_image(a.w, a.h, a.c);

    int max_index = a.w * a.h * a.c;

    for (int i = 0; i < max_index; ++i)
    {
        output_image.data[i] = a.data[i] + b.data[i];
    }

    return output_image;
}

image sub_image(image a, image b)
{
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image output_image = make_image(a.w, a.h, a.c);

    int max_index = a.w * a.h * a.c;

    for (int i = 0; i < max_index; ++i)
    {
        output_image.data[i] = a.data[i] - b.data[i];
    }

    return output_image;
}

image make_gx_filter()
{
    image filter = make_image(3 , 3 , 1);

    set_pixel(filter, 0, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 1);
    set_pixel(filter, 0, 1, 0, -2);
    set_pixel(filter, 2, 1, 0, 2);
    set_pixel(filter, 0, 2, 0, -1);
    set_pixel(filter, 2, 2, 0, 1);

    return filter;
}

image make_gy_filter()
{
    image filter = make_image(3 , 3 , 1);

    set_pixel(filter, 0, 0,0, -1);
    set_pixel(filter, 1, 0,0, -2);
    set_pixel(filter, 2, 0,0, -1);
    set_pixel(filter, 0, 2,0, 1);
    set_pixel(filter, 1, 2,0, 2);
    set_pixel(filter, 2, 2,0, 1);

    return filter;
}

void feature_normalize(image im)
{
    int max_index = im.w * im.h * im.c;
    float max = 0;
    float min = 1;

    for (int i = 0; i < max_index; ++i)
    {
        if (im.data[i] > max) max = im.data[i];
        if (im.data[i] < min) min = im.data[i];
    }

    float spread = max - min;

    if (spread == 0)
    {
        for (int i = 0; i < max_index; ++i)
        {
            im.data[i] = 0;
        }
    }
    else
    {
        for (int i = 0; i < max_index; ++i)
        {
            float new_value = (im.data[i]-min) / (max - min);
            im.data[i] = new_value;
        }
    }
}

image *sobel_image(image im)
{
    image* images = calloc(2, sizeof(image));

    images[0] = make_image(im.w, im.h, 1);
    images[1] = make_image(im.w, im.h, 1);

    image dx = make_gx_filter();
    image dy = make_gy_filter();

    image gx = convolve_image(im, dx, 0);
    image gy = convolve_image(im, dy, 0);

    free_image(dx);
    free_image(dy);

    for (int i = 0; i < im.w * im.h; i++)
    {
        images[0].data[i] = sqrtf(pow(gx.data[i], 2) + pow(gy.data[i], 2));
        images[1].data[i] = atan2(gy.data[i], gx.data[i]);
    }

    free_image(gx);
    free_image(gy);

    return images;
}

image colorize_sobel(image im)
{
    image *sobel = sobel_image(im);
    clamp_image(sobel[0]);

    image preserve = make_image(im.w, im.h, im.c);
    image invert = make_image(im.w, im.h, im.c);

    feature_normalize(sobel[0]);
    for (int xy = 0; xy < im.w*im.h; xy++) {
        for (int i = 0; i < im.c; i++) {
            invert.data[xy + i * im.w * im.h] = 1 - im.data[i*im.w*im.h + xy];
            preserve.data[xy + i * im.w * im.h] = sobel[0].data[xy];
        }
    }
    clamp_image(preserve);
    image sub = sub_image(preserve, invert);
    clamp_image(sub);
    return sub;
}
