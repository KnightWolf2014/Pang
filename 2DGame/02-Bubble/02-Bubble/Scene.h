#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Levels.h"
#include "UI.h"
#include "Bubble.h"
#include "Text.h"
#include "SoundProgram.h"
#include <irrKlang.h>
using namespace irrklang;



// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init(const int& level, const int& lives, bool& godMode);
	void update(int deltaTime, bool& godMode);
	void render();
	bool gameOver();
	
private:
	void initShaders();
	void collisionBubblePlayer();

private:
	TileMap *map;
	Player* player;
	Levels *levels;
	Bubble *bubble;
	UI *ui;
	ShaderProgram texProgram;
	float currentTime;
	int hp;
	glm::mat4 projection;
	ISoundEngine* engine;
	Text text;

	int posPlayerX, posPlayerY, posBubbleX, posBubbleY;
	int sizePlayer, sizeBubble;
	int timerHitbox;
	bool activeHitbox;
	bool god;

};


#endif // _SCENE_INCLUDE

