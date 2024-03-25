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

	void init(const int& level);
	void update(int deltaTime);
	void render();
	
private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	Levels *levels;
	Bubble *bubble;
	UI *ui;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	ISoundEngine* engine;
	Text text;

};


#endif // _SCENE_INCLUDE

