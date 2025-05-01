#include <stdio.h>
#include <stdlib.h>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Utils/UtilsOpenGL.h"

#define MAX_OBJECTS 100

int num_objects_GL = 0;
struct ObjectData objects_GL[100] = {0};

static void addObject(const GLfloat *vertices, const GLfloat *colors, const GLushort *indices, GLuint vertex_count,
					  GLuint index_count) {
	struct ObjectData object;
	GL_CALL(glGenBuffers(1, &object.vertex_buffer_id));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, object.vertex_buffer_id));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, vertex_count * 3 * sizeof(vertices[0]), vertices, GL_STATIC_DRAW));

	GL_CALL(glGenBuffers(1, &object.color_buffer_id));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, object.color_buffer_id));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, vertex_count * 4 * sizeof(colors[0]), colors, GL_STATIC_DRAW));

	GL_CALL(glGenBuffers(1, &object.index_buffer_id));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.index_buffer_id));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(indices[0]), indices, GL_STATIC_DRAW));

	if (num_objects_GL >= MAX_OBJECTS) {
		fprintf(stderr, "Maximum number of objects reached\n");
		return;
	}

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	objects_GL[num_objects_GL].vertex_buffer_id = object.vertex_buffer_id;
	objects_GL[num_objects_GL].color_buffer_id = object.color_buffer_id;
	objects_GL[num_objects_GL].index_buffer_id = object.index_buffer_id;
	objects_GL[num_objects_GL].index_count = index_count;

	num_objects_GL++;
}

int main(void) {
	GLFWwindow* window;

	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");

		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window) {
		fprintf(stderr, "Failed to create GLFW window\n");

		glfwTerminate();

		return EXIT_FAILURE;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");

		return EXIT_FAILURE;
	}

	printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));

	addObject((GLfloat[]) {
			-1.0f, -1.0f, 0.0f,
			 0.0f, -1.0f, 0.0f,
			 0.0f,  0.0f, 0.0f,
			-1.0f,  0.0f, 0.0f,
		},
		(GLfloat[]) {
			1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
		},
		(GLushort[]) {
			0, 1, 2,
			2, 3, 0,
		},
		4, 6);

	addObject((GLfloat[]) {
			1.0f,  1.0f, 0.0f,
			-0.0f,  1.0f, 0.0f,
			-0.0f, -0.0f, 0.0f,
			1.0f, -0.0f, 0.0f,
		},
		(GLfloat[]) {
			1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
		},
		(GLushort[]) {
			0, 1, 2,
			2, 3, 0,
		},
		4, 6);

	GLuint program_handle = createProgram();
	if (program_handle == 0) {
		fprintf(stderr, "Error creating OpenGL program");

		return EXIT_FAILURE;
	}
	GL_CALL(glUseProgram(program_handle));

	GL_CALL(GLuint scale_id = glGetUniformLocation(program_handle, "u_scale"));
	GL_CALL(GLuint position_id = glGetAttribLocation(program_handle, "a_position"));
	GL_CALL(GLuint color_id = glGetAttribLocation(program_handle, "a_color"));

	float scale = 0.0f;
	float increment = 0.05f;

	while (!glfwWindowShouldClose(window)) {
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

		for (int i = 0; i < num_objects_GL; i++) {
			struct ObjectData object = objects_GL[i];

			GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, object.vertex_buffer_id));
			GL_CALL(glEnableVertexAttribArray(position_id));
			GL_CALL(glVertexAttribPointer(position_id, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0));

			GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, object.color_buffer_id));
			GL_CALL(glEnableVertexAttribArray(color_id));
			GL_CALL(glVertexAttribPointer(color_id, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0));

			GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.index_buffer_id));
			GL_CALL(glDrawElements(GL_TRIANGLES, object.index_count, GL_UNSIGNED_SHORT, 0));
		}

		if (scale > 1.0f) {
			increment = -0.05f;
		} else if (scale < 0.0f) {
			increment = 0.05f;
		}
		scale += increment;
		GL_CALL(glUniform1f(scale_id, scale));

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
