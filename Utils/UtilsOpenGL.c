//
// Created by Edw590 on 27/04/2025.
//

#include <stdio.h>
#include <malloc.h>
#include "GL/glew.h"
#include "UtilsOpenGL.h"
#include "UtilsStrings.h"

GLuint createProgram() {
	char *sources[2];
	if (!getShaderSources("C:\\Users\\Edw590\\CLionProjects\\OpenGL\\res\\shaders\\Basic.glsl", sources)) {
		fprintf(stderr, "Failed to parse shader\n");

		return 0;
	}

	GLuint vertex_shader_id = loadShader(GL_VERTEX_SHADER, sources[0]);
	if (vertex_shader_id == 0) {
		fprintf(stderr, "Error creating vertex shader");

		return 0;
	}
	GLuint fragment_shader_id = loadShader(GL_FRAGMENT_SHADER, sources[1]);
	if (fragment_shader_id == 0) {
		fprintf(stderr, "Error creating fragment shader");

		return 0;
	}

	GL_CALL(GLuint program_id = glCreateProgram());
	GL_CALL(glAttachShader(program_id, vertex_shader_id));
	GL_CALL(glAttachShader(program_id, fragment_shader_id));
	GL_CALL(glLinkProgram(program_id));

	free(sources[0]);
	free(sources[1]);

	return program_id;
}

GLuint loadShader(GLuint type, const char *shader_code) {
	GL_CALL(GLuint shader_id = glCreateShader(type));
	GL_CALL(glShaderSource(shader_id, 1, &shader_code, NULL));
	GL_CALL(glCompileShader(shader_id));

	GLint compile_Status;
	GL_CALL(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_Status));
	if (compile_Status == GL_FALSE) {
		GLint length;
		GL_CALL(glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length));

		char *message = malloc(length);
		GL_CALL(glGetShaderInfoLog(shader_id, length, NULL, message));
		fprintf(stderr, "Error compiling shader:\n%s\n", message);
		free(message);

		GL_CALL(glDeleteShader(shader_id));

		return 0;
	}

	return shader_id;
}

bool getShaderSources(const char *file_name, char *sources[2]) {
	FILE *fp = fopen(file_name, "r");
	if (fp == NULL) {
		fprintf(stderr, "Failed to open file: %s\n", file_name);

		return false;
	}

	fseek(fp, 0, SEEK_END);
	long length = ftell(fp);
	if (length < 0) {
		fprintf(stderr, "Failed to get file length: %s\n", file_name);
		fclose(fp);

		return false;
	}
	rewind(fp);

	char *file_contents = malloc(length + 1);
	if (file_contents == NULL) {
		fprintf(stderr, "Failed to allocate memory for shader source\n");
		fclose(fp);

		return false;
	}

	fread(file_contents, sizeof(char), length, fp);
	fclose(fp);

	file_contents[length] = '\0';

	char **split_file = splitString(file_contents, "/////////////////////////////////////////////////////////////////"
												   "///////////////////////////////////////////////////////", NULL);
	free(file_contents);
	sources[0] = split_file[0];
	sources[1] = split_file[1];

	free(split_file);

	return true;
}

void clearGLErrors() {
	while (glGetError() != GL_NO_ERROR);
}

bool checkGLErrors(const char *function, const char* file, int line) {
	bool errors = false;
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		errors = true;
		fprintf(stderr, "OpenGL error in \"%s\" (%s:%d): ", function, file, line);
		switch (error) {
			case GL_NO_ERROR:
				break;
			case GL_INVALID_ENUM:
				fprintf(stderr, "GL_INVALID_ENUM\n");
				break;
			case GL_INVALID_VALUE:
				fprintf(stderr, "GL_INVALID_VALUE\n");
				break;
			case GL_INVALID_OPERATION:
				fprintf(stderr, "GL_INVALID_OPERATION\n");
				break;
			case GL_STACK_OVERFLOW:
				fprintf(stderr, "GL_STACK_OVERFLOW\n");
				break;
			case GL_STACK_UNDERFLOW:
				fprintf(stderr, "GL_STACK_UNDERFLOW\n");
				break;
			case GL_OUT_OF_MEMORY:
				fprintf(stderr, "GL_OUT_OF_MEMORY\n");
				break;
			default:
				fprintf(stderr, "Unknown error: %X\n", error);
		}
	}

	return !errors;
}
