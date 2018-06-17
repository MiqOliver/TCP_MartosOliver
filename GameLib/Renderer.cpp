#include "Renderer.h"

Renderer& Renderer::GetInstance() {
	static Renderer renderer;
	return renderer;
}

Renderer::~Renderer() {
	for each (sf::Sprite* s in sprites)
		delete s;
	for each (sf::Texture* t in textures)
		delete t;
}

void Renderer::CreateWindow(unsigned int w, unsigned int h, std::string name) {
	window.create(sf::VideoMode(w, h), name);
}

void Renderer::SetBackground(std::string path) {
	if (!bg_texture.loadFromFile(path))
		std::cout << "Couldn't load the background" << std::endl;
	bg_sprite.setTexture(bg_texture);
	bg_sprite.setPosition(0.f, 0.f);

	textures.push_back(&bg_texture);
	sprites.push_back(&bg_sprite);
}

void Renderer::Draw() {
	window.clear();

	for each (sf::Sprite* s in sprites)
		window.draw(*s);

	window.display();
}