#pragma once
#pragma once
#include "Vector.h"
#include <SFML/Graphics/Color.hpp>
#include <algorithm>
#include <cmath>

class Lighting {
	Vector3<float> light_position_;
	float ambient_;
	float diffuse_;
	float specular_;
	float shininess_;

public:
	Lighting(
		Vector3<float> light_position = { 0.0f, 10.0f, 10.0f },
		float ambient = 0.1f,
		float diffuse = 1.0f,
		float specular = 0.5f,
		float shininess = 100.0f)
		: light_position_(light_position),
		ambient_(ambient),
		diffuse_(diffuse),
		specular_(specular),
		shininess_(shininess) {
	}

	sf::Color calculate_color(const Vector3<float>& normal, const Vector3<float>& view_dir) const {
		Vector3<float> N = normal.normalized();
		Vector3<float> L = (light_position_).normalized();
		Vector3<float> V = view_dir.normalized();
		Vector3<float> R = (N * (2.0f * N.dot(L)) - L).normalized();

		float diff = std::max(0.0f, N.dot(L));
		float spec = std::pow(std::max(0.0f, R.dot(V)), shininess_);

		float intensity = ambient_ + diffuse_ * diff + specular_ * spec;
		intensity = std::clamp(intensity, 0.0f, 1.0f);

		int color_value = static_cast<int>(intensity * 255);
		return sf::Color(color_value, color_value, color_value);
	}
};
