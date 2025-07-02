#include <SFML/Graphics.hpp>

#include "Mesh.h"
#include "Window.h"

int main()
{
	constexpr unsigned int width = 800;
	constexpr unsigned int height = 800;

	const auto window = new Window(width, height, "Pipeline");

	const auto teapot_mesh = new Mesh();
	if (!teapot_mesh->load_from_obj("teapot.obj")) {
		return -1;
	}

	window->add_mesh(teapot_mesh);
	window->run();

	delete window;
	delete teapot_mesh;
	return 0;
}
