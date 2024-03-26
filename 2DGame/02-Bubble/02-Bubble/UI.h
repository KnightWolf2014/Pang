#ifndef _UI_INCLUDE
#define _UI_INCLUDE

#include <vector>
#include <glm/glm.hpp>
#include "TexturedQuad.h"
#include "ShaderProgram.h"
#include "Text.h"
#include "Quad.h"

#define CAMERA_WIDTH 1152
#define CAMERA_HEIGHT 740

#define SCREEN_WIDTH 1152
#define SCREEN_HEIGHT 624


class UI {
public:
	UI();
	~UI();

	void init(const int& level, const int& lives, const bool& godMode);
	void render();
	void update(int deltaTime, int& lives, bool& godMode);
	int getCountDown();

private:
	void initShaders();

private:
	float currentTime, timeAccumulatorCoin, timeAccumulatorTimer;
	bool showInsertCoin, god;
	Quad* quad;
	Texture texs[2];
	TexturedQuad* texQuad[3];
	glm::mat4 projection;
	Texture scene, level1, level2, level3;
	int game_ui, timer, timerInverse, hp, time;
	std::string timeScreen;

	ShaderProgram texProgram, simpleProgram;
	Text text;
};

#endif // _UI_INCLUDE