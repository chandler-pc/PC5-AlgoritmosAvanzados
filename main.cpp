#include <SFML/Graphics.hpp>

#include "CameraController.h"
#include "Framebuffer.h"
#include "Mesh.h"
#include "Matrix.h"


int main()
{
	constexpr unsigned int width = 800;
	constexpr unsigned int height = 800;
    sf::RenderWindow window(sf::VideoMode({ width, height}), "Pipeline");
    Framebuffer fb(width, height);
    Mesh mesh;
    if (!mesh.load_from_obj("teapot.obj")) {
        return -1;
    }
	CameraController camera;
    Matrix4 proj = Matrix4::perspective(90.0f * 3.1415f / 180.0f, width * 1.f / height, 0.1f, 100.0f);
    while (window.isOpen())
    {
	    while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        camera.handle_input();

        Matrix4 view = camera.getViewMatrix();
        Matrix4 model = Matrix4::identity();
        Matrix4 mvp = proj * view * model;

        fb.clear(sf::Color::Black);

        for (const auto& vertex : mesh.vertices) {
            Vector4 v(vertex.x, vertex.y, vertex.z, 1.0f);
            Vector4 projected = mvp * v;

            if (projected.w != 0.0f) {
                projected.x /= projected.w;
                projected.y /= projected.w;
                projected.z /= projected.w;
            }

            if (projected.x < -1 || projected.x > 1 ||
                projected.y < -1 || projected.y > 1 ||
                projected.z < -1 || projected.z > 1) {
                continue;
            }
            int screen_x = static_cast<int>((projected.x + 1.0f) * 0.5f * width);
            int screen_y = static_cast<int>((1.0f - projected.y) * 0.5f * height);
            fb.set_pixel(screen_x, screen_y, sf::Color::White);
        }
        fb.display(window);
        window.display();
    }
}
