#include "TextTextureProgram.hpp"

#include "gl_compile_program.hpp"
#include "gl_errors.hpp"

Load< TextTextureProgram > texture_program(LoadTagEarly, []() -> TextTextureProgram const * {
	TextTextureProgram *ret = new TextTextureProgram();
	return ret;
});

TextTextureProgram::TextTextureProgram() {
	//Compile vertex and fragment shaders using the convenient 'gl_compile_program' helper function:
	program = gl_compile_program(
		//vertex shader:
		"#version 330\n"
		"uniform mat4 CLIP_FROM_LOCAL;\n"
		"in vec4 Position;\n"
		"in vec2 TexCoord;\n"
		"out vec2 texCoord;\n"
		"void main() {\n"
		"	gl_Position = CLIP_FROM_LOCAL * Position;\n"
		"	texCoord = TexCoord;\n"
		"}\n"
	,
		//fragment shader:
		"#version 330\n"
		"uniform sampler2D TEX;\n"
		"in vec2 texCoord;\n"
		"out vec4 fragColor;\n"
		"void main() {\n"
		"	fragColor = texture(TEX, texCoord);\n"
		"	if(fragColor==vec4(0.0f,0,0,1))\n"
		"		{fragColor =vec4(1.0f,1,1,0);}\n"
		"	else\n"
		"		{fragColor.x = 1.0f;\n"
				"fragColor.y = 1.0f;\n"
				"fragColor.z = 1.0f;\n"
		"	}\n"
		"}\n"
	);
	//As you can see above, adjacent strings in C/C++ are concatenated.
	// this is very useful for writing long shader programs inline.

	//look up the locations of vertex attributes:
	Position_vec4 = glGetAttribLocation(program, "Position");
	TexCoord_vec2 = glGetAttribLocation(program, "TexCoord");

	//look up the locations of uniforms:
	CLIP_FROM_LOCAL_mat4 = glGetUniformLocation(program, "CLIP_FROM_LOCAL");

	GLuint TEX_sampler2D = glGetUniformLocation(program, "TEX");

	//set TEX to always refer to texture binding zero:
	glUseProgram(program); //bind program -- glUniform* calls refer to this program now

	glUniform1i(TEX_sampler2D, 0); //set TEX to sample from GL_TEXTURE0

	glUseProgram(0); //unbind program -- glUniform* calls refer to ??? now
}

TextTextureProgram::~TextTextureProgram() {
	glDeleteProgram(program);
	program = 0;
}

