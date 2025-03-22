#include "Chunk.h"
#include "Texture.h"
#include <iostream>
#include "ErrorManager.h"
#include <chrono>

const float Chunk::m_Continentalness = 0.005f;
const siv::PerlinNoise Chunk::perlin{ Chunk::SEED };
enum class BlockSides {
	FRONT=0,
	RIGHT,
	BACK,
	LEFT,
	TOP,
	BOTTOM
};
Chunk::Chunk(): Chunk(glm::vec3{0.0,0.0,0.0})
{
}

Chunk::Chunk(glm::vec3 position): m_Position(position)
{
	auto start = std::chrono::high_resolution_clock::now();
	for (unsigned int y = 0; y < Chunk::HEIGHT; y++) {
		for (unsigned int z = 0; z < Chunk::DEPTH; z++) {
			for (unsigned int x = 0; x < Chunk::WIDTH; x++) {
				unsigned int index = PositionToIndex(x, y, z);
				m_Blocks[index] = GetBlock(position.x+x, position.y+y, position.z+z);
			}
		}
	}
	auto endChunk = std::chrono::high_resolution_clock::now();
	//std::cout << "CREATE CHUNK: " << std::chrono::duration_cast<std::chrono::milliseconds>(endChunk - start).count() << " miliseconds" << std::endl;
}

double Chunk::GetContinentalness(float x, float z)
{
	//return perlin.normalizedOctave2D(x*m_Continentalness, z*m_Continentalness,4);
	return perlin.octave2D_11(x*m_Continentalness, z*m_Continentalness,4);
}

bool Chunk::IsNeighbourDifferentAndSolid(int_fast8_t currentBlockType, int_fast8_t neighbourType)
{
	if (
		currentBlockType != (int_fast8_t)BlockType::Water &&
		neighbourType == (int_fast8_t)BlockType::Water
		) return false;

	return neighbourType != (int_fast8_t)BlockType::Air;
	//return currentBlockType == neighbourType || (
	//		neighbourType != (int_fast8_t) BlockType::Air &&
	//		neighbourType != (int_fast8_t) BlockType::Water
	//	)
	//	;
}
static int MapValues(double x, double x1, double x2, double y1, double y2, double defaultValue) {
	if (x >= x1 && x < x2)
		return y1 + ((x - x1) * (y2 - y1)) / (x2 - x1);
	else
		return defaultValue;
}
int_fast8_t Chunk::GetBlock(int x, int y, int z) const
{
	int surfaceY = 100;
	if(y>surfaceY+50)
		return (int_fast8_t)BlockType::Air;
	double continentalness = GetContinentalness(x,z);
	float xStops[] = {
		-1.0f,
		-0.35f,
		-0.30f,
		0.20f,
		1.0f
	};
	float yStops[] = {
		50,
		70,
		85,
		90,
		150
	};
	unsigned int stopsCount = sizeof(xStops) / sizeof(float);
	for (int i = 0; i < stopsCount; i++) {
		surfaceY = MapValues(continentalness, xStops[i], xStops[i+1], yStops[i], yStops[i+1],surfaceY);
	}

	int waterY = 85;
	if (y <= surfaceY) {
		if(y==surfaceY) return (int_fast8_t)BlockType::Grass;
		return (int_fast8_t)BlockType::Dirt;
	}
	else {
		if (y < waterY) {
			return (int_fast8_t)BlockType::Water;
		}
		else {
			return (int_fast8_t)BlockType::Air;
		}
	}
}

Chunk::~Chunk()
{
	//std::cout << "Deleted Chunk: " << m_Position.x << "," << m_Position.y << "," << m_Position.z << std::endl;
}

