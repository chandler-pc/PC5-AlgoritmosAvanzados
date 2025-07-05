#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "CameraController.h"
#include "Framebuffer.h"
#include "Matrix.h"
#include "Vector.h"
#include "Mesh.h"
#include <limits>
#include <SFML/System/Clock.hpp>
#include "Lighting.h"
#include <SFML/Window/Keyboard.hpp>
#include "InputManager.h"

struct Vertex
{
	Vector2<int> position;
	float z;
	Vector3<float> normal;
	sf::Color color;
};


class Window {
	unsigned int width_;
	unsigned int height_;
	sf::RenderWindow window_;
	std::vector<std::unique_ptr<Mesh>> meshes_;
	std::unique_ptr<Framebuffer> framebuffer_;
	std::unique_ptr<Lighting> lighting_;
	float* depth_buffer_ = nullptr;
	float fps_ = 0.f;
	float last_time_ = 0.f;
	sf::Clock clock_;
	ShadingMode shading_mode_ = ShadingMode::PHONG;
	ProjectionMode projection_mode_ = ProjectionMode::PERSPECTIVE;
	InputManager input_manager_;



	void draw_mesh(const std::unique_ptr<Mesh>& mesh, const Matrix4& mvp, const Matrix4& view, const CameraController camera) const
	{
		std::vector<std::optional<Vertex>> projected_vertices;

		for (size_t i = 0; i < mesh->vertices.size(); ++i) {
			const auto& vertex = mesh->vertices[i];
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

			Vector3<float> n = mesh->normals[i];
			Vector4 n4(n.x, n.y, n.z, 0.0f);
			Vector4 n_transformed = view * n4;

			Vector3<float> normal_transformed = Vector3(
				n_transformed.x,
				n_transformed.y,
				n_transformed.z).normalized();

			const float depth = (projected.z + 1.0f) * 0.5f;

			const int screen_x = static_cast<int>((projected.x + 1.0f) * 0.5f * static_cast<float>(width_));
			const int screen_y = static_cast<int>((1.0f - (projected.y + 1.0f) * 0.5f) * static_cast<float>(height_));

			sf::Color vertex_color = lighting_->calculate_color(normal_transformed, camera.position);

			projected_vertices.emplace_back(Vertex{ Vector2(screen_x, screen_y), depth, normal_transformed, vertex_color });
		}

		for (const auto& face : mesh->faces) {
			const int v1 = (face.x);
			const int v2 = (face.y);
			const int v3 = (face.z);
			if (projected_vertices[v1] && projected_vertices[v2] && projected_vertices[v3]) {
				const auto& vtx1 = projected_vertices[v3].value();
				const auto& vtx2 = projected_vertices[v2].value();
				const auto& vtx3 = projected_vertices[v1].value();
				draw_triangle(vtx1, vtx2, vtx3, camera);
			}
		}

		/*for (const auto& edge : mesh->get_edges()) {
			const int x = (edge.x);
			const int y = (edge.y);

			if (!projected_vertices[x] || !projected_vertices[y]) {
				continue;
			}
			const auto& start = projected_vertices[x].value();
			const auto& end = projected_vertices[y].value();

			framebuffer_->draw_line(start.position.x, start.position.y, end.position.x, end.position.y);
		}

		for (const auto& vertex : projected_vertices) {
			if (vertex) {
				framebuffer_->set_pixel(static_cast<unsigned int>(vertex->position.x), static_cast<unsigned int>(vertex->position.y), sf::Color::Red);
			}
		}*/
	}

