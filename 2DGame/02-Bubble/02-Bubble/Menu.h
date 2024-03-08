#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

//#include "ShaderProgramManager.h"
#include <vector>
#include <glm/glm.hpp>
#include "TexturedQuad.h"
#include "ShaderProgram.h"

#define SCREEN_WIDTH 1152
#define SCREEN_HEIGHT 624

class Menu {
public:
	Menu();
	~Menu();

	void init(const int& idScene);
	void render();
	void update(int deltaTime);

private:
	void initShaders();

private:
	float currentTime;
	TexturedQuad *background;
	glm::mat4 projection;
	Texture scene, menu, instructions;

	ShaderProgram texProgram;
};

#endif // _MENU_INCLUDE