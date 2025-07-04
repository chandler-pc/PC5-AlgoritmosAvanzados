#include <SFML/Graphics.hpp>

#include "Mesh.h"
#include "Window.h"

int main()
{
	std::string mesh_name;
	std::cin >> mesh_name;


	constexpr unsigned int width = 1000;
	constexpr unsigned int height = 1000;

	const auto window = std::make_unique<Window>(width, height, "Pipeline");

	auto mesh = std::make_unique<Mesh>();


	if (!mesh->load_from_obj(mesh_name)) {
		std::cerr << "Failed to load teapot.obj" << "\n";
		return -1;
	}

	window->add_mesh(std::move(mesh));
	window->run();
	
	return 0;
}
