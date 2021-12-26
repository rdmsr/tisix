#include <tisix/gfx/canvas.hpp>
#include <tisix/log.hpp>

namespace tisix::gfx
{

Canvas::Canvas(RawPixel *buf, int width, int height)
{
    this->buffer = buf;
    this->width = width;
    this->height = height;
}

void Canvas::draw_pixel(Color color, int x, int y)
{
    size_t raw_position = x + y * width;

    buffer[raw_position] = {
        .blue = color.blue,
        .green = color.green,
        .red = color.red,
        .__unused = 0};
}

} // namespace tisix::gfx