	void draw_triangle(
		const Vertex& v0,
		const Vertex& v1,
		const Vertex& v2,
		CameraController camera
	) const
	{
		auto a = v0.position;
		auto b = v1.position;
		auto c = v2.position;
		float za = v0.z;
		float zb = v1.z;
		float zc = v2.z;

		const int xmin = static_cast<int>(std::floor(std::min({ a.x, b.x, c.x })));
		const int ymin = static_cast<int>(std::floor(std::min({ a.y, b.y, c.y })));
		const int xmax = static_cast<int>(std::ceil(std::max({ a.x, b.x, c.x })));
		const int ymax = static_cast<int>(std::ceil(std::max({ a.y, b.y, c.y })));

		float area = getDeterminant(a, b, c);
		if (std::abs(area) < 1e-6f) return;

		sf::Color face_color = sf::Color::White;
		if (shading_mode_ == ShadingMode::FLAT) {
			Vector3<float> face_normal = (v1.normal + v2.normal + v0.normal).normalized();
			face_color = lighting_->calculate_color(face_normal, camera.position);
		}

		for (int y = ymin; y <= ymax; y++) {
			for (int x = xmin; x <= xmax; x++) {
				if (x < 0 || x >= width_ || y < 0 || y >= height_) continue;
				Vector2 p(x, y);

				float w0 = getDeterminant(b, c, p);
				float w1 = getDeterminant(c, a, p);
				float w2 = getDeterminant(a, b, p);

				if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
					float alpha = w0 / area;
					float beta = w1 / area;
					float gamma = w2 / area;
					float z = 1.0f / (alpha / za + beta / zb + gamma / zc);

					if (z < depth_buffer_[y * width_ + x]) {
						sf::Color color;

						if (shading_mode_ == ShadingMode::FLAT) {
							color = face_color;
						}
						else if (shading_mode_ == ShadingMode::GOURAUD) {
							auto interpolate_channel = [&](uint8_t a, uint8_t b, uint8_t c) -> uint8_t {
								return static_cast<uint8_t>(
									alpha * static_cast<float>(a) +
									beta * static_cast<float>(b) +
									gamma * static_cast<float>(c));
								};
							color = sf::Color(
								interpolate_channel(v0.color.r, v1.color.r, v2.color.r),
								interpolate_channel(v0.color.g, v1.color.g, v2.color.g),
								interpolate_channel(v0.color.b, v1.color.b, v2.color.b));
						}
						else if (shading_mode_ == ShadingMode::PHONG) {
							Vector3<float> interpolated_normal = (v0.normal * alpha + v1.normal * beta + v2.normal * gamma).normalized();
							color = lighting_->calculate_color(interpolated_normal, camera.position);
						}

						framebuffer_->set_pixel(x, y, color);
						depth_buffer_[y * width_ + x] = z;
					}
				}
			}
		}
	}


	static float getDeterminant(const Vector2<float>& a, const Vector2<float>& b, const Vector2<float>& c)
	{
		return (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y);
	}

	Matrix4 get_projection_matrix() const {
		if (projection_mode_ == ProjectionMode::PERSPECTIVE) {
			return Matrix4::perspective(
				90.0f * 3.1415f / 180.0f,
				static_cast<float>(width_) / static_cast<float>(height_),
				0.1f,
				100.0f);
		}
		else {
			float aspect = static_cast<float>(width_) / static_cast<float>(height_);
			float ortho_width = 10.0f;
			float ortho_height = ortho_width / aspect;
			return Matrix4::orthographic(
				-ortho_width / 2, ortho_width / 2,
				ortho_height / 2, -ortho_height / 2,
				0.1f,
				100.0f);
		}
	}



public:
	Window(const unsigned int width, const unsigned int height, const sf::String& title)
		: width_(width), height_(height), window_(sf::VideoMode({ width, height }), title),
		depth_buffer_(new float[width_ * height_]), shading_mode_(ShadingMode::PHONG)
	{
		for (unsigned int i = 0; i < width * height; ++i) {
			depth_buffer_[i] = std::numeric_limits<float>::max();
		}
		clock_ = sf::Clock();
		lighting_ = std::make_unique<Lighting>();
	}

	void run()
	{
		framebuffer_ = std::make_unique<Framebuffer>(width_,height_);
		auto camera = CameraController();
		Matrix4 proj = Matrix4::perspective(90.0f * 3.1415f / 180.0f, static_cast<float>(width_) / static_cast<float>(height_), 0.1f, 100.0f);
        while (window_.isOpen())
        {
			sf::Time delta_time = clock_.restart();
            while (const std::optional event = window_.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                    window_.close();
            }

            camera.handle_input();

			input_manager_.update(shading_mode_, projection_mode_);


            Matrix4 view = camera.getViewMatrix();
            Matrix4 model = Matrix4::identity();
			proj = get_projection_matrix();

            Matrix4 mvp = proj * view * model;

			framebuffer_->clear(sf::Color::Black);
			for (unsigned int i = 0; i < width_ * height_; ++i) {
				depth_buffer_[i] = std::numeric_limits<float>::max();
			}
			for (const auto& mesh : meshes_)
			{
				draw_mesh(mesh, mvp, view, camera);
			}
			framebuffer_->display(&window_);
            window_.display();
			fps_ = 1.f / delta_time.asSeconds();
			std::string mode_str;
			switch (shading_mode_) {
				case ShadingMode::FLAT: mode_str = "Flat"; break;
				case ShadingMode::GOURAUD: mode_str = "Gouraud"; break;
				case ShadingMode::PHONG: mode_str = "Phong"; break;
			}
			window_.setTitle("Pipeline - FPS: " + std::to_string(static_cast<int>(fps_)) + " | " + mode_str);
        }
	}

	void add_mesh(std::unique_ptr<Mesh> mesh)
	{
		meshes_.push_back(std::move(mesh));
	}
};
