#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "UI.h"

UI::UI() {
	quad = NULL;
}

UI::~UI() {
	if (quad != NULL)
		delete quad;
	for (int i = 0; i < 3; i++)
		if (texQuad[i] != NULL)
			delete texQuad[i];
}

void UI::init(const int& level) {
	
	game_ui = level;


	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(36.f, 36.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };


	initShaders();

	currentTime = 0.0f;
	timeAccumulator = 0.0f;
	showInsertCoin = false;

	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f);
	texQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f);
	texQuad[1] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f);
	texQuad[2] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	// Load textures
	texs[0].loadFromFile("images/hp.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[0].setMagFilter(GL_NEAREST);



	projection = glm::ortho(0.f, float(CAMERA_WIDTH), float(CAMERA_HEIGHT), 0.f);

	if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
}

void UI::update(int deltaTime) {
	currentTime += deltaTime;
	timeAccumulator += deltaTime;
}

void UI::render() {

	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	// Renderizar texQuad[0] en la parte inferior
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(50.f, CAMERA_HEIGHT - 40.f, 0.f));  // Cambia la altura seg�n sea necesario
	modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[0]->render(texs[0]);

	// Renderizar texQuad[1] en la parte inferior
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(90.f, CAMERA_HEIGHT - 40.f, 0.f));  // Cambia la altura seg�n sea necesario
	modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[1]->render(texs[0]);

	// Renderizar texQuad[2] en la parte inferior
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(130.f, CAMERA_HEIGHT - 40.f, 0.f));  // Cambia la altura seg�n sea necesario
	modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[2]->render(texs[0]);




	if (game_ui == 1){
		text.render("MT.FUJI", glm::vec2(CAMERA_WIDTH / 2 - 65, CAMERA_HEIGHT - 90), 32, glm::vec4(1, 1, 1, 1));
		text.render("STAGE 1", glm::vec2(CAMERA_WIDTH / 2 - 55, CAMERA_HEIGHT - 40), 26, glm::vec4(1, 1, 1, 1));
	}
	if (game_ui == 2) {
		text.render("LONDON", glm::vec2(CAMERA_WIDTH / 2 - 70, CAMERA_HEIGHT - 90), 32, glm::vec4(1, 1, 1, 1));
		text.render("STAGE 2", glm::vec2(CAMERA_WIDTH / 2 - 55, CAMERA_HEIGHT - 40), 26, glm::vec4(1, 1, 1, 1));
	}
	if (game_ui == 3) {
		text.render("BARCELONA", glm::vec2(CAMERA_WIDTH / 2 - 100, CAMERA_HEIGHT - 90), 32, glm::vec4(1, 1, 1, 1));
		text.render("STAGE 3", glm::vec2(CAMERA_WIDTH / 2 - 55, CAMERA_HEIGHT - 40), 26, glm::vec4(1, 1, 1, 1));
	}

	text.render("PLAYER-1", glm::vec2(50, CAMERA_HEIGHT - 90), 32, glm::vec4(1, 1, 1, 1));
	text.render("0000000", glm::vec2(150, CAMERA_HEIGHT - 60), 32, glm::vec4(1, 1, 1, 1));



	text.render("PLAYER-2", glm::vec2(CAMERA_WIDTH - 300, CAMERA_HEIGHT - 90), 32, glm::vec4(1, 1, 1, 1));

	if (timeAccumulator >= 500.0f) {
		showInsertCoin = !showInsertCoin;
		timeAccumulator = 0.0f;
	}
	if (showInsertCoin) text.render("INSERT COIN", glm::vec2(CAMERA_WIDTH - 300, CAMERA_HEIGHT - 40), 26, glm::vec4(1, 1, 1, 1));
}

void UI::initShaders()
{

	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/simple.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/simple.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleProgram.init();
	simpleProgram.addShader(vShader);
	simpleProgram.addShader(fShader);
	simpleProgram.link();
	if (!simpleProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleProgram.log() << endl << endl;
	}
	simpleProgram.bindFragmentOutput("outColor");

	vShader.free();
	fShader.free();
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
}