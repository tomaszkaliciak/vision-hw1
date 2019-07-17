#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    return get_pixel(im, roundf(x), roundf(y), c);
}

image nn_resize(image im, int w, int h)
{
    image new_image = make_image(w, h, im.c);

    float width_ratio = ((float)im.w) / w;
    float height_ratio = ((float)im.h) / h;

    for (int y = 0; y < new_image.h; ++y)
    {
        for (int x = 0; x < new_image.w; ++x)
        {
            for (int z = 0; z < new_image.c; ++z)
            {
                float scaled_x = x * width_ratio + (width_ratio * 0.5 - 0.5);
                float scaled_y = y * height_ratio + (height_ratio * 0.5 - 0.5);
                float value = nn_interpolate(im, scaled_x, scaled_y, z);
                set_pixel(new_image, x, y, z, value);
            }
        }
    }

    return new_image;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    float d1 = x - ceilf(x);
    float d2 = floorf(x) - x;
    float d3 = y - ceil(y);
    float d4 = floorf(y) - y;

    float v1 = get_pixel(im, ceilf(x), ceilf(y), c);
    float v2 = get_pixel(im , floorf(x), ceilf(y), c);
    float v3 = get_pixel(im, ceilf(x), floorf(y), c);
    float v4 = get_pixel(im , floorf(x), floorf(y), c);

    float q1 = v1*d2 + v2*d1;
    float q2 = v3*d2 + v4*d1;
    return q1*d4 + q2*d3;
}

image bilinear_resize(image im, int w, int h)
{
    image new_image = make_image(w, h, im.c);

    float width_ratio = ((float)im.w) / w;
    float height_ratio = ((float)im.h) / h;

    for (int y = 0; y < new_image.h; ++y)
    {
        for (int x = 0; x < new_image.w; ++x)
        {
            for (int z = 0; z < new_image.c; ++z)
            {
                float scaled_x = x * width_ratio + (width_ratio * 0.5 - 0.5);
                float scaled_y = y * height_ratio + (height_ratio * 0.5 - 0.5);
                float value = bilinear_interpolate(im, scaled_x, scaled_y, z);
                set_pixel(new_image, x, y, z, value);
            }
        }
    }

    return new_image;
}

