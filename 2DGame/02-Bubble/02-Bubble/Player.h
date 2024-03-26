#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	int getPosX();
	int getPosY();
	int getSize();
	
private:
	bool falling, inStairs, activeExitStairs,left_orientation, right_orientation;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	int timerExitStairs;

	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

	void playerOrientation();

};


#endif // _PLAYER_INCLUDE


