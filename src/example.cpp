// Copied from: https://github.com/melchi45/glfw_example/blob/master/src/glfw_example.cpp
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#define PIXEL_FORMAT GL_RGB

// reference from 
// https://gist.github.com/victusfate/9214902
// https://nervous.io/ffmpeg/opengl/2017/01/31/ffmpeg-opengl/

static const GLchar *v_shader_source =
"attribute vec2 position;\n"
"varying vec2 texCoord;\n"
"void main(void) {\n"
"  gl_Position = vec4(position, 0, 1);\n"
"  texCoord = position;\n"
"}\n";

static const GLchar *f_shader_source =
"uniform sampler2D tex;\n"
"varying vec2 texCoord;\n"
"void main() {\n"
"  gl_FragColor = texture2D(tex, texCoord * 0.5 + 0.5);\n"
"}\n";
/*
typedef struct {
	const AVClass *class;
	GLuint        program;
	GLuint        frame_tex;
	GLFWwindow    *window;
	GLuint        pos_buf;
} GenericShaderContext
*/
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void)
{
    GLFWwindow* window;

	glfwSetErrorCallback(error_callback);

    // Initialize the library
    if (!glfwInit())
        return -1;

	// cout << "default shader lang: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	
	// select opengl version
	 int major, minor, rev;
	 glfwGetVersion(&major, &minor, &rev);
	 std::cout << "glfw major.minor " << major << "." << minor << "." << rev << std::endl;

	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
    // Create a windowed mode window and its OpenGL context 
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

    // Loop until the user closes the window 
    while (!glfwWindowShouldClose(window))
    {
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
		glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(-0.6f, -0.4f, 0.f);
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(0.6f, -0.4f, 0.f);
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(0.f, 0.6f, 0.f);
		glEnd();
		glfwSwapBuffers(window);
		glfwPollEvents();
    }

    glfwTerminate();
	
    return 0;
}