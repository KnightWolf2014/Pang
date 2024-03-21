#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Bubble.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum BubbleAnims
{
	FASEONE, FASETWO, FASETHREE,FASEFOUR
};


void Bubble::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	falling = false;
	spritesheet.loadFromFile("images/BubbleRock.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(128, 128), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(FASEONE, 16);
	sprite->addKeyframe(FASEONE, glm::vec2(0.0f, 0.f));


	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + 3 * posBubble.x), float(tileMapDispl.y + 3 * posBubble.y)));

}

void Bubble::update(int deltaTime)
{
	sprite->update(deltaTime);

	if (falling)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle == 180)
		{
			falling = false;
			posBubble.y = startY;
		}
		else
		{
			posBubble.y = int(startY - 400 * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90)
				falling = !map->bubbleCollisionMoveDown(posBubble, glm::ivec2(128, 128), &posBubble.y);
		}
	}
	else
	{
		posBubble.y += FALL_STEP;
		if (map->bubbleCollisionMoveDown(posBubble, glm::ivec2(128, 128), &posBubble.y))
		{
			falling = true;
			jumpAngle = 0;
			startY = posBubble.y;
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




