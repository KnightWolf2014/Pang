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
#include "Menu.h"
#include "SoundProgram.h"
#include "Fruit.h"
#include <irrKlang.h>
using namespace irrklang;



// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init(const int& level, const int& lives, bool& godMode, int points);
	void update(int deltaTime, bool& godMode);
	void render();
	bool gameOver();
	bool gameFinished();
	void updateTileMap(TileMap* mapV);
	void burst();
	void stop();
	int getScore();
	
private:
	void initShaders();
	void collisionBubblePlayer();
	void collisionBubbleHook();
	void collisionFruitPlayer();
	void divideBubble(Bubble* bubble, int type, int index);
	void timerOut();
	void deleteExteriorBubbles();

private:
	TileMap *map;
	Player* player;
	Levels *levels;
	Menu* menu;
	//Fruit* fruit;
	//Bubble *bubble;
	UI *ui;
	ShaderProgram texProgram;
	float currentTime;
	int hp, lvl;
	glm::mat4 projection;
	ISoundEngine* engine;
	Text text;

	vector<Bubble*> bubbles;
	vector<Fruit*> fruits;

	int posPlayerX, posPlayerY, posBubbleX, posBubbleY, posHookX, posHookY, posFruitX, posFruitY;
	int sizePlayer, sizeBubble, sizeFruit;
	int timerHitbox, timerTime, timerScore, timerStop;
	int typeFruit, posXfruita, posYfruita;
	bool activeHitbox, activeTime, activeScore, activeStop;
	bool god;
	bool finish;
	bool mostrarPoints, mostrarFruita;

	int viewType;
	int totalPoints, lastBubble, actualPoints, contLastBubble, posXpoints, posYpoints;
};


#endif // _SCENE_INCLUDE

