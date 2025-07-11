#include "Scene.h"

Scene::Scene(sf::RenderWindow& window) : window(window), uiManager(nullptr)
{
	// Initialize the scene
}

void Scene::display()
{
	window.display();
}