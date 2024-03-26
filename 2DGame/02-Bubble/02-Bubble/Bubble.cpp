#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Bubble.h"


#define JUMP_ANGLE_STEP 1
#define FALL_STEP 4


enum BubbleAnims
{
	FASEONE, FASETWO, FASETHREE,FASEFOUR
};


void Bubble::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int type)
{
	falling = false;

	movement = true;

	jumpAngle = 90;
	startY = 472;
	direction = 0;
	alturaMax = 72;


	if (type == 1) tamany = glm::ivec2(128, 128);
	if (type == 2) tamany = glm::ivec2(64, 64);
	if (type == 3) tamany = glm::ivec2(32, 32);
	if (type == 4) tamany = glm::ivec2(16, 16);


	spritesheet.loadFromFile("images/bubbleRock.png", TEXTURE_PIXEL_FORMAT_RGBA);
	
	sprite = Sprite::createSprite(tamany, glm::vec2(1, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(FASEONE, 16);
	sprite->addKeyframe(FASEONE, glm::vec2(0.0f, 0.f));


	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + 3 * posBubble.x), float(tileMapDispl.y + 3 * posBubble.y)));

}

int Bubble::getPosX() {
	return posBubble.x;
}

int Bubble::getPosY() {
	return posBubble.y;
}

int Bubble::getSize() {
	return tamany.x;
}

void Bubble::update(int deltaTime)
{
	sprite->update(deltaTime); 

	//cout << "posBubble.y: " << posBubble.y << endl;
	//cout << "jumpAngle: " << jumpAngle << endl;


	if (map->bubbleCollisionMoveUp(posBubble, tamany, &posBubble.y)) {
		//cout << "UUUUUUUUUUUUUUUUUUUUUUP" << endl;
		jumpAngle = 90;
		if (alturaMax >= 72) alturaMax = posBubble.y;
		else alturaMax = 72;
	}

	if (map->bubbleCollisionFloor(posBubble, tamany, &posBubble.y)) {
		alturaMax = 72;
	}

	float alturaAux;
	if (startY > alturaMax) {
		alturaAux = (startY - alturaMax) * 45 / 400;
	}
	else {
		alturaAux = (alturaMax - startY) * 45 / 400;
	}
	float jump = 90 / alturaAux;

	jumpAngle += ceil(jump);

	int alturaProv = int(startY - 400 * sin(3.14159f * jumpAngle / 180.f));
	//cout << "startY: " << startY << endl;
	//cout << "alturaProv: " << alturaProv << endl;
	//cout << "alturaMax: " << alturaMax << endl;

	if (alturaProv >= alturaMax) {
		//cout << "if" << endl;
		posBubble.y = alturaProv;
	}
	else {
		//cout << "else" << endl;
		if (jumpAngle < 90) {
			jumpAngle = 90;
		}
		posBubble.y = alturaMax;
	}
		
	if ((jumpAngle > 90) && map->bubbleCollisionMoveDown(posBubble, tamany, &posBubble.y)) {
		jumpAngle = 0;
		startY = posBubble.y;
	}
			

	if (direction == 0) posBubble.x += 4;
	else posBubble.x -= 4;

	if (map->bubbleCollisionMoveRight(posBubble, tamany, &posBubble.x)) {
		posBubble.x -= 4;
		direction = 1;
	}
	if (map->bubbleCollisionMoveLeft(posBubble, tamany, &posBubble.x)) {
		posBubble.x += 4;
		direction = 0;
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBubble.x), float(tileMapDispl.y + posBubble.y)));
}

void Bubble::render()
{
	sprite->render();
}

void Bubble::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Bubble::setPosition(const glm::vec2& pos)
{
	posBubble = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBubble.x), float(tileMapDispl.y + posBubble.y)));
}




