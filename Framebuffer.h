#pragma once

#include <SFML/Graphics.hpp>
#include <cstring>

class Framebuffer {
public:
    Framebuffer(unsigned int width, unsigned int height)
        : width(width), height(height), texture(sf::Vector2u(width, height)), sprite(texture) {
        const size_t pixelCount = static_cast<size_t>(width) * static_cast<size_t>(height) * 4;
        pixels = new uint8_t[pixelCount];
        memset(pixels, 0, pixelCount);
    }

    ~Framebuffer() {
        delete[] pixels;
    }

    void clear(const sf::Color& color) const {
        for (unsigned int y = 0; y < height; ++y) {
            for (unsigned int x = 0; x < width; ++x) {
                set_pixel(x, y, color);
            }
        }
    }

	void set_pixel(const unsigned int x, const unsigned int y, const sf::Color& color = sf::Color::White) const
	{
        if (x < 0 || x >= width ||
            y < 0 || y >= height) return;

        const unsigned int index = (y * width + x) * 4;
        pixels[index + 0] = color.r;
        pixels[index + 1] = color.g;
        pixels[index + 2] = color.b;
        pixels[index + 3] = color.a;
    }

    void draw_line(int x0, int y0, int x1, int y1, bool aa = false, const sf::Color& color = sf::Color::White) const
    {
        if (!aa)
        {
            //Bresenham's line algorithm
            const int dx = abs(x1 - x0);
            const int dy = -abs(y1 - y0);
            const int sx = x0 < x1 ? 1 : -1;
            const int sy = y0 < y1 ? 1 : -1;
            int err = dx + dy;
            while (true)
            {
                set_pixel(x0, y0, color);
                if (x0 == x1 && y0 == y1) break;
                if (const int err2 = 2 * err >= dy) {
                    if (x0 == x1) break;
                    x0 += sx;
                    err += dy;
                }
                else if (err2 <= dx) {
                    if (y0 == y1) break;
                    y0 += sy;
                    err += dx;
                }
            }
        }
    }

    void display(sf::RenderWindow* window) {
        texture.update(pixels);
        window->draw(sprite);
    }

    unsigned int get_width() const { return width; }
    unsigned int get_height() const { return height; }

private:
    unsigned int width, height;
    uint8_t* pixels;
    sf::Texture texture;
    sf::Sprite sprite;
};
