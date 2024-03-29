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


void Bubble::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int type, int dir)
{
	falling = false;

	movement = true;
	mida = type;

	direction = dir;
	alturaMax = 16;
	jump = -4;
	gravetatMax = 8;

	energy = 0;
	contJump = 0;


	if (type == 1) tamany = glm::ivec2(128, 128);
	if (type == 2) tamany = glm::ivec2(112, 112);
	if (type == 3) tamany = glm::ivec2(96, 96);
	if (type == 4) tamany = glm::ivec2(80, 80);


	spritesheet.loadFromFile("images/Bubble.png", TEXTURE_PIXEL_FORMAT_RGBA);
	
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

int Bubble::getType() {
	return mida;
}

void Bubble::update(int deltaTime)
{
	sprite->update(deltaTime); 

	posBubble.y += jump;

	if (energy >= 0) {
		if (jump < 0) {
			jump = 0;
		}

		contJump += 2;

		if (contJump > 4) {
			contJump = 0;
			jump = (jump + 1 > gravetatMax) ? gravetatMax : jump + 1;
		}
	}
	else {
		int velParabola = energy / 4;
		jump = (velParabola < -gravetatMax) ? -gravetatMax : velParabola;
		jump = (velParabola > -1) ? -1 : jump;
		energy = alturaMax - posBubble.y;
	}


		if (map->bubbleCollisionMoveDown(posBubble, tamany, &posBubble.y))
		{
			while (map->bubbleCollisionMoveDown(posBubble, tamany, &posBubble.y))
				posBubble.y -= 1;
			jump = 0;
			energy = alturaMax - posBubble.y;
		}

		if (map->bubbleCollisionMoveUp(posBubble, tamany, &posBubble.y))
		{
			while (map->bubbleCollisionMoveUp(posBubble, tamany, &posBubble.y))
				posBubble.y += 1;
			jump = 0;
			energy = 0;
		}

		if (direction == 0) posBubble.x += 4;
		else posBubble.x -= 4;

		if (map->bubbleCollisionMoveRight(posBubble, tamany, &posBubble.x)) {
			while (map->bubbleCollisionMoveRight(posBubble, tamany, &posBubble.x))
				posBubble.x -= 1;
			direction = 1;
		}
		if (map->bubbleCollisionMoveLeft(posBubble, tamany, &posBubble.x)) {
			while (map->bubbleCollisionMoveLeft(posBubble, tamany, &posBubble.x))
				posBubble.x += 1;
			direction = 0;
		}



		if (map->bubbleCollisionMoveDown(posBubble, tamany, &posBubble.y) && map->bubbleCollisionMoveRight(posBubble, tamany, &posBubble.x)) {
			if (map->bubbleCollisionMoveDown(posBubble, tamany, &posBubble.y)) {
				while (map->bubbleCollisionMoveDown(posBubble, tamany, &posBubble.y))
					posBubble.y -= 1;
				jump = 0;
				energy = alturaMax - posBubble.y;

				if (map->bubbleCollisionMoveRight(posBubble, tamany, &posBubble.x)) {
					while (map->bubbleCollisionMoveRight(posBubble, tamany, &posBubble.x))
						posBubble.x -= 1;
					direction = 1;
				}
			}
		}

		if (map->bubbleCollisionMoveDown(posBubble, tamany, &posBubble.y) && map->bubbleCollisionMoveLeft(posBubble, tamany, &posBubble.x)) {
			if (map->bubbleCollisionMoveDown(posBubble, tamany, &posBubble.y)) {
				while (map->bubbleCollisionMoveDown(posBubble, tamany, &posBubble.y))
					posBubble.y -= 1;
				jump = 0;
				energy = alturaMax - posBubble.y;


				if (map->bubbleCollisionMoveLeft(posBubble, tamany, &posBubble.x)) {
					while (map->bubbleCollisionMoveLeft(posBubble, tamany, &posBubble.x))
						posBubble.x += 1;
					direction = 0;
				}
			}
		}

		if (map->bubbleCollisionMoveUp(posBubble, tamany, &posBubble.y) && map->bubbleCollisionMoveRight(posBubble, tamany, &posBubble.x)) {
			if (map->bubbleCollisionMoveUp(posBubble, tamany, &posBubble.y)) {
				while (map->bubbleCollisionMoveUp(posBubble, tamany, &posBubble.y))
					posBubble.y += 1;
				jump = 0;
				energy = 0;


				if (map->bubbleCollisionMoveRight(posBubble, tamany, &posBubble.x)) {
					while (map->bubbleCollisionMoveRight(posBubble, tamany, &posBubble.x))
						posBubble.x -= 1;
					direction = 1;
				}
			}
		}

		if (map->bubbleCollisionMoveUp(posBubble, tamany, &posBubble.y) && map->bubbleCollisionMoveLeft(posBubble, tamany, &posBubble.x)) {
			if (map->bubbleCollisionMoveUp(posBubble, tamany, &posBubble.y)) {
				while (map->bubbleCollisionMoveUp(posBubble, tamany, &posBubble.y))
					posBubble.y += 1;
				jump = 0;
				energy = 0;

				if (map->bubbleCollisionMoveLeft(posBubble, tamany, &posBubble.x)) {
					while (map->bubbleCollisionMoveLeft(posBubble, tamany, &posBubble.x))
						posBubble.x += 1;
					direction = 0;
				}
			}
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




