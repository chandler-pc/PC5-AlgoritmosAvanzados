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
	sf::RenderWindow window_;
	std::vector<std::unique_ptr<Mesh>> meshes_;
	std::unique_ptr<Framebuffer> framebuffer_;

	void draw_mesh(const std::unique_ptr<Mesh>& mesh, const Matrix4& mvp) const
	{
		std::vector<std::optional<Vector2>> projected_vertices;

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
				projected_vertices.emplace_back(std::nullopt);
				continue;
			}
			const int screen_x = static_cast<int>((projected.x + 1.0f) * 0.5f * static_cast<float>(width_));
			const int screen_y = static_cast<int>((1.0f - (projected.y + 1.0f) * 0.5f) * static_cast<float>(height_));
			projected_vertices.emplace_back(Vector2(screen_x, screen_y));
		}

		for (const auto& face : mesh->faces) {
			const int v1 = static_cast<int>(face.x);
			const int v2 = static_cast<int>(face.y);
			const int v3 = static_cast<int>(face.z);
			if (projected_vertices[v1] && projected_vertices[v2] && projected_vertices[v3]) {
				draw_triangle(projected_vertices[v1].value(), projected_vertices[v2].value(), projected_vertices[v3].value());
			}
		}

		for (const auto& edge : mesh->get_edges()) {
			const int x = static_cast<int>(edge.x);
			const int y = static_cast<int>(edge.y);

			if (!projected_vertices[x] || !projected_vertices[y]) {
				continue;
			}
			const auto& start = projected_vertices[x].value();
			const auto& end = projected_vertices[y].value();

			framebuffer_->draw_line(start.x, start.y, end.x, end.y);
		}

		for (const auto& vertex : projected_vertices) {
			if (vertex) {
				framebuffer_->set_pixel(static_cast<unsigned int>(vertex->x), static_cast<unsigned int>(vertex->y), sf::Color::Red);
			}
		}
	}

	void draw_triangle(Vector2 a, Vector2 b, Vector2 c) const
	{
		const int xmin = std::min(a.x, std::min(b.x, c.x));
		const int ymin = std::min(a.y, std::min(b.y, c.y));

		const int xmax = std::max(a.x, std::max(b.x, c.x));
		const int ymax = std::max(a.y, std::max(b.y, c.y));
		for (int y = ymin; y <= ymax; y++) {
			for (int x = xmin; x <= xmax; x++) {
				Vector2 p;
				p.x = x;
				p.y = y;
				Vector3 w;
				w.x = getDeterminant(b, c, p);
				w.y = getDeterminant(c, a, p);
				w.z = getDeterminant(a, b, p);

				if (w.x >= 0 && w.y >= 0 && w.z >= 0) {
					framebuffer_->set_pixel(p.x, p.y);
				}
			}
		}
	}

	static float getDeterminant(const Vector2& a, const Vector2& b, const Vector2& c)
	{
		return (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y);
	}

public:
	Window(const unsigned int width, const unsigned int height, const sf::String& title) : width_(width), height_(height), window_(sf::VideoMode({ width, height }), title)
	{
	}

	void run()
	{
		framebuffer_ = std::make_unique<Framebuffer>(width_,height_);
		auto camera = CameraController();
		const Matrix4 proj = Matrix4::perspective(90.0f * 3.1415f / 180.0f, static_cast<float>(width_) / static_cast<float>(height_), 0.1f, 100.0f);
        while (window_.isOpen())
        {
            while (const std::optional event = window_.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                    window_.close();
            }

            camera.handle_input();

            Matrix4 view = camera.getViewMatrix();
            Matrix4 model = Matrix4::identity();
            Matrix4 mvp = proj * view * model;

			framebuffer_->clear(sf::Color::Black);
			for (const auto& mesh : meshes_)
			{
				draw_mesh(mesh, mvp);
			}
			framebuffer_->display(&window_);
            window_.display();
        }
	}

	void add_mesh(std::unique_ptr<Mesh> mesh)
	{
		meshes_.push_back(std::move(mesh));
	}
};
