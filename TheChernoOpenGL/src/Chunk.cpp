#include "Chunk.h"
#include "Texture.h"
#include <iostream>
#include "ErrorManager.h"
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
	std::cout << "Created Chunk: " << position.x << "," << position.y << "," << position.z << std::endl;
	m_Blocks = new BlockType[WIDTH * DEPTH * HEIGHT];
	for (unsigned int y = 0; y < Chunk::HEIGHT; y++) {
		for (unsigned int z = 0; z < Chunk::DEPTH; z++) {
			for (unsigned int x = 0; x < Chunk::WIDTH; x++) {
				unsigned int index = PositionToIndex(x, y, z);
				m_Blocks[index] = GetBlock(x, y, z);
			}
		}
	}
}

BlockType Chunk::GetBlock(int x, int y, int z) const
{
	int surfaceY = 100;
	if (y < surfaceY) {
		return BlockType::Stone;
	}
	else {
		return BlockType::Air;
	}
}

Chunk::~Chunk()
{
	std::cout << "Deleted Chunk: " << m_Position.x << "," << m_Position.y << "," << m_Position.z << std::endl;
	delete[] m_Blocks;
}

std::vector<Vertex> Chunk::GetChunkBlocksVertecies(const Chunk& leftChunk, const Chunk& rightChunk, const Chunk& frontChunk, const Chunk& backChunk) const
{
	std::vector<Vertex> vertices;
	size_t verticesOffset = 0;
	size_t indicesOffset = 0;
	for (unsigned int y = 0; y < Chunk::HEIGHT; y++) {
		for (unsigned int z = 0; z < Chunk::DEPTH; z++) {
			for (unsigned int x = 0; x < Chunk::WIDTH; x++) {
				unsigned int index = PositionToIndex(x, y, z);
				if (m_Blocks[index] == BlockType::Air) continue;
				bool neighbours[6] = { false };
				int neighbourIndex = 0;
				neighbourIndex = PositionToIndex(x, y, z+1);
				neighbours[(int)BlockSides::FRONT] = z<Chunk::DEPTH-1 && m_Blocks[neighbourIndex] != BlockType::Air;
				neighbourIndex = PositionToIndex(x+1, y, z);
				neighbours[(int)BlockSides::RIGHT] = x<Chunk::WIDTH-1 &&  m_Blocks[neighbourIndex] != BlockType::Air;
				neighbourIndex = PositionToIndex(x, y, z-1);
				neighbours[(int)BlockSides::BACK] = z>0 &&  m_Blocks[neighbourIndex] != BlockType::Air;
				neighbourIndex = PositionToIndex(x-1, y, z);
				neighbours[(int)BlockSides::LEFT] = x>0 &&  m_Blocks[neighbourIndex] != BlockType::Air;
				neighbourIndex = PositionToIndex(x, y+1, z);
				neighbours[(int)BlockSides::TOP] = y<Chunk::HEIGHT-1 &&  m_Blocks[neighbourIndex] != BlockType::Air;
				neighbourIndex = PositionToIndex(x, y-1, z);
				neighbours[(int)BlockSides::BOTTOM] = y>0 &&  m_Blocks[neighbourIndex] != BlockType::Air;
				glm::vec3 position = glm::vec3{x,y,z}+m_Position;
				std::vector<Vertex> blockVertices = GenerateBlockVertices(position, m_Blocks[index], neighbours);
				vertices.insert(vertices.begin(), blockVertices.begin(), blockVertices.end());
			}
		}
	}
	return vertices;
}


