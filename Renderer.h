#pragma once
#include "Framebuffer.h"
#include "Lighting.h"
#include "CameraController.h"
#include "Enums.h"
#include "Mesh.h"
#include "Matrix.h"
#include "Vector.h"
#include <vector>
#include <optional>
#include <SFML/Graphics/Color.hpp>
#include <cmath>
#include <limits>
#include <algorithm>

struct Vertex {
	Vector2<int> position;
	float z;
	Vector3<float> normal;
	sf::Color color;
};

class Renderer {
	int width_;
	int height_;
	Framebuffer* framebuffer_;
	float* depth_buffer_;
	Lighting* lighting_;
	ShadingMode shading_mode_;

public:
	Renderer(int width, int height, Framebuffer* framebuffer, float* depth_buffer, Lighting* lighting, ShadingMode mode)
		: width_(width), height_(height), framebuffer_(framebuffer), depth_buffer_(depth_buffer),
		lighting_(lighting), shading_mode_(mode) {
	}

	void set_shading_mode(ShadingMode mode) { shading_mode_ = mode; }

	void clear_depth() {
		for (int i = 0; i < width_ * height_; ++i)
			depth_buffer_[i] = std::numeric_limits<float>::max();
	}

	void draw_mesh(const Mesh& mesh, const Matrix4& mvp, const Matrix4& view, const CameraController& camera) {
		std::vector<std::optional<Vertex>> projected_vertices;
		for (size_t i = 0; i < mesh.vertices.size(); ++i) {
			const auto& vertex = mesh.vertices[i];
			Vector4 v(vertex.x, vertex.y, vertex.z, 1.0f);
			Vector4 projected = mvp * v;

			if (projected.w != 0.0f) {
				projected.x /= projected.w;
				projected.y /= projected.w;
				projected.z /= projected.w;
			}

			if (projected.x < -1 || projected.x > 1 || projected.y < -1 || projected.y > 1 || projected.z < -1 || projected.z > 1) {
				projected_vertices.emplace_back(std::nullopt);
				continue;
			}

			Vector3<float> n = mesh.normals[i];
			Vector4 n4(n.x, n.y, n.z, 0.0f);
			Vector4 n_transformed = view * n4;

			Vector3<float> normal_transformed = Vector3(
				n_transformed.x,
				n_transformed.y,
				n_transformed.z).normalized();

			float depth = (projected.z + 1.0f) * 0.5f;
			int screen_x = static_cast<int>((projected.x + 1.0f) * 0.5f * static_cast<float>(width_));
			int screen_y = static_cast<int>((1.0f - (projected.y + 1.0f) * 0.5f) * static_cast<float>(height_));

			sf::Color vertex_color = lighting_->calculate_color(normal_transformed, camera.position);

			projected_vertices.emplace_back(Vertex{ Vector2(screen_x, screen_y), depth, normal_transformed, vertex_color });
		}

		for (const auto& face : mesh.faces) {
			int v1 = face.x;
			int v2 = face.y;
			int v3 = face.z;
			if (projected_vertices[v1] && projected_vertices[v2] && projected_vertices[v3]) {
				const auto& vtx1 = projected_vertices[v3].value();
				const auto& vtx2 = projected_vertices[v2].value();
				const auto& vtx3 = projected_vertices[v1].value();
				draw_triangle(vtx1, vtx2, vtx3, camera);
			}
		}
	}

	void draw_triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2, const CameraController& camera) {
		auto a = v0.position;
		auto b = v1.position;
		auto c = v2.position;
		float za = v0.z;
		float zb = v1.z;
		float zc = v2.z;

		int xmin = static_cast<int>(std::floor(std::min({ a.x, b.x, c.x })));
		int ymin = static_cast<int>(std::floor(std::min({ a.y, b.y, c.y })));
		int xmax = static_cast<int>(std::ceil(std::max({ a.x, b.x, c.x })));
		int ymax = static_cast<int>(std::ceil(std::max({ a.y, b.y, c.y })));

		float area = getDeterminant(a, b, c);
		if (std::abs(area) < 1e-6f) return;

		sf::Color face_color = sf::Color::White;
		if (shading_mode_ == ShadingMode::FLAT) {
			Vector3<float> face_normal = (v1.normal + v2.normal + v0.normal).normalized();
			face_color = lighting_->calculate_color(face_normal, camera.position);
		}

		auto randomColor = sf::Color(rand() % 256, rand() % 256, rand() % 256);
		for (int y = ymin; y <= ymax; ++y) {
			for (int x = xmin; x <= xmax; ++x) {
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
							auto interp = [&](uint8_t ca, uint8_t cb, uint8_t cc) -> uint8_t {
								return static_cast<uint8_t>(
									alpha * static_cast<float>(ca) +
									beta * static_cast<float>(cb) +
									gamma * static_cast<float>(cc));
								};
							color = sf::Color(
								interp(v0.color.r, v1.color.r, v2.color.r),
								interp(v0.color.g, v1.color.g, v2.color.g),
								interp(v0.color.b, v1.color.b, v2.color.b));
						}
						else if (shading_mode_ == ShadingMode::PHONG) {
							Vector3<float> interpolated_normal =
								(v0.normal * alpha + v1.normal * beta + v2.normal * gamma).normalized();
							color = lighting_->calculate_color(interpolated_normal, camera.position);
						}

						framebuffer_->set_pixel(x, y, color);
						depth_buffer_[y * width_ + x] = z;
					}
				}
			}
		}
	}

private:
	static float getDeterminant(const Vector2<float>& a, const Vector2<float>& b, const Vector2<float>& c) {
		return (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y);
	}
};