void Chunk::GetChunkBlocksVertecies(std::vector<Vertex>& terrainVertices, std::vector<Vertex>& waterVertices, const Chunk* rightChunk, const Chunk* leftChunk, const Chunk* frontChunk, const Chunk* backChunk) const
{
	auto start = std::chrono::high_resolution_clock::now();
	size_t verticesOffset = 0;
	size_t indicesOffset = 0;
	for (unsigned int y = 0; y < Chunk::HEIGHT; y++) {
		for (unsigned int z = 0; z < Chunk::DEPTH; z++) {
			for (unsigned int x = 0; x < Chunk::WIDTH; x++) {
				unsigned int index = PositionToIndex(x, y, z);
				if (m_Blocks[index] == (int_fast8_t)BlockType::Air) continue;
				bool neighbours[6] = { false };
				int neighbourIndex = 0;
				neighbourIndex = PositionToIndex(x, y, z+1);
				neighbours[(int)BlockSides::FRONT] = z<Chunk::DEPTH-1 && IsNeighbourDifferentAndSolid(m_Blocks[index],m_Blocks[neighbourIndex]);
				neighbours[(int)BlockSides::FRONT] = neighbours[(int)BlockSides::FRONT] || (frontChunk && z==Chunk::DEPTH-1 && IsNeighbourDifferentAndSolid(m_Blocks[index],frontChunk->m_Blocks[PositionToIndex(x,y,0)]));
				neighbourIndex = PositionToIndex(x+1, y, z);
				neighbours[(int)BlockSides::RIGHT] = x<Chunk::WIDTH-1 && IsNeighbourDifferentAndSolid(m_Blocks[index],m_Blocks[neighbourIndex]);
				neighbours[(int)BlockSides::RIGHT] = neighbours[(int)BlockSides::RIGHT] || (rightChunk && x==Chunk::WIDTH-1 && IsNeighbourDifferentAndSolid(m_Blocks[index],rightChunk->m_Blocks[PositionToIndex(0,y,z)]));
				neighbourIndex = PositionToIndex(x, y, z-1);
				neighbours[(int)BlockSides::BACK] = z>0 && IsNeighbourDifferentAndSolid(m_Blocks[index],m_Blocks[neighbourIndex]);
				neighbours[(int)BlockSides::BACK] = neighbours[(int)BlockSides::BACK] || (backChunk && z==0 && IsNeighbourDifferentAndSolid(m_Blocks[index],backChunk->m_Blocks[PositionToIndex(x,y,Chunk::DEPTH-1)]));
				neighbourIndex = PositionToIndex(x-1, y, z);
				neighbours[(int)BlockSides::LEFT] = x>0 && IsNeighbourDifferentAndSolid(m_Blocks[index],m_Blocks[neighbourIndex]);
				neighbours[(int)BlockSides::LEFT] = neighbours[(int)BlockSides::LEFT] || (leftChunk && x==0 && IsNeighbourDifferentAndSolid(m_Blocks[index],leftChunk->m_Blocks[PositionToIndex(Chunk::WIDTH-1,y,z)]));
				neighbourIndex = PositionToIndex(x, y+1, z);
				neighbours[(int)BlockSides::TOP] = y<Chunk::HEIGHT-1 && IsNeighbourDifferentAndSolid(m_Blocks[index],m_Blocks[neighbourIndex]);
				neighbourIndex = PositionToIndex(x, y-1, z);
				neighbours[(int)BlockSides::BOTTOM] = y>0 && IsNeighbourDifferentAndSolid(m_Blocks[index],m_Blocks[neighbourIndex]);
				glm::vec3 position = glm::vec3{x,y,z}+m_Position;

				auto start = std::chrono::high_resolution_clock::now();
				int sum = std::accumulate(neighbours, neighbours + 6, 0);
				if (sum == 6)
					continue;
				std::vector<Vertex> blockVertices = GenerateBlockVertices(position, m_Blocks[index], neighbours);
				auto end = std::chrono::high_resolution_clock::now();
				//std::cout << "GENERATE BLOCK: " << std::chrono::duration_cast<std::chrono::microseconds>(end- start).count() << " microseconds" << std::endl;
				if (m_Blocks[index] == (int_fast8_t)BlockType::Water) {
					waterVertices.insert(waterVertices.begin(), blockVertices.begin(), blockVertices.end());
				}
				else {
					terrainVertices.insert(terrainVertices.begin(), blockVertices.begin(), blockVertices.end());
				}
			}
		}
	}
	auto endChunk = std::chrono::high_resolution_clock::now();
	//std::cout << "GENERATE CHUNK: " << std::chrono::duration_cast<std::chrono::milliseconds>(endChunk - start).count() << " miliseconds" << std::endl;
}


