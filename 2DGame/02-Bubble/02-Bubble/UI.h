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

	void init(const int& level);
	void render();
	void update(int deltaTime);

private:
	void initShaders();

private:
	float currentTime, timeAccumulator;
	bool showInsertCoin;
	Quad* quad;
	Texture texs[2];
	TexturedQuad* texQuad[3];
	glm::mat4 projection;
	Texture scene, level1, level2, level3;
	int game_ui;

	ShaderProgram texProgram, simpleProgram;
	Text text;
};

#endif // _UI_INCLUDE