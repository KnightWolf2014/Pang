#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include "TileMap.h"


using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);

	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;
	
	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	
	cout << mapSize.x << " " << mapSize.y << endl;
	map = new int[mapSize.x * mapSize.y];
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			fin >> map[j * mapSize.x + i];
			cout << map[j * mapSize.x + i] << " ";
		}
		cout << endl;
		fin.get(tile);
#ifndef _WIN32
		//fin.get(tile);
#endif
	}
	fin.close();
	
	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	nTiles = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile-1)%tilesheetSize.x) / tilesheetSize.x, float((tile-1)/tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if ((map[y * mapSize.x + x] != 0) && (map[y * mapSize.x + x] != 2) && (map[y * mapSize.x + x] != 3) && (map[y * mapSize.x + x] != 4))
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = (pos.x + size.x-1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++) {
		if ((map[y * mapSize.x + x] != 0) && (map[y * mapSize.x + x] != 2) && (map[y * mapSize.x + x] != 3) && (map[y * mapSize.x + x] != 4))
			return true;
	}
	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const {
	int x0, x1, y;
	
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for(int x=x0; x<=x1; x++)
	{
		if(map[y*mapSize.x+x] != 0 && *posY-tileSize*y+size.y <= 4) {
			*posY = tileSize * y - size.y;
			return true;
		}
	}
	
	return false;
}

bool TileMap::bubbleCollisionFloor(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const {
	int x0, x1, y0, y1;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;

	int yMedio = (y0 + y1) / 2;

	for (int y = yMedio; y <= y1; y++) {
		for (int x = x0; x <= x1; x++) {

			int dx = pos.x + size.x / 2 - (x * tileSize + tileSize / 2);
			int dy = pos.y + size.y / 2 - (y * tileSize + tileSize / 2);
			int distanceSquared = dx * dx + dy * dy;

			int radiusSquared = (size.x / 2) * (size.x / 2);
			if (distanceSquared <= radiusSquared) {
				if (map[y * mapSize.x + x] != 1) {
					int collisionY = (y)*tileSize - size.y - 1;
					if (*posY + size.y > collisionY) {
						*posY = collisionY;
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool TileMap::bubbleCollisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const {
	int x0, x1, y0, y1;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;

	int yMedio = (y0 + y1) / 2;

	for (int y = yMedio; y <= y1; y++) {
		for (int x = x0; x <= x1; x++) {

			int dx = pos.x + size.x / 2 - (x * tileSize + tileSize / 2);
			int dy = pos.y + size.y / 2 - (y * tileSize + tileSize / 2);
			int distanceSquared = dx * dx + dy * dy;

			int radiusSquared = (size.x / 2) * (size.x / 2);
			if (distanceSquared <= radiusSquared) {
				if (map[y * mapSize.x + x] != 0 && map[y * mapSize.x + x] != 2 && map[y * mapSize.x + x] != 3 && map[y * mapSize.x + x] != 4) {
					int collisionY = (y) * tileSize - size.y - 1;
					if (*posY + size.y > collisionY) {
						*posY = collisionY;
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool TileMap::bubbleCollisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size, int* posX) const {
	int x0, x1, y0, y1;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;

	int xMedio = (x0 + x1) / 2;

	for (int x = xMedio; x <= x1; x++) {
		for (int y = y0; y <= y1; y++) {

			int dx = pos.x + size.x / 2 - (x * tileSize + tileSize / 2);
			int dy = pos.y + size.y / 2 - (y * tileSize + tileSize / 2);
			int distanceSquared = dx * dx + dy * dy; 

			int radiusSquared = (size.x / 2) * (size.x / 2);
			if (distanceSquared <= radiusSquared) {
				if (map[y * mapSize.x + x] != 0 && map[y * mapSize.x + x] != 2 && map[y * mapSize.x + x] != 3 && map[y * mapSize.x + x] != 4) {
					int collisionX = x * tileSize - size.x - 1;
					if (*posX + size.x > collisionX) {
						*posX = collisionX;
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool TileMap::bubbleCollisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const {
	int x0, x1, y0, y1;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;

	int yMedio = (y0 + y1) / 2;

	for (int y = yMedio; y >= y0; y--) {
		for (int x = x0; x <= x1; x++) {

			int dx = pos.x + size.x / 2 - (x * tileSize + tileSize / 2);
			int dy = pos.y + size.y / 2 - (y * tileSize + tileSize / 2);
			int distanceSquared = dx * dx + dy * dy;

			int radiusSquared = (size.x / 2) * (size.x / 2);
			if (distanceSquared <= radiusSquared) {
				if (map[y * mapSize.x + x] != 0 && map[y * mapSize.x + x] != 2 && map[y * mapSize.x + x] != 3 && map[y * mapSize.x + x] != 4) {
					int collisionY = (y + 1) * tileSize + 1;
					if (*posY < collisionY) {
						*posY = collisionY;
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool TileMap::bubbleCollisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size, int* posX) const {
	int x0, x1, y0, y1;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;

	int xMedio = (x0 + x1) / 2;

	for (int x = xMedio; x >= x0; x--) {
		for (int y = y0; y <= y1; y++) {

			int dx = pos.x + size.x / 2 - (x * tileSize + tileSize / 2);
			int dy = pos.y + size.y / 2 - (y * tileSize + tileSize / 2);
			int distanceSquared = dx * dx + dy * dy;

			int radiusSquared = (size.x / 2) * (size.x / 2);
			if (distanceSquared <= radiusSquared) {
				if (map[y * mapSize.x + x] != 0 && map[y * mapSize.x + x] != 2 && map[y * mapSize.x + x] != 3 && map[y * mapSize.x + x] != 4) {
					int collisionX = (x + 1) * tileSize + 1;
					if (*posX < collisionX) {
						*posX = collisionX;
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool TileMap::collisionStairs(const glm::ivec2& pos, const glm::ivec2& size, int& posX) const {

	int x0, x1, y0, y1;

	x0 = (pos.x) / tileSize -1;
	x1 = (pos.x + size.x) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y-1) / tileSize;

	for (int y = y0; y <= y1; y++) {
		for (int x = x0; x <= x1; ++x) {
			if (map[y * mapSize.x + x] == 3 || map[y * mapSize.x + x] == 14) {
				posX = x*tileSize-size.x/2;
				return true;
			}
		}
	}

	return false;
}

bool TileMap::exitUpStairs(const glm::ivec2& pos, const glm::ivec2& size) const {

	int x0, x1, y0, y1;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y/2-4) / tileSize;

	//Detectar que la meitat del cos esta fora
	for (int y = y0; y <= y1; y++) {
		for (int x = x0; x <= x1; ++x) {
			if (map[y * mapSize.x + x] != 0) return false;
		}
	}

	return true;
}

bool TileMap::exitDownStairs(const glm::ivec2& pos, const glm::ivec2& size) const {
	int x0, x1, y0, y1;
	bool colision, bodyOut;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y0 = (pos.y + size.y/2) / tileSize;
	y1 = (pos.y + size.y-1) / tileSize;

	colision = false;
	bodyOut = true;

	//Detectar si hi ha colisio per sota
	for (int x = x0; x <= x1; x++)
	{
		if (map[y1*mapSize.x+x] != 0 && map[y1*mapSize.x+x] != 2 && map[y1*mapSize.x+x] != 3 && map[y1*mapSize.x+x] != 4 && map[y1 * mapSize.x + x] != 13 && 
			map[y1 * mapSize.x + x] != 14 && map[y1 * mapSize.x + x] != 15 && map[y1 * mapSize.x + x] != 21 && map[y1*mapSize.x + x] != 22 && pos.y-tileSize*y1+size.y <= 4) {
			colision = true;
			break;
			
		}
	}

	//Detectar que la meitat del cos esta fora
	for (int y = y0; y <= y1; y++) {
		for (int x = x0; x <= x1; ++x) {
			if (map[y * mapSize.x + x] != 0) {
				bodyOut = false;
				break;
			}
		}
	}
	return colision || bodyOut;
}