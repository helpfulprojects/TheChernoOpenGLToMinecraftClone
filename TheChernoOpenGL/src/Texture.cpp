#include "Texture.h"
#include "stb_image/stb_image.h"
#include <GL\glew.h>
#include "ErrorManager.h"
#include <iostream>
#include <cmath>

const float Texture::m_Offset = (float)(Texture::m_AtlasSize / (float)Texture::m_TexturesPerRow) / (float)Texture::m_AtlasSize;

Texture::Texture(const std::string& filepath): m_RendererID(0), m_Filepath(filepath), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{

	stbi_set_flip_vertically_on_load(1);
	GlCall(glGenTextures(1, &m_RendererID));
	GlCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	m_LocalBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GlCall(glGenerateMipmap(GL_TEXTURE_2D));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GlCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0));
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic") || GLEW_EXT_texture_filter_anisotropic) {
		std::cout << "Anisotropic filtering supported" << std::endl;
		GLfloat aniso = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &aniso);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, std::min(4.0f,aniso));
	}
	else {
		std::cout << "Anisotropic filtering not supported" << std::endl;
	}
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	Unbind();

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	GlCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GlCall(glActiveTexture(GL_TEXTURE0 + slot));
	GlCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	GlCall(glBindTexture(GL_TEXTURE_2D, 0));
}