std::vector<Vertex> Chunk::GenerateBlockVertices(const glm::vec3& position, int_fast8_t type, const bool* neighbours)const
{
	std::vector<Vertex> vertices;
	glm::vec3 leftDownBack = { 0.0,0.0,0.0 };
	glm::vec3 rightDownBack = { 1.0,0.0,0.0 };
	glm::vec3 rightUpBack = { 1.0,1.0,0.0 };
	glm::vec3 leftUpBack = { 0.0,1.0,0.0 };

	glm::vec3 leftDownFront = { 0.0,0.0,1.0 };
	glm::vec3 rightDownFront = { 1.0,0.0,1.0 };
	glm::vec3 rightUpFront = { 1.0,1.0,1.0 };
	glm::vec3 leftUpFront = { 0.0,1.0,1.0 };

	glm::vec2 frontTexCoords;
	glm::vec2 rightTexCoords;
	glm::vec2 backTexCoords;
	glm::vec2 leftTexCoords;
	glm::vec2 topTexCoords;
	glm::vec2 bottomTexCoords;
	switch (type) {
	case (int_fast8_t)BlockType::Dirt:
		frontTexCoords = { Texture::AtlasBlockX(2),Texture::AtlasBlockY(0) };
		rightTexCoords = { Texture::AtlasBlockX(2),Texture::AtlasBlockY(0) };
		backTexCoords = { Texture::AtlasBlockX(2),Texture::AtlasBlockY(0) };
		leftTexCoords = { Texture::AtlasBlockX(2),Texture::AtlasBlockY(0) };
		topTexCoords = { Texture::AtlasBlockX(2),Texture::AtlasBlockY(0) };
		bottomTexCoords = { Texture::AtlasBlockX(2),Texture::AtlasBlockY(0) };
		break;
	case (int_fast8_t)BlockType::Grass:
		frontTexCoords = { Texture::AtlasBlockX(3),Texture::AtlasBlockY(0) };
		rightTexCoords = { Texture::AtlasBlockX(3),Texture::AtlasBlockY(0) };
		backTexCoords = { Texture::AtlasBlockX(3),Texture::AtlasBlockY(0) };
		leftTexCoords = { Texture::AtlasBlockX(3),Texture::AtlasBlockY(0) };
		topTexCoords = { Texture::AtlasBlockX(0),Texture::AtlasBlockY(0) };
		bottomTexCoords = { Texture::AtlasBlockX(2),Texture::AtlasBlockY(0) };
		break;
	case (int_fast8_t)BlockType::Stone:
		frontTexCoords = { Texture::AtlasBlockX(1),Texture::AtlasBlockY(0) };
		rightTexCoords = { Texture::AtlasBlockX(1),Texture::AtlasBlockY(0) };
		backTexCoords = { Texture::AtlasBlockX(1),Texture::AtlasBlockY(0) };
		leftTexCoords = { Texture::AtlasBlockX(1),Texture::AtlasBlockY(0) };
		topTexCoords = { Texture::AtlasBlockX(1),Texture::AtlasBlockY(0) };
		bottomTexCoords = { Texture::AtlasBlockX(1),Texture::AtlasBlockY(0) };
		break;
	case (int_fast8_t)BlockType::Water:
		frontTexCoords = { Texture::AtlasBlockX(13),Texture::AtlasBlockY(12) };
		rightTexCoords = { Texture::AtlasBlockX(13),Texture::AtlasBlockY(12) };
		backTexCoords = { Texture::AtlasBlockX(13),Texture::AtlasBlockY(12) };
		leftTexCoords = { Texture::AtlasBlockX(13),Texture::AtlasBlockY(12) };
		topTexCoords = { Texture::AtlasBlockX(13),Texture::AtlasBlockY(12) };
		bottomTexCoords = { Texture::AtlasBlockX(13),Texture::AtlasBlockY(12) };
		break;
	default:
		ASSERT(false);
	}
	glm::vec2 rightDown = { Texture::m_Offset,0.0 };
	glm::vec2 rightUp = { Texture::m_Offset,Texture::m_Offset };
	glm::vec2 leftUp = { 0.0,Texture::m_Offset };
	glm::vec2 leftDown = { 0.0,0.0 };
	glm::vec3 frontBackColor = glm::vec3{0.80,0.80,0.80};
	glm::vec3 leftRightColor = glm::vec3{0.60,0.60,0.60};
	//FRONT
	if(!neighbours[(int)BlockSides::FRONT]){
		//m_Indices.insert(m_Indices.end(), { verticesOffset,verticesOffset + 1,verticesOffset + 2,verticesOffset,verticesOffset + 2,verticesOffset + 3 });
		vertices.push_back({ position + rightDownFront,frontTexCoords + rightDown,frontBackColor });
		vertices.push_back({ position + leftDownFront,	frontTexCoords + leftDown,frontBackColor });
		vertices.push_back({ position + leftUpFront,	frontTexCoords + leftUp,frontBackColor });
		vertices.push_back({ position + rightUpFront,	frontTexCoords + rightUp,frontBackColor });
	}
	//RIGHT
	if (!neighbours[(int)BlockSides::RIGHT]) {
		vertices.push_back({ position + rightDownBack,	rightTexCoords + rightDown,leftRightColor });
		vertices.push_back({ position + rightDownFront,rightTexCoords + leftDown,leftRightColor });
		vertices.push_back({ position + rightUpFront,	rightTexCoords + leftUp,leftRightColor });
		vertices.push_back({ position + rightUpBack,	rightTexCoords + rightUp,leftRightColor });
	}
	//BACK
	if(!neighbours[(int)BlockSides::BACK]){
		vertices.push_back({ position + leftDownBack,	backTexCoords + rightDown,frontBackColor });
		vertices.push_back({ position + rightDownBack,backTexCoords + leftDown,frontBackColor });
		vertices.push_back({ position + rightUpBack,	backTexCoords + leftUp,frontBackColor });
		vertices.push_back({ position + leftUpBack,	backTexCoords + rightUp,frontBackColor });
	}
	//LEFT
	if(!neighbours[(int)BlockSides::LEFT]){
		vertices.push_back({ position + leftDownFront,leftTexCoords + rightDown,leftRightColor });
		vertices.push_back({ position + leftDownBack,	leftTexCoords + leftDown,leftRightColor });
		vertices.push_back({ position + leftUpBack,	leftTexCoords + leftUp,leftRightColor });
		vertices.push_back({ position + leftUpFront,	leftTexCoords + rightUp,leftRightColor });
	}
	//TOP
	if(!neighbours[(int)BlockSides::TOP]){
		vertices.push_back({ position + rightUpFront,	topTexCoords + rightDown });
		vertices.push_back({ position + leftUpFront,	topTexCoords + leftDown });
		vertices.push_back({ position + leftUpBack,	topTexCoords + leftUp });
		vertices.push_back({ position + rightUpBack,	topTexCoords + rightUp });
	}
	//BOTTOM
	if(!neighbours[(int)BlockSides::BOTTOM]){
		vertices.push_back({ position + leftDownFront,	bottomTexCoords + leftUp });
		vertices.push_back({ position + rightDownFront,bottomTexCoords + rightUp });
		vertices.push_back({ position + rightDownBack,	bottomTexCoords + rightDown });
		vertices.push_back({ position + leftDownBack,	bottomTexCoords + leftDown });
	}
	return vertices;
}
