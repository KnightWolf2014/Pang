#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"


Game::~Game() {
	if (menu != NULL) delete menu;
	if (scene != NULL) delete scene;
}

void Game::init()
{

	bPlay = true, start = false;
	viewType = 0, level = 0;
	glClearColor(0.f, 0.f, 0.f, 1.0f);

	SoundProgram::instance().init();
	engine = SoundProgram::instance().getSoundEngine();

	
	menu = new Menu();
	menu->init(viewType);

	scene = new Scene();
}

bool Game::update(int deltaTime)
{
	if (start) {
		if (viewType == 2) {
			scene->update(deltaTime);
		}
	}
	else {
		if (viewType == 0) menu->update(deltaTime);
	}

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (start) {
		if (viewType == 2) {
			scene->render();
		}
	}
	else {
		if (viewType == 0 || viewType == 1) menu->render();
	}

	//scene.render();
}

void Game::keyPressed(int key)
{
	if (key == GLFW_KEY_ESCAPE) { // Escape code
		if (viewType == 0) bPlay = false;
		else {
			viewType = 0;
			start = false;
			engine->removeAllSoundSources();
			menu->init(viewType);
		}
	}
	if (key == GLFW_KEY_I) {
		if (viewType == 0) {
			viewType = 1;
			menu->init(viewType);
		}
	}
	if (key == GLFW_KEY_SPACE) {
		if (viewType == 0) {
			start = true;
			level = 1;
			viewType = 2;
			scene->init(level);
		}
	}
	if (key == GLFW_KEY_1) {
		if (viewType == 2) {
			level = 1;
			scene->init(level);
		}
	}
	if (key == GLFW_KEY_2) {
		if (viewType == 2) {
			level = 2;
			scene->init(level);
		}
	}
	if (key == GLFW_KEY_3) {
		if (viewType == 2) {
			level = 3;
			scene->init(level);
		}
	}
	
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}



