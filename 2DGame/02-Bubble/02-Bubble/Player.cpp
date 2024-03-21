#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"

#define FALL_STEP 4
#define TIME_EXIT_STAIRS 150

enum PlayerAnims {
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, SHOOTING_LEFT, SHOOTING_RIGHT, MOVE_UP, MOVE_DOWN, EXIT_STAIRS
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	falling = true;
	inStairs = false;
	activeExitStairs = false;
	timerExitStairs = TIME_EXIT_STAIRS;

	spritesheet.loadFromFile("images/playerSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(96, 96), glm::vec2(0.20, 0.20), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(9);
	
		sprite->setAnimationSpeed(STAND_LEFT, 16);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.6f, 0.f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 16);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.8f, 0.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 16);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.2f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.4f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.6f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.8f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 16);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0., 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0., 0.2f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0., 0.4f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0., 0.6f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0., 0.8f));

		sprite->setAnimationSpeed(SHOOTING_LEFT, 16);
		sprite->addKeyframe(SHOOTING_LEFT, glm::vec2(0.6f, 0.2f));

		sprite->setAnimationSpeed(SHOOTING_RIGHT, 16);
		sprite->addKeyframe(SHOOTING_RIGHT, glm::vec2(0.8f, 0.2f));

		sprite->setAnimationSpeed(MOVE_UP, 16);
		sprite->addKeyframe(MOVE_UP, glm::vec2(0.4f, 0.0f));
		sprite->addKeyframe(MOVE_UP, glm::vec2(0.4f, 0.2f));
		sprite->addKeyframe(MOVE_UP, glm::vec2(0.4f, 0.4f));

		sprite->setAnimationSpeed(MOVE_DOWN, 16);
		sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.4f, 0.4f));
		sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.4f, 0.2f));
		sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.4f, 0.0f));

		sprite->setAnimationSpeed(EXIT_STAIRS, 16);
		sprite->addKeyframe(EXIT_STAIRS, glm::vec2(0.4f, 0.6f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + 3*posPlayer.x), float(tileMapDispl.y + 3*posPlayer.y)));
}

void Player::update(int deltaTime)
{

	sprite->update(deltaTime);

	if (activeExitStairs) {
		if (timerExitStairs < 0) {
			timerExitStairs = TIME_EXIT_STAIRS;
			playerOrientation();
			activeExitStairs = false;
		}
		timerExitStairs -= deltaTime;
	}
	
	// DISPARAR
	if (Game::instance().getKey(GLFW_KEY_SPACE) && !inStairs && !activeExitStairs) {
		if (left_orientation && sprite->animation() != SHOOTING_LEFT)
			sprite->changeAnimation(SHOOTING_LEFT);
		else if (right_orientation && sprite->animation() != SHOOTING_RIGHT)
			sprite->changeAnimation(SHOOTING_RIGHT);

	// MOURE ESQUERRA
	} else if (Game::instance().getKey(GLFW_KEY_LEFT) && !inStairs && !activeExitStairs) {
		posPlayer.x -= 4;

		left_orientation = true;
		right_orientation = false;

		if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);

		if (map->collisionMoveLeft(posPlayer, glm::ivec2(96, 96))) {
			posPlayer.x += 4;
			sprite->changeAnimation(STAND_LEFT);
		}
		
	// MOURE DRETA
	} else if(Game::instance().getKey(GLFW_KEY_RIGHT) && !inStairs && !activeExitStairs) {
		posPlayer.x += 4;

		left_orientation = false;
		right_orientation = true;
		
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);

		if (map->collisionMoveRight(posPlayer, glm::ivec2(96, 96))) {
			posPlayer.x -= 4;
			sprite->changeAnimation(STAND_RIGHT);
		}

	// MOURE AMUNT
	} else if (Game::instance().getKey(GLFW_KEY_UP) && !activeExitStairs) {

		//Aixo ho fem per evitar moure el personatge abans de saber que fara
		glm::ivec2 posAux = posPlayer;
		int newPos;
		posAux.y -= 4;

		//Sortir d'escales
		if (inStairs && map->exitUpStairs(posAux, glm::ivec2(96, 96))) {
			inStairs = false;
			activeExitStairs = true;
			posPlayer.y = posPlayer.y - 96/2 -4;
			sprite->changeAnimation(EXIT_STAIRS);
		
		//Pujar escales
		} else if (map->collisionStairs(posAux, glm::ivec2(96, 96), newPos)) {
			inStairs = true;
			posPlayer.y -= 4; //Aquest -4 s'ha afegit perque es detecta 4 pixels abans quan ha de pujar escales. Es per evitar bug d'animacio!
			posPlayer.x = newPos; //Ajustar posicio del jugador a l'escala
			if (sprite->animation() != MOVE_UP) sprite->changeAnimation(MOVE_UP);
		}
		
	// MOURE A BAIX
	} else if (Game::instance().getKey(GLFW_KEY_DOWN) && !activeExitStairs) {

		//Aixo ho fem per evitar moure el personatge abans de saber que fara
		glm::ivec2 posAux = posPlayer;
		posAux.y += 4;
		
		int newPos;
		bool collisionPostMove = map->collisionStairs(posAux, glm::ivec2(96, 96), newPos);
		bool exitStairs = map->exitDownStairs(posAux, glm::ivec2(96, 96));

		//Pot baixar escales
		if (collisionPostMove && !exitStairs) {

			//Entra a les escales per amunt o segueix baixant
			if (!inStairs) posPlayer.y += (96/2);
			else posPlayer.y += 4;
			
			posPlayer.x = newPos; //Ajustar posicio del jugador a l'escala
			inStairs = true;
			if (sprite->animation() != MOVE_DOWN) sprite->changeAnimation(MOVE_DOWN);

		//No pot baixar escales o surt d'elles
		} else if (exitStairs){
			inStairs = false;
			activeExitStairs = true;
			playerOrientation();
		}


	} else if (!activeExitStairs) {
		if (!inStairs) playerOrientation();
		else sprite->update(-deltaTime);
	}
	
	//CODI DE CAIGUDA DEL PERSONATGE
	if (!falling && !inStairs && !map->collisionMoveDown(posPlayer, glm::ivec2(96, 96), &posPlayer.y)) {
			falling = true;	

	} else if (!inStairs){

		posPlayer.y += FALL_STEP;

		if(map->collisionMoveDown(posPlayer, glm::ivec2(96, 96), &posPlayer.y)) {
			falling = false;
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::playerOrientation() {
	if (left_orientation && sprite->animation() != STAND_LEFT) {
		sprite->changeAnimation(STAND_LEFT);
	
	} else if (right_orientation && sprite->animation() != STAND_RIGHT) {
		sprite->changeAnimation(STAND_RIGHT);
	}
}



