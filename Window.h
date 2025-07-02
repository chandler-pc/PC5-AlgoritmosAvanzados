#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

#include "CameraController.h"
#include "Framebuffer.h"
#include "Matrix.h"
#include "Vector.h"
#include "Mesh.h"

class Window {
	unsigned int width_;
	unsigned int height_;
	sf::RenderWindow* window_;
	std::vector<Mesh*> meshes_;

	void draw_mesh(const Mesh* mesh, const Matrix4& mvp, Framebuffer* framebuffer) const
	{
		for (const auto& vertex : mesh->vertices) {
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
			const int screen_x = static_cast<int>((projected.x + 1.0f) * 0.5f * static_cast<float>(width_));
			const int screen_y = static_cast<int>((1.0f - (projected.y + 1.0f) * 0.5f) * static_cast<float>(height_));
			framebuffer->set_pixel(screen_x, screen_y, sf::Color::White);
		}
	}

public:
	Window(const unsigned int width, const unsigned int height, const sf::String& tittle) : width_(width), height_(height)
	{
		window_ = new sf::RenderWindow(sf::VideoMode({width, height}), tittle);
	}

	void run() const
	{
		const auto framebuffer = new Framebuffer(width_,height_);
		const auto camera = new CameraController();
		const Matrix4 proj = Matrix4::perspective(90.0f * 3.1415f / 180.0f, static_cast<float>(width_) / static_cast<float>(height_), 0.1f, 100.0f);
        while (window_->isOpen())
        {
            while (const std::optional event = window_->pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                    window_->close();
            }

            camera->handle_input();

            Matrix4 view = camera->getViewMatrix();
            Matrix4 model = Matrix4::identity();
            Matrix4 mvp = proj * view * model;

			framebuffer->clear(sf::Color::Black);
			for (const auto mesh : meshes_)
			{
				draw_mesh(mesh, mvp, framebuffer);
			}
			framebuffer->display(window_);
            window_->display();
        }
	}

	void add_mesh(Mesh* mesh)
	{
		meshes_.push_back(mesh);
	}
};
