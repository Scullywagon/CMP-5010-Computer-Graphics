#include "Camera.h"
#include "CollisionManager.h"
#include "Cube.h"
#include "FerrisWheel.h"
#include "Floor.h"
#include "Light.h"
#include "Mesh.h"
#include "Model.h"
#include "ParentCamera.h"
#include "PersonCamera.h"
#include "Scene.h"
#include "Skybox.h"
#include "glm/detail/type_mat.hpp"
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

bool willRotate = false;

Scene *scene;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// handled by the glfw callback function
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

    scene->camera->ProcessMouseMovement(xoffset, yoffset);
}

// handled by the glfw callback function
void input_callback(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        scene->camera->processInput(Camera::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        scene->camera->processInput(Camera::BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        scene->camera->processInput(Camera::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        scene->camera->processInput(Camera::RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        scene->camera->processInput(Camera::UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        scene->camera->processInput(Camera::DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        scene->changeCamera(currentFrame);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        scene->activateRotation();
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

int main()
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
    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT,
                                          "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK)
    {
        cout << "Failed to initialize GLEW" << endl;
        return -1;
    }

    // depth testing for 3D renders
    glEnable(GL_DEPTH_TEST);

    scene = new Scene(SCREEN_WIDTH, SCREEN_HEIGHT);

    while (!glfwWindowShouldClose(window))
    {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        input_callback(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set the color of the window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene->use(deltaTime);
        checkOpenGLError();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
