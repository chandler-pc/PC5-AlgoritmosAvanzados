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

    void clear(const sf::Color& color) {
        for (unsigned int y = 0; y < height; ++y) {
            for (unsigned int x = 0; x < width; ++x) {
                set_pixel(x, y, color);
            }
        }
    }

	void set_pixel(const unsigned int x, const unsigned int y, const sf::Color& color) {
        if (x < 0 || x >= width ||
            y < 0 || y >= height) return;

        const unsigned int index = (y * width + x) * 4;
        pixels[index + 0] = color.r;
        pixels[index + 1] = color.g;
        pixels[index + 2] = color.b;
        pixels[index + 3] = color.a;
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
