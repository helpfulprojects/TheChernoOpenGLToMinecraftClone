#include "Chunk.h"
#include "Texture.h"

Chunk::Chunk()
{
	for (unsigned int z = 0; z < Chunk::DEPTH; z++) {

		for (unsigned int y = 0; y < Chunk::HEIGHT; y++) {

			for (unsigned int x = 0; x < Chunk::WIDTH; x++) {
				unsigned int index = x + (y * Chunk::WIDTH) + (z * Chunk::WIDTH * Chunk::HEIGHT);
				m_Blocks[index] = BlockType::Dirt;
			}
		}
	}
}

Chunk::~Chunk()
{
	delete m_Va;
	delete m_Ib;
	delete m_Vb;
}

void Chunk::UpdateVertexArray()
{
	Vertex* vertices = new Vertex[Chunk::WIDTH * Chunk::HEIGHT * Chunk::DEPTH*24];
	unsigned int* indices = new unsigned int[Chunk::WIDTH * Chunk::HEIGHT * Chunk::DEPTH * 36];
	size_t verticesOffset = 0;
	size_t indicesOffset = 0;
	for (unsigned int z = 0; z < Chunk::DEPTH; z++) {

		for (unsigned int y = 0; y < Chunk::HEIGHT; y++) {

			for (unsigned int x = 0; x < Chunk::WIDTH; x++) {
				unsigned int index = x + (y * Chunk::WIDTH) + (z * Chunk::WIDTH * Chunk::HEIGHT);
				// TODO: if air don't generate verts
				glm::vec3 position = {x,y,z};
				std::array<Vertex, 24> blockVertices = Chunk::GenerateBlockVerts(position, m_Blocks[index]);
				unsigned int blockIndices[36] = {
					verticesOffset,verticesOffset + 1,verticesOffset + 2,verticesOffset,verticesOffset + 2,verticesOffset + 3,
					verticesOffset +4, verticesOffset +5, verticesOffset +6, verticesOffset +4, verticesOffset +6, verticesOffset +7,
					verticesOffset +8, verticesOffset +9, verticesOffset +10, verticesOffset +8, verticesOffset +10, verticesOffset +11,
					verticesOffset +12, verticesOffset +13, verticesOffset +14, verticesOffset +12, verticesOffset +14, verticesOffset +15,
					verticesOffset +16, verticesOffset +17,verticesOffset + 18,verticesOffset + 16, verticesOffset +18, verticesOffset +19,
					verticesOffset +20, verticesOffset +21, verticesOffset +22, verticesOffset +20, verticesOffset +22, verticesOffset +23,
				};
				memcpy(vertices+verticesOffset, blockVertices.data(), blockVertices.size() * sizeof(Vertex));
				memcpy(indices+indicesOffset, blockIndices, 36 * sizeof(unsigned int));
				verticesOffset += blockVertices.size();
				indicesOffset += 36;
			}
		}
	}
	m_Va = new VertexArray();
	m_Va->Bind();
	m_Vb = new VertexBuffer(vertices, Chunk::WIDTH*Chunk::HEIGHT*Chunk::DEPTH*24);
	m_Vb->Bind();
	m_Va->AddBlockBuffer(*m_Vb);

	m_Ib = new IndexBuffer(indices, Chunk::WIDTH*Chunk::HEIGHT*Chunk::DEPTH*36);
	m_Ib->Bind();
	delete[] vertices;
	delete[] indices;
}

std::array<Vertex, 24> Chunk::GenerateBlockVerts(glm::vec3 position, BlockType type)
{
	std::array<Vertex, 24> vertices;
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
		frontTexCoords = { Texture::AtlasBlockX(1),Texture::AtlasBlockY(1) };
		rightTexCoords = { Texture::AtlasBlockX(1),Texture::AtlasBlockY(1) };
		backTexCoords = { Texture::AtlasBlockX(1),Texture::AtlasBlockY(1) };
		leftTexCoords = { Texture::AtlasBlockX(1),Texture::AtlasBlockY(1) };
		topTexCoords = { Texture::AtlasBlockX(1),Texture::AtlasBlockY(1) };
		bottomTexCoords = { Texture::AtlasBlockX(1),Texture::AtlasBlockY(1) };
	}
	glm::vec2 rightDown = { Texture::m_Offset,0.0 };
	glm::vec2 rightUp = { Texture::m_Offset,Texture::m_Offset };
	glm::vec2 leftUp = { 0.0,Texture::m_Offset };
	glm::vec2 leftDown = { 0.0,0.0 };
	//FRONT
	vertices[0] = { position + leftDownFront,	frontTexCoords + leftDown };
	vertices[1] = { position + rightDownFront,frontTexCoords + rightDown };
	vertices[2] = { position + rightUpFront,	frontTexCoords + rightUp };
	vertices[3] = { position + leftUpFront,	frontTexCoords + leftUp };
	//RIGHT
	vertices[4] = { position + rightDownFront,rightTexCoords + leftDown };
	vertices[5] = { position + rightDownBack,	rightTexCoords + rightDown };
	vertices[6] = { position + rightUpBack,	rightTexCoords + rightUp };
	vertices[7] = { position + rightUpFront,	rightTexCoords + leftUp };
	//BACK
	vertices[8] = { position + leftDownBack,	backTexCoords + rightDown };
	vertices[9] = { position + rightDownBack,backTexCoords + leftDown };
	vertices[10] = { position + rightUpBack,	backTexCoords + leftUp };
	vertices[11] = { position + leftUpBack,	backTexCoords + rightUp };
	//LEFT
	vertices[12] = { position + leftDownFront,leftTexCoords + rightDown };
	vertices[13] = { position + leftDownBack,	leftTexCoords + leftDown };
	vertices[14] = { position + leftUpBack,	leftTexCoords + leftUp };
	vertices[15] = { position + leftUpFront,	leftTexCoords + rightUp };
	//TOP
	vertices[16] = { position + leftUpFront,	topTexCoords + leftDown };
	vertices[17] = { position + rightUpFront,	topTexCoords + rightDown };
	vertices[18] = { position + rightUpBack,	topTexCoords + rightUp };
	vertices[19] = { position + leftUpBack,	topTexCoords + leftUp };
	//BOTTOM
	vertices[20] = { position + leftDownFront,	bottomTexCoords + leftUp };
	vertices[21] = { position + rightDownFront,bottomTexCoords + rightUp };
	vertices[22] = { position + rightDownBack,	bottomTexCoords + rightDown };
	vertices[23] = { position + leftDownBack,	bottomTexCoords + leftDown };
	return vertices;
}
