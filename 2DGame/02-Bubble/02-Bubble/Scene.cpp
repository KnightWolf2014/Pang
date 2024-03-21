#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 22
#define INIT_PLAYER_Y_TILES 21


Scene::Scene()
{
	map = NULL;
	player = NULL;
	levels = NULL;
	ui = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (levels != NULL)
		delete levels;
	if (ui != NULL)
		delete ui;
}


void Scene::init(const int& level){
	
	currentTime = 0.0f;

	cout << "Level " << level << endl;

	levels = new Levels();
	levels->init(level);

	ui = new UI();
	ui->init(level);


	initShaders();

	if (level == 1) {
		map = TileMap::createTileMap("levels/pangLevel01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	}
	if (level == 2) {
		map = TileMap::createTileMap("levels/pangLevel02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	}
	if (level == 3) {
		map = TileMap::createTileMap("levels/pangLevel03.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	}

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);


	bubble = new Bubble();
	bubble->init(glm::ivec2(0, 0), texProgram);
	if (level == 1) {

		bubble->setPosition(glm::vec2(4 * map->getTileSize(), 2 * map->getTileSize()));
		bubble->setTileMap(map);
	}


	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);

}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	bubble->update(deltaTime);
	ui->update(deltaTime);
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	levels->render();
	map->render();
	player->render();
	bubble->render();
	ui->render();

}



void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



