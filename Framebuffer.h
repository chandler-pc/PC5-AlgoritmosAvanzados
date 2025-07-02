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

    void draw_line(int x1, int y1, int x2, int y2, bool aa = false, const sf::Color& color = sf::Color::White) const
    {
    	const int dx = abs(x2 - x1);
    	const int dy = -abs(y2 - y1);
    	const int sx = x1 < x2 ? 1 : -1;
    	const int sy = y1 < y2 ? 1 : -1;
        /* Bresenham's line algorithm
        int err = dx + dy;
        while (true)
        {
			set_pixel(x1, y1, color);
			if (x1 == x2 && y1 == y2) break;
			if (int err2 = 2 * err >= dy) {
				if (x1 == x2) break;
				x1 += sx;
				err += dy;
			}
			else if (err2 <= dx) {
				if (y1 == y2) break;
				y1 += sy;
				err += dx;
			}
        }
		*/
        //Bresenham's line algorithm (Optimized version)
        if (dx > dy) {
            int err = dx / 2;
            while (x1 != x2) {
                set_pixel(x1, y1);
                x1 += sx;
                err -= dy;
                if (err < 0) {
                    y1 += sy;
                    err += dx;
                }
            }
        }
        else {
            int err = dy / 2;
            while (y1 != y2) {
                set_pixel(x1, y1);
                y1 += sy;
                err -= dx;
                if (err < 0) {
                    x1 += sx;
                    err += dy;
                }
            }
        }
        set_pixel(x2, y2);
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
