#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cmath>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

using namespace std;

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

int lastX = SCREEN_WIDTH / 2;
int lastY = SCREEN_HEIGHT / 2;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float currentFrame = 0.0f;

GLFWwindow *window;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    // automaticaly takes the x and y positions of the mouse
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    // ensures the first mouse movement is not a jump starts in starting
    // position
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX; // change since last frame
    float yoffset = lastY - ypos; // change since last frame
    lastX = xpos; // updates the last x position for the next frame
    lastY = ypos; // updates the last y position for the next frame
}

// handled by the glfw callback function
void input_callback(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void checkOpenGLError()
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        std::cerr << "OpenGL Error: " << err << std::endl;
    }
}

int initGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,
                   3); // Set the major version of the OpenGL context to 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,
                   3); // Set the minor version of the OpenGL context to 3
    glfwWindowHint(GLFW_OPENGL_PROFILE,
                   GLFW_OPENGL_CORE_PROFILE); // Set the OpenGL profile to core
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    // Create windowed mode window and OpenGL context
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT,
                                          "LearnOpenGL", NULL, NULL);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (window == NULL)
    {
        checkOpenGLError();
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK)
    {
        cout << "Failed to initialize GLEW" << endl;
        return 1;
    }
    return 0;
}

int main()
{
    if (initGLFW() != 0)
    {
        cout << "Failed to initialize GLFW" << endl;
        return 1;
    }

    while (!glfwWindowShouldClose(window))
    {
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        input_callback(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set the color of the window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    cout << "GLFW initialized" << endl;
    return 0;
}
