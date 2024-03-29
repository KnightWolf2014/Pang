
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <chrono>
#include <thread>

#include <iostream>
#include <vector>
#include <algorithm>

#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 22
#define INIT_PLAYER_Y_TILES 21

#define TIME_HITBOX 500
#define TIME_TIMER 3000


Scene::Scene()
{
	map = NULL;
	player = NULL;
	levels = NULL;
	ui = NULL;
	menu = NULL;
	//bubble = NULL;

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
	if (menu != NULL)
		delete menu;
	/*if (bubble != NULL)
		delete bubble;*/
}


void Scene::init(const int& level, const int& lives, bool& godMode){
	
	currentTime = 0.0f;
	lvl = level;
	hp = lives;
	god = godMode;

	timerHitbox = TIME_HITBOX;
	timerTime = TIME_TIMER;
	activeHitbox = true;
	activeTime = false;

	cout << "Level " << level << endl;

	finish = false;

	engine = SoundProgram::instance().getSoundEngine();

	levels = new Levels();
	levels->init(level);

	ui = new UI();
	ui->init(level, hp, godMode);

	while (bubbles.size() > 0) bubbles.pop_back();

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
	player->setTileMap(map);
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));

	//if (!text.init("fonts/DroidSerif.ttf"))
	//	cout << "Could not load font!!!" << endl;

	//text.render("READY", glm::vec2(50, CAMERA_HEIGHT - 90), 40, glm::vec4(1, 1, 1, 1));

	if (level == 1) {
		Bubble* bubble1 = new Bubble();
		bubble1->init(glm::ivec2(0, 0), texProgram, 1, 0);
		bubble1->setPosition(glm::vec2(4 * map->getTileSize(), 2 * map->getTileSize()));
		bubble1->setTileMap(map);

		bubbles.push_back(bubble1);

		engine->removeAllSoundSources();
		engine->play2D("sounds/MtFuji.mp3");

	}
	if (level == 2) {
		Bubble* bubble1 = new Bubble();
		bubble1->init(glm::ivec2(0, 0), texProgram, 1, 0);
		bubble1->setPosition(glm::vec2(32 * map->getTileSize(), 2 * map->getTileSize()));
		bubble1->setTileMap(map);

		bubbles.push_back(bubble1);

		Bubble* bubble2 = new Bubble();
		bubble2->init(glm::ivec2(0, 0), texProgram, 3, 0);
		bubble2->setPosition(glm::vec2(12 * map->getTileSize(), 12 * map->getTileSize()));
		bubble2->setTileMap(map);

		bubbles.push_back(bubble2);

		Bubble* bubble3 = new Bubble();
		bubble3->init(glm::ivec2(0, 0), texProgram, 3, 0);
		bubble3->setPosition(glm::vec2(20 * map->getTileSize(), 12 * map->getTileSize()));
		bubble3->setTileMap(map);

		bubbles.push_back(bubble3);

		Bubble* bubble4 = new Bubble();
		bubble4->init(glm::ivec2(0, 0), texProgram, 3, 1);
		bubble4->setPosition(glm::vec2(32 * map->getTileSize(), 12 * map->getTileSize()));
		bubble4->setTileMap(map);

		bubbles.push_back(bubble4);

		engine->removeAllSoundSources();
		engine->play2D("sounds/London.mp3");
	}
	if (level == 3) {
		Bubble* bubble1 = new Bubble();
		bubble1->init(glm::ivec2(0, 0), texProgram, 1, 0);
		bubble1->setPosition(glm::vec2(4 * map->getTileSize(), 2 * map->getTileSize()));
		bubble1->setTileMap(map);

		bubbles.push_back(bubble1);

		Bubble* bubble2 = new Bubble();
		bubble2->init(glm::ivec2(0, 0), texProgram, 1, 0);
		bubble2->setPosition(glm::vec2(18 * map->getTileSize(), 1 * map->getTileSize()));
		bubble2->setTileMap(map);

		bubbles.push_back(bubble2);

		engine->removeAllSoundSources();
		engine->play2D("sounds/Barcelona.mp3");
	}


	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);


}

void Scene::update(int deltaTime, bool& godMode)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	for (auto& bubble: bubbles) bubble->update(deltaTime);
	ui->update(deltaTime, hp, godMode);

	if (!activeHitbox) {
		if (timerHitbox < 0) {
			timerHitbox = TIME_HITBOX;
			activeHitbox = true;
		}
		timerHitbox -= deltaTime;
	}

	if (!activeTime) {
		if (timerTime < 0) {
			activeTime = true;
		}
		timerTime -= deltaTime;
	}

	collisionBubbleHook();

	

	if (!godMode) {
		collisionBubblePlayer();
		timerOut();
	}

	if (bubbles.size() == 0 && (lvl < 3)) {
		cout << "next lvl!" << endl;
		lvl++;
		init(lvl, hp, godMode);
	}
	else if (bubbles.size() == 0 && lvl == 3) {
		cout << "credits!" << endl;
		finish = true;
	}
}

