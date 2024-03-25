#ifndef _BUBBLE_INCLUDE
#define _BUBBLE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Bubble
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int type); //type = 1 -> gran, type = 4 -> petita
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

private:
	bool falling, movement;
	int direction; //0 left, 1 right
	glm::ivec2 tileMapDispl, posBubble;
	int jumpAngle, startY, alturaMax;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	glm::vec2 tamany;

};


#endif // _PLAYER_INCLUDE


