#include <SFML/Graphics.hpp>

#include "Mesh.h"
#include "Window.h"

int main()
{
	constexpr unsigned int width = 1000;
	constexpr unsigned int height = 1000;

	const auto window = std::make_unique<Window>(width, height, "Pipeline");

	auto mesh = std::make_unique<Mesh>();

	if (!mesh->load_from_obj("crashbandicoot.obj")) {
		std::cerr << "Failed to load crashbandicoot.obj" << "\n";
		return -1;
	}

	window->add_mesh(std::move(mesh));
	window->run();
	
	return 0;
}
