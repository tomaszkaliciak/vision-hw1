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
    return 0;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    return make_image(1,1,1);
}

