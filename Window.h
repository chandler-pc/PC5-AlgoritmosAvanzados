#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <limits>
#include <vector>
#include <memory>

#include "CameraController.h"
#include "Framebuffer.h"
#include "Matrix.h"
#include "Vector.h"
#include "Mesh.h"
#include "Lighting.h"
#include "InputManager.h"
#include "Renderer.h"

class Window {
	unsigned int width_;
	unsigned int height_;
	sf::RenderWindow window_;
	std::vector<std::unique_ptr<Mesh>> meshes_;
	std::unique_ptr<Framebuffer> framebuffer_;
	std::unique_ptr<Lighting> lighting_;
	std::unique_ptr<Renderer> renderer_;
	float* depth_buffer_ = nullptr;
	float fps_ = 0.f;
	sf::Clock clock_;
	ShadingMode shading_mode_ = ShadingMode::PHONG;
	ProjectionMode projection_mode_ = ProjectionMode::PERSPECTIVE;
	InputManager input_manager_;

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
		depth_buffer_(new float[width * height]), shading_mode_(ShadingMode::PHONG)
	{
		for (unsigned int i = 0; i < width * height; ++i)
			depth_buffer_[i] = std::numeric_limits<float>::max();

		clock_ = sf::Clock();
		lighting_ = std::make_unique<Lighting>();
		framebuffer_ = std::make_unique<Framebuffer>(width_, height_);
		renderer_ = std::make_unique<Renderer>(width_, height_, framebuffer_.get(), depth_buffer_, lighting_.get(), shading_mode_);
	}

	void run() {
		auto camera = CameraController();

		while (window_.isOpen()) {
			sf::Time delta_time = clock_.restart();

			while (const std::optional event = window_.pollEvent()) {
				if (event->is<sf::Event::Closed>())
					window_.close();
			}

			camera.handle_input();
			input_manager_.update(shading_mode_, projection_mode_);
			renderer_->set_shading_mode(shading_mode_);
			renderer_->clear_depth();
			framebuffer_->clear(sf::Color::Black);

			Matrix4 model = Matrix4::identity();
			Matrix4 view = camera.getViewMatrix();
			Matrix4 proj = get_projection_matrix();
			Matrix4 mvp = proj * view * model;

			for (const auto& mesh : meshes_) {
				renderer_->draw_mesh(*mesh, mvp, view, camera);
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

	void add_mesh(std::unique_ptr<Mesh> mesh) {
		meshes_.push_back(std::move(mesh));
	}
};
