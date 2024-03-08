#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Menu.h"

Menu::Menu() {
	background = NULL;
}

Menu::~Menu() {
	if (background != NULL)
		delete background;
}

void Menu::init(const int& viewType) {

	currentTime = 0.0f;

	initShaders();

	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(1152.f, 624.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	if (viewType == 0) {
		scene.loadFromFile("images/Menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	}
	if (viewType == 1) {
		scene.loadFromFile("images/Instructions.png", TEXTURE_PIXEL_FORMAT_RGBA);
	}

	scene.setMinFilter(GL_NEAREST);
	scene.setMagFilter(GL_NEAREST);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
}

void Menu::update(int deltaTime) {
	currentTime += deltaTime;

}

void Menu::render() {

	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	background->render(scene);
}

void Menu::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}
