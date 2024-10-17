#include "TextTextureProgram.hpp"
#include "gl_errors.hpp"
#include "core/Files.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include <stddef.h>
#include <stdio.h>
#include FT_FREETYPE_H


struct PosTexVertex {
	glm::vec3 Position;
	glm::vec2 TexCoord;
};
static_assert( sizeof(PosTexVertex) == 3*4 + 2*4, "PosTexVertex is packed." );

struct text {
	GLuint tex = 0; //created at startup
	GLuint tristrip = 0; //vertex buffer (of PosTexVertex)
	GLuint tristrip_for_texture_program = 0; //vertex array object

	GLuint count = 0; //number of vertices in buffer
	glm::mat4 CLIP_FROM_LOCAL = glm::mat4(1.0f); //transform to use when drawing
} ;
// std::vector<text> font_texs;

// text
struct Character {
	unsigned int TextureID;  // ID handle of the glyph texture
	glm::ivec2   Size;       // Size of glyph
	glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph
};
// std::vector<Character> Characters; 

// these functions are based on what Jim did in class
void UploadTexture(GLuint &tex, void* data,uint w,uint h, uint colorformat);
void SetupBufferandVAO(GLuint &vao,GLuint &buffer);

void loadText(std::vector<Character>& Characters ,std::vector<text> &font_texs);


void renderImg(text tex);


// code based on : https://learnopengl.com/In-Practice/Text-Rendering
void RenderText(std::string text, float x, float y, float scale, std::vector<Character>& Characters,std::vector<struct text>& font_texs);