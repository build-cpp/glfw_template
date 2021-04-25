// Based on: https://gist.github.com/iondune/bf24795910abdcfa3360

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

static void PrintOpenGLErrors(const char* Function, const char* File, int Line)
{
	bool Succeeded = true;

	GLenum Error = glGetError();
	if (Error != GL_NO_ERROR)
	{
		const char* ErrorString = (const char*)gluErrorString(Error);
		if (ErrorString)
			std::cerr << ("OpenGL Error in %s at line %d calling function %s: '%s'", File, Line, Function, ErrorString) << std::endl;
		else
			std::cerr << ("OpenGL Error in %s at line %d calling function %s: '%d 0x%X'", File, Line, Function, Error, Error) << std::endl;
	}
}

#ifdef _DEBUG
#define CheckedGLCall(x) do { PrintOpenGLErrors(">>BEFORE<< "#x, __FILE__, __LINE__); (x); PrintOpenGLErrors(#x, __FILE__, __LINE__); } while (0)
#define CheckedGLResult(x) (x); PrintOpenGLErrors(#x, __FILE__, __LINE__);
#define CheckExistingErrors(x) PrintOpenGLErrors(">>BEFORE<< "#x, __FILE__, __LINE__);
#else
#define CheckedGLCall(x) (x)
#define CheckedGLResult(x) (x)
#define CheckExistingErrors(x)
#endif

static void PrintShaderInfoLog(GLint const Shader)
{
	int InfoLogLength = 0;
	int CharsWritten = 0;

	glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (InfoLogLength > 0)
	{
		GLchar* InfoLog = new GLchar[InfoLogLength];
		glGetShaderInfoLog(Shader, InfoLogLength, &CharsWritten, InfoLog);
		std::cout << "Shader Info Log:" << std::endl << InfoLog << std::endl;
		delete[] InfoLog;
	}
}

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
		glfwTerminate();
		return -1;
	}

	const char* VertexShaderSource = R"GLSL(
		#version 150
		in vec2 position;
		void main()
		{
			gl_Position = vec4(position, 0.0, 1.0);
		}
	)GLSL";

	const char* FragmentShaderSource = R"GLSL(
		#version 150
		out vec4 outColor;
		void main()
		{
			outColor = vec4(1.0, 1.0, 1.0, 1.0);
		}
	)GLSL";

	const GLfloat Vertices[] = {
		0.0f, 0.5f,
		0.5f, -0.5f,
		-0.5f, -0.5f
	};

	const GLuint Elements[] = {
		0, 1, 2
	};

	GLuint VAO;
	CheckedGLCall(glGenVertexArrays(1, &VAO));
	CheckedGLCall(glBindVertexArray(VAO));

	GLuint VBO;
	CheckedGLCall(glGenBuffers(1, &VBO));
	CheckedGLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	CheckedGLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW));
	CheckedGLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

	GLuint EBO;
	CheckedGLCall(glGenBuffers(1, &EBO));
	CheckedGLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	CheckedGLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Elements), Elements, GL_STATIC_DRAW));

	GLint Compiled;
	GLuint VertexShader = CheckedGLResult(glCreateShader(GL_VERTEX_SHADER));
	CheckedGLCall(glShaderSource(VertexShader, 1, &VertexShaderSource, NULL));
	CheckedGLCall(glCompileShader(VertexShader));
	CheckedGLCall(glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Compiled));
	if (!Compiled)
	{
		std::cerr << "Failed to compile vertex shader!" << std::endl;
		PrintShaderInfoLog(VertexShader);
	}

	GLuint FragmentShader = CheckedGLResult(glCreateShader(GL_FRAGMENT_SHADER));
	CheckedGLCall(glShaderSource(FragmentShader, 1, &FragmentShaderSource, NULL));
	CheckedGLCall(glCompileShader(FragmentShader));
	CheckedGLCall(glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Compiled));
	if (!Compiled)
	{
		std::cerr << "Failed to compile fragment shader!" << std::endl;
		PrintShaderInfoLog(FragmentShader);
	}

	GLuint ShaderProgram = CheckedGLResult(glCreateProgram());
	CheckedGLCall(glAttachShader(ShaderProgram, VertexShader));
	CheckedGLCall(glAttachShader(ShaderProgram, FragmentShader));
	CheckedGLCall(glBindFragDataLocation(ShaderProgram, 0, "outColor"));
	CheckedGLCall(glLinkProgram(ShaderProgram));
	CheckedGLCall(glUseProgram(ShaderProgram));

	GLint PositionAttribute = CheckedGLResult(glGetAttribLocation(ShaderProgram, "position"));
	CheckedGLCall(glEnableVertexAttribArray(PositionAttribute));

	CheckedGLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	CheckedGLCall(glVertexAttribPointer(PositionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0));
	CheckedGLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

	while (!glfwWindowShouldClose(window))
	{
		CheckedGLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		CheckedGLCall(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	CheckedGLCall(glDeleteProgram(ShaderProgram));
	CheckedGLCall(glDeleteShader(FragmentShader));
	CheckedGLCall(glDeleteShader(VertexShader));

	CheckedGLCall(glDeleteBuffers(1, &EBO));
	CheckedGLCall(glDeleteBuffers(1, &VBO));
	CheckedGLCall(glDeleteVertexArrays(1, &VAO));

	glfwTerminate();
	return 0;
}