bool Scene::gameFinished() {
	return finish;
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
	for (auto& bubble : bubbles) bubble->render();
	ui->render();

}

void Scene::timerOut() {
	if (activeTime) {
		int time = ui->getCountDown();
		if (time == 0) {
			--hp;

			cout << "time" << endl;

			init(lvl, hp, god);
		}
	}
}

void Scene::divideBubble(Bubble* bubble, int type, int index) {

	engine->play2D("sounds/BubblePop.mp3");

	if (type == 1) {

		int posX = bubble->getPosX();
		int posY = bubble->getPosY();

		if (index >= 0 && index < bubbles.size()) {
			bubbles.erase(bubbles.begin() + index);
		}

		Bubble* bubble1 = new Bubble();
		bubble1->init(glm::ivec2(0, 0), texProgram, 2, 1);
		bubble1->setPosition(glm::vec2(posX, posY));
		bubble1->setTileMap(map);

		bubbles.push_back(bubble1);

		Bubble* bubble2 = new Bubble();
		bubble2->init(glm::ivec2(0, 0), texProgram, 2, 0);
		bubble2->setPosition(glm::vec2(posX, posY));
		bubble2->setTileMap(map);

		bubbles.push_back(bubble2);

	}
	if (type == 2) {

		int posX = bubble->getPosX();
		int posY = bubble->getPosY();

		if (index >= 0 && index < bubbles.size()) {
			bubbles.erase(bubbles.begin() + index);
		}

		Bubble* bubble1 = new Bubble();
		bubble1->init(glm::ivec2(0, 0), texProgram, 3, 1);
		bubble1->setPosition(glm::vec2(posX, posY));
		bubble1->setTileMap(map);

		bubbles.push_back(bubble1);

		Bubble* bubble2 = new Bubble();
		bubble2->init(glm::ivec2(0, 0), texProgram, 3, 0);
		bubble2->setPosition(glm::vec2(posX, posY));
		bubble2->setTileMap(map);

		bubbles.push_back(bubble2);

	}
	if (type == 3) {

		int posX = bubble->getPosX();
		int posY = bubble->getPosY();

		if (index >= 0 && index < bubbles.size()) {
			bubbles.erase(bubbles.begin() + index);
		}

		Bubble* bubble1 = new Bubble();
		bubble1->init(glm::ivec2(0, 0), texProgram, 4, 1);
		bubble1->setPosition(glm::vec2(posX, posY));
		bubble1->setTileMap(map);

		bubbles.push_back(bubble1);

		Bubble* bubble2 = new Bubble();
		bubble2->init(glm::ivec2(0, 0), texProgram, 4, 0);
		bubble2->setPosition(glm::vec2(posX, posY));
		bubble2->setTileMap(map);

		bubbles.push_back(bubble2);

	}
	if (type == 4) {

		if (index >= 0 && index < bubbles.size()) {
			bubbles.erase(bubbles.begin() + index);
		}
	}



}



void Scene::burst() {

	for (int index = 0; index < bubbles.size(); ++index) {
		Bubble* bubble = bubbles[index];
		int type = bubble->getType();

		//cout << "type: " << type << endl;
		
		divideBubble(bubble,type,index);


		cout << "index: " << index << endl;
		//cout << "maxRec: " << maxRec << endl;

	}

	//cout << "burst completo" << endl;
	cout << "--------------" << endl;
}

void Scene::updateTileMap(TileMap* mapV) {
	map = mapV;
	map->updateTileMap(glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
}

void Scene::collisionBubblePlayer() {
	posPlayerX = player->getPosX();
	posPlayerY = player->getPosY();
	sizePlayer = player->getSize();

	for (auto& bubble : bubbles) {
		posBubbleX = bubble->getPosX();
		posBubbleY = bubble->getPosY();
		sizeBubble = bubble->getSize();

		if (activeHitbox && map->collisionBubblePlayer(posPlayerX, posPlayerY, sizePlayer, posBubbleX, posBubbleY, sizeBubble)) {


			--hp;
			activeHitbox = false;
		}
	}
}

void Scene::collisionBubbleHook() {

	glm::ivec2 hookPos = player->getHookPos();
	int keyFrame = player->getHookKeyFrame();

	for (int index = 0; index < bubbles.size(); ++index) {
		Bubble* bubble = bubbles[index];
		int type = bubble->getType();

		posBubbleX = bubble->getPosX();
		posBubbleY = bubble->getPosY();
		sizeBubble = bubble->getSize();

		if (map->collisionBubbleHook(posBubbleX, posBubbleY, sizeBubble, hookPos, glm::ivec2(27, 567), keyFrame)) 
			divideBubble(bubble,type,index);
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



