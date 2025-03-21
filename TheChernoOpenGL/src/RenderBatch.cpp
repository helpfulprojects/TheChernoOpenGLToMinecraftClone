#include "RenderBatch.h"

RenderBatch::RenderBatch(): m_Va(0), m_Vb(0)
{
	GlCall(glGenVertexArrays(1, &m_Va));
	GlCall(glBindVertexArray(m_Va));

	GlCall(glGenBuffers(1, &m_Vb));
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_Vb));
	glBufferData(GL_ARRAY_BUFFER, BATCH_SIZE*sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	GlCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex,Position))));
	GlCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex,TexCoords))));
	GlCall(glEnableVertexAttribArray(0));
	GlCall(glEnableVertexAttribArray(1));

	unsigned int EBO;
	GlCall(glGenBuffers(1, &EBO));
	unsigned int indices[BATCH_SIZE * 6];
	for (int i = 0; i < BATCH_SIZE; i++)
	{
		int index = i * 6;
		int offset = 4 * i;
		indices[index + 0] = 0 + offset;
		indices[index + 1] = 1 + offset;
		indices[index + 2] = 2 + offset;
		indices[index + 3] = 0 + offset;
		indices[index + 4] = 2 + offset;
		indices[index + 5] = 3 + offset;
	}
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
	GlCall(glBindVertexArray(0));
}

bool RenderBatch::AddVertex(const Vertex& vertex)
{
	if (m_Vertices.size() >= BATCH_SIZE)
	{
		return false;
	}
	m_Vertices.push_back(vertex);
	return true;
}

void RenderBatch::Draw()
{
	GlCall(glBindVertexArray(m_Va));
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_Vb));
	GlCall(glBufferSubData(GL_ARRAY_BUFFER, 0, m_Vertices.size()*sizeof(Vertex), &m_Vertices[0]));
	GlCall(glDrawElements(GL_TRIANGLES, BATCH_SIZE*6, GL_UNSIGNED_INT, 0));
	GlCall(glBindVertexArray(0));
}

