#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Levels.h"
//#include <GL/glut.h>


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
	void renderText(const std::string& text, float x, float y, float size, const glm::vec3& color);

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	Levels *levels;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

};


#endif // _SCENE_INCLUDE

