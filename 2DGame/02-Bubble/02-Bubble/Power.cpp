#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Power.h"
#include "Game.h"

#define FALL_STEP 4

#define SIZE_POWER_X 38
#define SIZE_POWER_y 38

enum PowerAnim {
	FALLING
};

void Power::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int powerType) {
	falling = true;

	texProgram = shaderProgram;

	type = powerType;

	cout << "typeFruit: " << powerType << endl;


	spritesheet.loadFromFile("images/playerSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(glm::ivec2(SIZE_POWER_X, SIZE_POWER_y), glm::vec2(0.2, 0.2), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	//if (powerType == 0) {
	sprite->setAnimationSpeed(FALLING, 16);
	sprite->addKeyframe(FALLING, glm::vec2(0.f, 0.f));
	/* }
	if (powerType == 1) {
		sprite->setAnimationSpeed(FALLING, 16);
		sprite->addKeyframe(FALLING, glm::vec2(0.33f, 0.f));
	}
	if (powerType == 2) {
		sprite->setAnimationSpeed(FALLING, 16);
		sprite->addKeyframe(FALLING, glm::vec2(0.66f, 0.f));
	}*/
	

	sprite->changeAnimation(FALLING);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + 3 * posPower.x), float(tileMapDispl.y + 3 * posPower.y)));


}

void Power::update(int deltaTime)
{

	sprite->update(deltaTime);

	//cout << posPower.x << " " << posPower.y << endl;

	if (falling) {
		posPower.y += 4;
		sprite->changeAnimation(FALLING);
	}

	if (map->collisionMoveDown(posPower, glm::ivec2(SIZE_POWER_X, SIZE_POWER_y), &posPower.y)) {
		falling = false;
	}
	else falling = true;


	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPower.x), float(tileMapDispl.y + posPower.y)));

}

void Power::render()
{
	sprite->render();
}

void Power::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Power::setPosition(const glm::vec2& pos)
{
	posPower = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPower.x), float(tileMapDispl.y + posPower.y)));
}

int Power::getPosX() {
	return posPower.x;
}

int Power::getPosY() {
	return posPower.y;
}

int Power::getSize() {
	return 38;
}

int Power::getType() {
	return type;
}
