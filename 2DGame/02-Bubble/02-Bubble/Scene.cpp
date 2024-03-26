
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <chrono>
#include <thread>

#include <iostream>

#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 22
#define INIT_PLAYER_Y_TILES 21

#define TIME_HITBOX 150


Scene::Scene()
{
	map = NULL;
	player = NULL;
	levels = NULL;
	ui = NULL;
	bubble = NULL;

	engine = SoundProgram::instance().getSoundEngine();
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
	if (bubble != NULL)
		delete bubble;
}


void Scene::init(const int& level, const int& lives, bool& godMode){
	
	currentTime = 0.0f;
	lvl = level;
	hp = lives;
	god = godMode;

	timerHitbox = TIME_HITBOX;
	activeHitbox = true;

	cout << "Level " << level << endl;

	engine = SoundProgram::instance().getSoundEngine();

	levels = new Levels();
	levels->init(level);

	ui = new UI();
	ui->init(level, hp, godMode);


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

	//if (!text.init("fonts/DroidSerif.ttf"))
	//	cout << "Could not load font!!!" << endl;

	//text.render("READY", glm::vec2(50, CAMERA_HEIGHT - 90), 40, glm::vec4(1, 1, 1, 1));

	if (level == 1) {
		bubble = new Bubble();
		bubble->init(glm::ivec2(0, 0), texProgram, 1);
		bubble->setPosition(glm::vec2(4 * map->getTileSize(), 2 * map->getTileSize()));
		bubble->setTileMap(map);


		engine->removeAllSoundSources();
		engine->play2D("sounds/MtFuji.mp3");

	}
	if (level == 2) {
		bubble = new Bubble();
		bubble->init(glm::ivec2(0, 0), texProgram, 1);
		bubble->setPosition(glm::vec2(4 * map->getTileSize(), 2 * map->getTileSize()));
		bubble->setTileMap(map);

		engine->removeAllSoundSources();
		engine->play2D("sounds/London.mp3");
	}
	if (level == 3) {
		bubble = new Bubble();
		bubble->init(glm::ivec2(0, 0), texProgram, 1);
		bubble->setPosition(glm::vec2(4 * map->getTileSize(), 2 * map->getTileSize()));
		bubble->setTileMap(map);

		engine->removeAllSoundSources();
		engine->play2D("sounds/Barcelona.mp3");
	}


	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);

	// Hem de mirar on colocar aixo (per coordinar la música)
	//
	//this_thread::sleep_for(chrono::milliseconds(2000));

}

void Scene::update(int deltaTime, bool& godMode)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	bubble->update(deltaTime);
	ui->update(deltaTime, hp, godMode);

	if (!activeHitbox) {
		if (timerHitbox < 0) {
			timerHitbox = TIME_HITBOX;
			activeHitbox = true;
		}
		timerHitbox -= deltaTime;
	}

	if (!godMode) {
		collisionBubblePlayer();
		timerOut();
	}
}

bool Scene::gameOver() {
	if (hp == 0) return true;
	else return false;
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

void Scene::timerOut() {
	int time = ui->getCountDown();
	if (time == 0) {
		--hp;

		init(lvl, hp, god);
	}
}

void Scene::collisionBubblePlayer() {
	posPlayerX = player->getPosX();
	posPlayerY = player->getPosY();
	sizePlayer = player->getSize();

	posBubbleX = bubble->getPosX();
	posBubbleY = bubble->getPosY();
	sizeBubble = bubble->getSize();

	/*cout << "playerPos: (" << posPlayerX << ", " << posPlayerY << "), playerSize: " << sizePlayer << endl;
	cout << "bubblePos: (" << posBubbleX << ", " << posBubbleY << "), bubbleSize: " << sizeBubble << endl;
	cout << "-------------------------" << endl;*/

	if (activeHitbox && map->collisionBubblePlayer(posPlayerX, posPlayerY, sizePlayer, posBubbleX, posBubbleY, sizeBubble)) {

		--hp;
		activeHitbox = false;

		/*cout << endl;
		cout << endl;
		cout << endl;
		cout << endl;
		cout << endl;
		cout << "-------------------------" << endl;
		cout << "TOCADOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO" << endl;
		cout << "-------------------------" << endl;
		cout << endl;
		cout << endl;
		cout << endl;
		cout << endl;
		cout << endl;*/
	}
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



