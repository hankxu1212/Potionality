#pragma once

#include "gl_errors.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "core/Files.hpp"
#include "core/Module.hpp"
#include "core/ResourceManager.h"

#define FONT_SHADER "fontShader"

struct PosTexVertex {
	glm::vec3 Position;
	glm::vec2 TexCoord;
};
static_assert( sizeof(PosTexVertex) == 3*4 + 2*4, "PosTexVertex is packed." );

struct Text {
	GLuint tex = 0; //created at startup
	GLuint tristrip = 0; //vertex buffer (of PosTexVertex)
	GLuint tristrip_for_texture_program = 0; //vertex array object

	GLuint count = 0; //number of vertices in buffer
};

struct Character {
	unsigned int TextureID;  // ID handle of the glyph texture
	glm::ivec2   Size;       // Size of glyph
	glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph
};

class TextRenderer : public Module::Registrar<TextRenderer>
{
	inline static const bool Registered = Register(UpdateStage::Never, DestroyStage::Normal);
public:
	TextRenderer();

	virtual ~TextRenderer();

	// code based on : https://learnopengl.com/In-Practice/Text-Rendering
	void RenderText(const std::string& text, float x, float y, float scale= 0.001f,float rightlimit = 1920);

private:
	// these functions are based on what Jim did in class
	void UploadTexture(GLuint &tex, void* data,uint32_t w,uint32_t h, uint32_t colorformat);
	void SetupBufferandVAO(GLuint &vao, GLuint &buffer);

	void LoadText();

	std::vector<Character> Characters;
	std::vector<Text> font_texs;

	Shader fontShader;
	GLuint Position_vec4;
	GLuint TexCoord_vec2;
};
