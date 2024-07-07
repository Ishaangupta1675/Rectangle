#include <iostream>
using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint Width = 800;
const GLint Height = 800;

GLuint VAO, VBO, shader;

//vertex shader
static const char* vShader = "                                     \n\
#version 330                                                       \n\
layout (location = 0) in vec3 pos;                                 \n\
void main() {                                                      \n\
	gl_Position = vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);      \n\
}                                                                  \n\
";

//fragment shader
static const char* fShader = "                                     \n\
    #version 330                                                   \n\
	out vec4 colour;                                               \n\
	void main() {                                                  \n\
		colour = vec4(1.0, 1.0, 1.0, 1.0);                         \n\
	}                                                              \n\
";


void AddShader(GLuint theProgram, const char* shadercode, GLenum shadertype) {
    GLuint theShader = glCreateShader(shadertype);

    const GLchar* theCode[1];
    theCode[0] = shadercode;

    GLint codeLength[1];
    codeLength[0] = strlen(shadercode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        cout << "Error compiling the " << shadertype << " shader: " << eLog;
        return;
    }

    glAttachShader(theProgram, theShader);
    return;
}

void CompileShaders() {
    shader = glCreateProgram();
    if (!shader) {
        cout << "Error creating shader program";
        return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        cout << "Error linking program: " << eLog;
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        cout << "Error validating program: " << eLog;
        return;
    }
}

void createTriangle() {
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

int main()
{
    if (!glfwInit()) {
        cout << "GLFW initialization failed ";
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainwindow = glfwCreateWindow(Width, Height, "Test window", NULL, NULL);
    if (!mainwindow) {
        cout << "GLFW_Window_creation_failed";
        glfwTerminate();
        return 1;
    }

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainwindow, &bufferWidth, &bufferHeight);

    glfwMakeContextCurrent(mainwindow);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        cout << "GLEW failed..";
        glfwDestroyWindow(mainwindow);
        glfwTerminate();
        return 1;
    }

    glViewport(0, 0, bufferWidth, bufferHeight);

    createTriangle();
    CompileShaders();

    while (!glfwWindowShouldClose(mainwindow)) {
        glfwPollEvents();

        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glUseProgram(0);

        glfwSwapBuffers(mainwindow);
    }

    return 0;
}