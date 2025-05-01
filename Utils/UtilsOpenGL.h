//
// Created by Edw590 on 27/04/2025.
//

#ifndef OPENGL_UTILSOPENGL_H
#define OPENGL_UTILSOPENGL_H


#define ASSERT(x) if (!(x)) __builtin_trap()
#define GL_CALL(x) clearGLErrors();\
	x;\
	ASSERT(checkGLErrors(#x, __FILE__, __LINE__))

struct ObjectData {
	GLuint vertex_buffer_id;
	GLuint color_buffer_id;
	GLuint index_buffer_id;
	int index_count;
};

GLuint loadShader(GLuint type, const char *shader_code);
GLuint createProgram();
bool getShaderSources(const char *file_name, char *sources[2]);

void clearGLErrors();
bool checkGLErrors(const char *function, const char* file, int line);


#endif //OPENGL_UTILSOPENGL_H
