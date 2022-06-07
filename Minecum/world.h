#pragma once
#include "chunk.h"

const unsigned int WORLD_SIZE_X = 3;
const unsigned int WORLD_SIZE_Z = 3;
const unsigned int WORLD_START_SIZE = WORLD_SIZE_X * WORLD_SIZE_Z;
const glm::vec3 START_POINT = glm::vec3(-6.0f, 0.0f, -6.0f);
const unsigned int DRAW_DISTANCE = 2;

const std::array<glm::vec3, 2> vec3e = { glm::vec3(1.0f, 0.0f, 0.0f) , glm::vec3(0.0f, 0.0f, 1.0f) };
const std::array<glm::vec3, 4> vec3a = { glm::vec3(CHUNK_SIZE_X * 1.0f, 0.0f, CHUNK_SIZE_Z * -1.0f), glm::vec3(CHUNK_SIZE_X * 1.0f, 0.0f, 0.0f),
										 glm::vec3(CHUNK_SIZE_X * 1.0f, 0.0f, CHUNK_SIZE_Z * 1.0f), glm::vec3(0.0f, 0.0f, CHUNK_SIZE_Z * 1.0f) };

class World
{
public:
	std::vector<Chunk*> chunk;
	std::vector<Chunk*> activeChunk;
	//std::array<Chunk*, (DRAW_DISTANCE * DRAW_DISTANCE + 1) * (DRAW_DISTANCE * DRAW_DISTANCE + 1)> activeChunk;
	World(glm::vec3 position = START_POINT);
	Chunk* getChunk(glm::vec3 position) const;
	void update(glm::vec3 position);
	~World();
private:
	void updateMeshes();
	void getChunksNeighboor(int i);
	void updateActivechunks(glm::vec3 position);
	bool lyingBetween(float coordinate, float delta, float firstBound, float secondBound) const;
};
