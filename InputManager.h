#pragma once
#include <SFML/Window/Keyboard.hpp>
#include "Enums.h"

class InputManager {
	bool key_f1_was_pressed_ = false;
	bool key_f2_was_pressed_ = false;
	bool key_f3_was_pressed_ = false;
	bool key_f4_was_pressed_ = false;

public:
	void update(ShadingMode& shading_mode, ProjectionMode& projection_mode) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F1)) {
			if (!key_f1_was_pressed_) {
				shading_mode = ShadingMode::FLAT;
				key_f1_was_pressed_ = true;
			}
		}
		else key_f1_was_pressed_ = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F2)) {
			if (!key_f2_was_pressed_) {
				shading_mode = ShadingMode::GOURAUD;
				key_f2_was_pressed_ = true;
			}
		}
		else key_f2_was_pressed_ = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F3)) {
			if (!key_f3_was_pressed_) {
				shading_mode = ShadingMode::PHONG;
				key_f3_was_pressed_ = true;
			}
		}
		else key_f3_was_pressed_ = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F4)) {
			if (!key_f4_was_pressed_) {
				projection_mode = (projection_mode == ProjectionMode::PERSPECTIVE)
					? ProjectionMode::ORTHOGRAPHIC
					: ProjectionMode::PERSPECTIVE;
				key_f4_was_pressed_ = true;
			}
		}
		else key_f4_was_pressed_ = false;
	}
};
