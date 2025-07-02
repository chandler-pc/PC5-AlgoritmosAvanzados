#include <SFML/Graphics.hpp>

#include "Mesh.h"
#include "Window.h"

int main()
{
	constexpr unsigned int width = 800;
	constexpr unsigned int height = 800;

	const auto window = std::make_unique<Window>(width, height, "Pipeline");

	const auto teapot_mesh = std::make_unique<Mesh>();
	if (!teapot_mesh->load_from_obj("teapot.obj")) {
		std::cerr << "Failed to load teapot.obj" << "\n";
		return -1;
	}

	window->add_mesh(teapot_mesh.get());
	window->run();
	
	return 0;
}
