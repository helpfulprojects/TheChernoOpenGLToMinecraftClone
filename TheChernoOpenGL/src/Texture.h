#pragma once
#include <string>
class Texture {
public:
	Texture(const std::string& filepath);
	~Texture();

	void Bind(unsigned int slot=0) const;
	void Unbind() const;

	inline static float AtlasBlockX(unsigned int column) { return m_Offset * column; }
	inline static float AtlasBlockY(unsigned int row) { return 1-(m_Offset * row) - m_Offset; }

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	const static unsigned int m_AtlasSize = 2048;
	const static unsigned int m_TexturesPerRow = 16;
	const static float m_Offset;
private:
	unsigned int m_RendererID;
	std::string m_Filepath;
	unsigned char* m_LocalBuffer;
	int m_Width;
	int m_Height;
	int m_BPP;
};
