#include "renderText.hpp"

// these functions are based on what Jim did in class
void TextRenderer::UploadTexture(GLuint &tex, void* data, uint32_t w, uint32_t h, uint32_t colorformat){
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, colorformat, w, h, 0, colorformat, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
}

PosTexVertex verts[] = {
	{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
	{ glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
	{ glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
	{ glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f) }
};
constexpr uint32_t vertsSize = sizeof(PosTexVertex) * 4;

//set up vertex array:
void TextRenderer::SetupBufferandVAO(GLuint& vao, GLuint& buffer) 
{
	glGenBuffers(1, &buffer);
	glGenVertexArrays(1, &vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBindVertexArray(vao);

	glVertexAttribPointer(Position_vec4, 3, GL_FLOAT, GL_FALSE, sizeof(PosTexVertex), (GLbyte*)0 + offsetof(PosTexVertex, Position));
	glEnableVertexAttribArray(Position_vec4);

	glVertexAttribPointer(TexCoord_vec2, 2, GL_FLOAT, GL_FALSE, sizeof(PosTexVertex), (GLbyte*)0 + offsetof(PosTexVertex, TexCoord));
	glEnableVertexAttribArray(TexCoord_vec2);

	glBufferData(GL_ARRAY_BUFFER, vertsSize, &verts, GL_STREAM_DRAW);
}

void TextRenderer::LoadText(){
	// GLuint idtracker = 0;
	// setting up characters for text
	std::string fontstr = Files::Path("rainyhearts.ttf"); //test font for now
	const char *fontfile = fontstr.c_str();
	constexpr int FONT_SIZE =36;
	{
		FT_Library library;
		FT_Face ft_face;
		FT_Error ft_error;

		if ((ft_error = FT_Init_FreeType( &library )))
			abort();
		if ((ft_error = FT_New_Face (library, fontfile, 0, &ft_face))) //0 if there is only one face in the font file
			abort();
		if ((ft_error = FT_Set_Char_Size (ft_face, FONT_SIZE*64, FONT_SIZE*64, 0, 0))) //horizontal + vertical resolution -- default 72dpi
			abort();

		// want to use ft to create our text
		// code based on : https://learnopengl.com/In-Practice/Text-Rendering
		// also uses https://freetype.org/freetype2/docs/tutorial/step1.html
			
		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		for (unsigned char c = 0; c < 128; c++){
			GLuint idtracker = (int)c;
			// printf("HOW %d %c %d\n",(int)c, (char)c,idtracker);
			FT_UInt  glyph_index;
			/* retrieve glyph index from character code */
			glyph_index = FT_Get_Char_Index(ft_face, c );

			/* load glyph image into the slot (erase previous one) */
			if((ft_error = FT_Load_Glyph(ft_face, glyph_index, FT_LOAD_DEFAULT ))) continue;
			/* convert to an anti-aliased bitmap */
			if((ft_error = FT_Render_Glyph(ft_face->glyph, FT_RENDER_MODE_NORMAL ))) continue;
					
			// generate texture
			//std::vector< glm::u8vec4 > data;
			UploadTexture(idtracker, ft_face->glyph->bitmap.buffer, ft_face->glyph->bitmap.width, ft_face->glyph->bitmap.rows,GL_RED);
				
			Character character = {
				idtracker,
				glm::ivec2(ft_face->glyph->bitmap.width, ft_face->glyph->bitmap.rows),
				glm::ivec2(ft_face->glyph->bitmap_left, ft_face->glyph->bitmap_top),
				static_cast<unsigned int>(ft_face->glyph->advance.x)
			};
			Characters.emplace_back(character);
				
			SetupBufferandVAO(idtracker, idtracker); 

			Text character_tex = {
				idtracker, idtracker, idtracker, (GLuint) (4)
			};
			font_texs.emplace_back(character_tex);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D, 0);
		FT_Done_Face (ft_face);
		FT_Done_FreeType (library);
	}
	GL_ERRORS();
}

// code based on : https://learnopengl.com/In-Practice/Text-Rendering
void TextRenderer::RenderText(const std::string& text, float xin, float yin, float scale,float rightlimit, int visibleCharacters)
{
	assert(visibleCharacters == -1 || visibleCharacters <= text.size());

	// bind shader only once!
	fontShader.Use();

	//texture example drawing
	float x = xin * 2.0f/1920.0f - 1.0f;
	float y = yin * 2.0f/1080.0f-1.0;
	float r = rightlimit *2.0f/1920.0f - 1.0f;
	float origx =x;

	std::string::const_iterator c;
	std::string::const_iterator endit = visibleCharacters == -1 ? text.end() : text.begin() + visibleCharacters;
	for (c = text.begin(); c != endit; c++)
	{
		if (*c=='\n'){
			y -= (40) * scale; //idk just a guess
			x = origx;
			continue;
		}

		const Character& ch = Characters[(int)*c];
		const Text& tex = font_texs[(int)*c]; // ok this could conflict with other images being rendered I think, so have it be the first one

		float xpos = x + ch.Bearing.x * scale;
			//float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
		float ypos = y + ch.Bearing.y * scale;

		//prevent text from going off screen, probably best to include a dash but don't have time
		if(xpos>=r-.05f){
			y -= (40) * scale; //idk just a guess
			x = origx;
			xpos = x + ch.Bearing.x * scale;
			ypos = y +ch.Bearing.y* scale;
		}

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// float w =  20*scale;
		// float h = 20*scale;
		// printf("huh %d %d %c\n",ch.TextureID,tex->tex,*c);

		// render
		{
			// Create a transformation matrix that scales and translates to the correct position in clip space
			glm::mat4 toClip = glm::mat4(1.0f); // Start with the identity matrix

			// Translate to the desired position
			toClip = glm::translate(toClip, glm::vec3(xpos, ypos, 0));

			// Scale to the specified width (w) and height (-h, flipped vertically)
			toClip = glm::scale(toClip, glm::vec3(w, -1.0f * h, 1.0f));

			// push constant
			fontShader.SetMatrix4("CLIP_FROM_LOCAL", toClip);

			// bind vertex array
			glBindVertexArray(tex.tristrip_for_texture_program);

			// bind texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex.tex);

			// draw
			glDrawArrays(GL_TRIANGLE_STRIP, 0, tex.count);
		}

		// advance
		x += (ch.Advance >> 6) * scale ; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	glBindVertexArray(0);
}

TextRenderer::TextRenderer()
{
	fontShader = ResourceManager::GetShader(FONT_SHADER);

	Position_vec4 = glGetAttribLocation(fontShader.ID, "Position");
	TexCoord_vec2 = glGetAttribLocation(fontShader.ID, "TexCoord");

	LoadText();
}

TextRenderer::~TextRenderer()
{
	// shader is automatically cleaned up by resource manager
}