std::vector<Vertex> Chunk::GenerateBlockVertices(const glm::vec3& position, const BlockType& type, const bool* neighbours)const
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
	case BlockType::Dirt:
		frontTexCoords = { Texture::AtlasBlockX(2),Texture::AtlasBlockY(0) };
		rightTexCoords = { Texture::AtlasBlockX(2),Texture::AtlasBlockY(0) };
		backTexCoords = { Texture::AtlasBlockX(2),Texture::AtlasBlockY(0) };
		leftTexCoords = { Texture::AtlasBlockX(2),Texture::AtlasBlockY(0) };
		topTexCoords = { Texture::AtlasBlockX(2),Texture::AtlasBlockY(0) };
		bottomTexCoords = { Texture::AtlasBlockX(2),Texture::AtlasBlockY(0) };
		break;
	case BlockType::Grass:
		frontTexCoords = { Texture::AtlasBlockX(3),Texture::AtlasBlockY(0) };
		rightTexCoords = { Texture::AtlasBlockX(3),Texture::AtlasBlockY(0) };
		backTexCoords = { Texture::AtlasBlockX(3),Texture::AtlasBlockY(0) };
		leftTexCoords = { Texture::AtlasBlockX(3),Texture::AtlasBlockY(0) };
		topTexCoords = { Texture::AtlasBlockX(0),Texture::AtlasBlockY(0) };
		bottomTexCoords = { Texture::AtlasBlockX(2),Texture::AtlasBlockY(0) };
		break;
	case BlockType::Stone:
		frontTexCoords = { Texture::AtlasBlockX(1),Texture::AtlasBlockY(0) };
		rightTexCoords = { Texture::AtlasBlockX(1),Texture::AtlasBlockY(0) };
		backTexCoords = { Texture::AtlasBlockX(1),Texture::AtlasBlockY(0) };
		leftTexCoords = { Texture::AtlasBlockX(1),Texture::AtlasBlockY(0) };
		topTexCoords = { Texture::AtlasBlockX(1),Texture::AtlasBlockY(0) };
		bottomTexCoords = { Texture::AtlasBlockX(1),Texture::AtlasBlockY(0) };
		break;
	default:
		ASSERT(false);
	}
	glm::vec2 rightDown = { Texture::m_Offset,0.0 };
	glm::vec2 rightUp = { Texture::m_Offset,Texture::m_Offset };
	glm::vec2 leftUp = { 0.0,Texture::m_Offset };
	glm::vec2 leftDown = { 0.0,0.0 };
	//FRONT
	if(!neighbours[(int)BlockSides::FRONT]){
		//m_Indices.insert(m_Indices.end(), { verticesOffset,verticesOffset + 1,verticesOffset + 2,verticesOffset,verticesOffset + 2,verticesOffset + 3 });
		vertices.push_back({ position + rightDownFront,frontTexCoords + rightDown });
		vertices.push_back({ position + leftDownFront,	frontTexCoords + leftDown });
		vertices.push_back({ position + leftUpFront,	frontTexCoords + leftUp });
		vertices.push_back({ position + rightUpFront,	frontTexCoords + rightUp });
	}
	//RIGHT
	if (!neighbours[(int)BlockSides::RIGHT]) {
		vertices.push_back({ position + rightDownBack,	rightTexCoords + rightDown });
		vertices.push_back({ position + rightDownFront,rightTexCoords + leftDown });
		vertices.push_back({ position + rightUpFront,	rightTexCoords + leftUp });
		vertices.push_back({ position + rightUpBack,	rightTexCoords + rightUp });
	}
	//BACK
	if(!neighbours[(int)BlockSides::BACK]){
		vertices.push_back({ position + leftDownBack,	backTexCoords + rightDown });
		vertices.push_back({ position + rightDownBack,backTexCoords + leftDown });
		vertices.push_back({ position + rightUpBack,	backTexCoords + leftUp });
		vertices.push_back({ position + leftUpBack,	backTexCoords + rightUp });
	}
	//LEFT
	if(!neighbours[(int)BlockSides::LEFT]){
		vertices.push_back({ position + leftDownFront,leftTexCoords + rightDown });
		vertices.push_back({ position + leftDownBack,	leftTexCoords + leftDown });
		vertices.push_back({ position + leftUpBack,	leftTexCoords + leftUp });
		vertices.push_back({ position + leftUpFront,	leftTexCoords + rightUp });
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
