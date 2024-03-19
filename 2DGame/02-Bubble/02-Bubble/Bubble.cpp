#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Bubble.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum BubbleAnim
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Bubble::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	falling = true;
	spritesheet.loadFromFile("images/playerSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(96, 96), glm::vec2(0.20, 0.20), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(STAND_LEFT, 16);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.6f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 16);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.6f, 0.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 16);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.2f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.4f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.6f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.8f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 16);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0., 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0., 0.2f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0., 0.4f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0., 0.6f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0., 0.8f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + 3 * posPlayer.x), float(tileMapDispl.y + 3 * posPlayer.y)));

}

/*void Bubble::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (Game::instance().getKey(GLFW_KEY_LEFT))
	{
		if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 8;
		if (map->collisionMoveLeft(posPlayer, glm::ivec2(96, 96)))
		{
			posPlayer.x += 8;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if (Game::instance().getKey(GLFW_KEY_RIGHT))
	{
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 8;
		if (map->collisionMoveRight(posPlayer, glm::ivec2(96, 96)))
		{
			posPlayer.x -= 8;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}

	//ARREGLAR ESCALERAS!!!
	else if (Game::instance().getKey(GLFW_KEY_UP)) {
		if (map->inStairs(posPlayer, glm::ivec2(96, 96))) {
			posPlayer.y -= 8;
			sprite->changeAnimation(MOVE_UP);
		}
		//posPlayer.y -= 8;
	}
	else if (Game::instance().getKey(GLFW_KEY_DOWN)) {
		if (map->inStairs(posPlayer, glm::ivec2(96, 96))) {
			posPlayer.y += 8;
			sprite->changeAnimation(MOVE_DOWN);
		}
	}


	else if (Game::instance().getKey(GLFW_KEY_SPACE))
	{
		if (sprite->animation() != SHOOTING)
			sprite->changeAnimation(SHOOTING);
		/*else {
			sprite->changeAnimation(STAND_RIGHT);
		}*/
	/*}

	else {
		if (sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}

	if (!falling)
	{
		if (!map->collisionMoveDown(posPlayer, glm::ivec2(96, 96), &posPlayer.y))
		{
			falling = true;
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if (map->collisionMoveDown(posPlayer, glm::ivec2(96, 96), &posPlayer.y))
		{
			falling = false;
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}*/

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
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




